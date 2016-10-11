package eeci.behaviours

import improbable.papi.entity.{Entity, EntityBehaviour}
import improbable.papi.world.World
import eeci.building.{Hvac, HvacWriter}
import eeci.util.ThermalEngineConstraint

class PropagateHvacBehaviour(hvac: HvacWriter, world: World, entity: Entity) extends EntityBehaviour {

  override def onReady(): Unit = {
    entity.addEngineConstraint(ThermalEngineConstraint)
    entity.delegateState[Hvac](ThermalEngineConstraint)
  }

}
