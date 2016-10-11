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
static const unsigned fast_forward_factor = 400;
static const double heating_set_point = 21;
static const double cooling_set_point = 26;
static const double outside_temperature = 30;

double CalculateNextTemperature(const worker::Entity& entity, double outside_temperature,
                                double time_step_length_seconds, double hvac_power) {
    const double current_temperature = entity.Get<Temperature>()->current_temperature();
    const auto traits = entity.Get<eeci::building::Traits>();
    const double heat_transmission = traits->heat_transmission();
    const double heat_mass_capacity = traits->heat_mass_capacity();
    const double dt_by_cm = time_step_length_seconds / heat_mass_capacity;
    return (current_temperature * (1 - dt_by_cm * heat_transmission) +
            dt_by_cm * (hvac_power + heat_transmission * outside_temperature));
}

double CalculateNextHvacPower(const worker::Entity& entity, double outside_temperature, double time_step_length_seconds) {
    const auto next_temperature_no_power = CalculateNextTemperature(entity, outside_temperature,
                                                                    time_step_length_seconds, 0);
    if (next_temperature_no_power >= heating_set_point && next_temperature_no_power <= cooling_set_point){
        return 0;
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
        const double ten_watt_per_square_meter_power = 10 * entity.Get<eeci::building::Traits>()->conditioned_floor_area();
        const auto next_temperature_10 = CalculateNextTemperature(entity, outside_temperature,
                                                                  time_step_length_seconds, ten_watt_per_square_meter_power);
        const double unrestricted_power = ten_watt_per_square_meter_power * (set_point - next_temperature_no_power) /
                (next_temperature_10 - next_temperature_no_power);
        double power;
        std::abs(unrestricted_power) <= std::abs(max_power) ? power = unrestricted_power : power = max_power;
        return power;
    }
}

void UpdateBuilding(worker::Connection &connection, const worker::EntityId &entity_id, worker::Entity &entity) {
    const double next_power = CalculateNextHvacPower(entity, outside_temperature,
                                                     seconds_per_update * fast_forward_factor);
    const double next_temperature = CalculateNextTemperature(entity, outside_temperature,
                                                             seconds_per_update * fast_forward_factor, next_power);
    eeci::building::Hvac::Update power_update;
    power_update.set_current_power(next_power);
    Temperature::Update temperature_update;
    temperature_update.set_current_temperature(next_temperature);
    connection.SendComponentUpdate<Temperature>(entity_id, temperature_update);
    connection.SendComponentUpdate<eeci::building::Hvac>(entity_id, power_update);
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
            UpdateBuilding(connection, pair.first, pair.second);
        }
        time += std::chrono::microseconds(seconds_per_update * 1000000);
        std::this_thread::sleep_until(time);
    }
}
