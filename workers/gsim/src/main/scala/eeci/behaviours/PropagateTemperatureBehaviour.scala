package eeci.behaviours

import improbable.papi.entity.{Entity, EntityBehaviour}
import improbable.papi.world.World
import eeci.building.{Temperature, TemperatureWriter}
import eeci.util.ThermalEngineConstraint

class PropagateTemperatureBehaviour(temperature: TemperatureWriter, world: World, entity: Entity) extends EntityBehaviour {

  override def onReady(): Unit = {
    entity.addEngineConstraint(ThermalEngineConstraint)
    entity.delegateState[Temperature](ThermalEngineConstraint)
  }

}