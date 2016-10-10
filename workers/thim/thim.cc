#include <improbable/worker.h>
#include <iostream>
#include <thread>
#include <eeci/building/temperature.h>
#include <eeci/building/traits.h>
#include <eeci/building/hvac.h>

using eeci::building::Temperature;

static const std::string worker_type = "Thim";
static const std::string logger_name = "thim.cc";

static const unsigned seconds_per_update = 5;
static const unsigned fast_forward_factor = 100;
static const double heating_set_point = 21;
static const double cooling_set_point = 26;
static const double outside_temperature = 30;

double CalculateNextTemperature(double current_temperature, double outside_temperature,
                                double heat_mass_capacity, double heat_transmission,
                                double heating_cooling_power, int time_step_length_seconds) {
    const double dt_by_cm = time_step_length_seconds / heat_mass_capacity;
    return (current_temperature * (1 - dt_by_cm * heat_transmission) +
            dt_by_cm * (heating_cooling_power + heat_transmission * outside_temperature));
}

double CalculateNextBuildingTemperature(const worker::Entity& entity) {
    const double current_temperature = entity.Get<Temperature>()->current_temperature();
    const auto traits = entity.Get<eeci::building::Traits>();
    const double heat_transmission = traits->heat_transmission();
    const double heat_mass_capacity = traits->heat_mass_capacity();
    auto next_temperature = [&] (double heating_cooling_power) {
        return CalculateNextTemperature(
                current_temperature,
                outside_temperature,
                heat_mass_capacity,
                heat_transmission,
                heating_cooling_power,
                seconds_per_update * fast_forward_factor
        );
    };
    const auto next_temperature_no_power = next_temperature(0);
    if (next_temperature_no_power >= heating_set_point && next_temperature_no_power <= cooling_set_point){
        return next_temperature_no_power;
    } else {
        const auto hvac = entity.Get<eeci::building::Hvac>();
        double set_point;
        double max_power;
        if (next_temperature_no_power < heating_set_point) {
            set_point = heating_set_point;
            max_power = hvac->max_heating_power();
        } else {
            set_point = cooling_set_point;
            max_power = hvac->max_cooling_power();
        }
        const double ten_watt_per_square_meter_power = 10 * traits->conditioned_floor_area();
        const double unrestricted_power = ten_watt_per_square_meter_power * (set_point - next_temperature_no_power) /
                (next_temperature(ten_watt_per_square_meter_power) - next_temperature_no_power);
        double power;
        std::abs(unrestricted_power) <= std::abs(max_power) ? power = unrestricted_power : power = max_power;
        return power;
    }
}

void UpdateBuildingTemperature(worker::Connection &connection,
                               const worker::EntityId &entity_id,
                               worker::Entity &entity,
                               double next_temperature) {
    Temperature::Update update;
    update.set_current_temperature(next_temperature);
    connection.SendComponentUpdate<Temperature>(entity_id, update);
}

int main(int argc, char **argv) {
    if (argc != 4) {
        std::cout << "Usage: " << argv[0] << " <hostname> <port> <worker id>" << std::endl;
        std::cout << "Connects to the Simulation and satisfies the "
                "ThermalEngineConstraint." << std::endl;
        std::cout << "For example: " << argv[0] << " localhost 7777 Thim0" << std::endl;
        return 1;
    }

    const std::string hostname = argv[1];
    const std::uint16_t port = static_cast<std::uint16_t>(std::stoul(argv[2]));
    const std::string worker_id = argv[3];

    worker::ConnectionParameters params;
    params.WorkerType = worker_type;
    params.WorkerId = worker_id;
    params.Network.UseExternalIp = true;
    params.Network.Tcp.MultiplexLevel = 4;
    worker::Connection connection{hostname, port, params};

    worker::View view;

    auto time = std::chrono::high_resolution_clock::now();
    while (true) {
        auto op_list = connection.GetOpList(0 /* non-blocking */);

        // Invoke user-provided callbacks.
        view.Process(op_list);

        auto &entities = view.Entities;
        for (auto &pair : entities) {
            const auto next_temperature = CalculateNextBuildingTemperature(pair.second);
            UpdateBuildingTemperature(connection, pair.first, pair.second, next_temperature);
        }
        time += std::chrono::microseconds(seconds_per_update * 1000000);
        std::this_thread::sleep_until(time);
    }
}
