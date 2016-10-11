package eeci.apps

import java.io.InputStream

import eeci.natures.Building
import improbable.logging.Logger
import improbable.math.Vector3d
import improbable.papi.world.AppWorld
import improbable.papi.worldapp.WorldApp

class TestBuildingSpawner(appWorld: AppWorld, logger: Logger) extends WorldApp {

  val stream : InputStream = getClass.getResourceAsStream("/test_building_selection.csv")
  val src = scala.io.Source.fromInputStream( stream )
  val iter = src.getLines().drop(1).map(_.split(","))
  iter.foreach(coords =>
    appWorld.entities.spawnEntity(Building(
      Vector3d(coords(1).toDouble, 0.0, coords(2).toDouble),
      heatMassCapacity = 165000 * 100,
      heatTransmission = 200,
      conditionedFloorArea = 100,
      maxCoolingPower = 10000,
      maxHeatingPower = 10000,
      initialTemperature = (Math.random().toFloat * 2 - 1) * 4 + 22
    )))
}
