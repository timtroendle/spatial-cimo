package eeci.natures

import eeci.building.{Hvac, Temperature, Traits}
import eeci.behaviours.PropagateTemperatureBehaviour
import improbable.corelib.natures.{BaseNature, NatureApplication, NatureDescription}
import improbable.corelibrary.transforms.TransformNature
import improbable.papi.entity.EntityPrefab
import improbable.papi.entity.behaviour.EntityBehaviourDescriptor

object Building extends NatureDescription {

  override val dependencies = Set[NatureDescription](BaseNature, TransformNature)

  override def activeBehaviours: Set[EntityBehaviourDescriptor] = Set(
    descriptorOf[PropagateTemperatureBehaviour]
  )

  def apply(heatMassCapacity: Float, heatTransmission: Float, conditionedFloorArea: Float,
            maxCoolingPower: Float, maxHeatingPower: Float,
            initialTemperature: Float): NatureApplication = {
    application(
      states = Seq(
        Traits(heatMassCapacity = heatMassCapacity,
          heatTransmission = heatTransmission,
          conditionedFloorArea = conditionedFloorArea),
        Hvac(maxCoolingPower = maxCoolingPower, maxHeatingPower = maxHeatingPower),
        Temperature(initialTemperature)
      ),
      natures = Seq(
        BaseNature(entityPrefab = EntityPrefab("Building"), isPhysical = false, isVisual = false),
        TransformNature()
      )
    )
  }
}