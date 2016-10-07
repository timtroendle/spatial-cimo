package eeci.util

import improbable.fapi.engine.ProtoEngineConstraintSatisfier
import improbable.papi.engine.{EngineConstraints, ProtoEngineConstraint}

case object ThermalEngineConstraintSatisfier extends ProtoEngineConstraintSatisfier(
  EngineConstraints.makeClaim(EngineConstraints.makeAtom("Temperature"))
)

case object ThermalEngineConstraint extends ProtoEngineConstraint(EngineConstraints.makePredicate("Temperature"))