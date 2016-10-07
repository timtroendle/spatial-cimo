#include <improbable/worker.h>
#include <cstdint>
#include <iostream>
#include <string>
#include <thread>
#include <eeci/building/temperature.h>

using eeci::building::Temperature;

static const std::string kWorkerType = "Thim";
static const std::string kLoggerName = "thim.cc";

void UpdateBuildingTemperature(worker::Connection& connection,
                               const worker::EntityId& entity_id,
                               worker::Entity& entity) {
  const auto building_temperature = entity.Get<Temperature>()->current_temperature();
  Temperature::Update update;
  update.set_current_temperature(building_temperature + 1);
  connection.SendComponentUpdate<Temperature>(entity_id, update);
}

int main(int argc, char** argv) {
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
  params.WorkerType = kWorkerType;
  params.WorkerId = worker_id;
  params.Network.UseExternalIp = true;
  params.Network.Tcp.MultiplexLevel = 4;
  worker::Connection connection{hostname, port, params};

  worker::View view;

  static const unsigned kSecondsPerUpdate = 5;
  auto time = std::chrono::high_resolution_clock::now();
  while (true) {
    auto op_list = connection.GetOpList(0 /* non-blocking */);

    // Invoke user-provided callbacks.
    view.Process(op_list);

    auto& entities = view.Entities;
    for (auto& pair : entities) {
      UpdateBuildingTemperature(connection, pair.first, pair.second);
    }
    time += std::chrono::microseconds(kSecondsPerUpdate * 1000000);
    std::this_thread::sleep_until(time);
  }
}
