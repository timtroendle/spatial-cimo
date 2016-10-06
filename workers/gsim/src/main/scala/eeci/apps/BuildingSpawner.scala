package eeci.apps

import eeci.natures.Building
import improbable.logging.Logger
import improbable.papi.world.AppWorld
import improbable.papi.worldapp.WorldApp

class TestBuildingSpawner(appWorld: AppWorld, logger: Logger) extends WorldApp {

  appWorld.entities.spawnEntity(Building(
    heatMassCapacity = 165000 * 100,
    heatTransmission = 200,
    conditionedFloorArea = 100,
    maxCoolingPower = 10000,
    maxHeatingPower = 10000,
    initialTemperature = 22
  ))

}