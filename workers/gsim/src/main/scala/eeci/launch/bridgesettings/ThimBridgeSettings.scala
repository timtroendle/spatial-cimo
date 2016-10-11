package improbable.launch.bridgesettings

import eeci.util.ThermalEngineConstraintSatisfier
import improbable.fapi.bridge._
import improbable.fapi.network.MultiplexTcpLinkSettings
import improbable.serialization.KryoSerializable

object ThimBridgeSettings extends BridgeSettingsResolver {
  val THIM_TYPE = "Thim"

  private val CLIENT_ENGINE_BRIDGE_SETTINGS = BridgeSettings(
    ThimAssetContextDiscriminator,
    MultiplexTcpLinkSettings(4),
    THIM_TYPE,
    ThermalEngineConstraintSatisfier,
    RadialInterestPolicy,
    MetricsEngineLoadPolicy,
    PerEntityOrderedStateUpdateQos
  )

  object ThimAssetContextDiscriminator extends AssetContextDiscriminator{
    override def assetContextForEntity(entity: EngineEntity): String = {
      ""
    }
  }

  object RadialInterestPolicy extends EntityInterestPolicy with KryoSerializable {
    val RADIUS = 2
    override def interestTypeFor(entity: EngineEntity): Option[InterestType] = {
      Some(RadialInterest(RADIUS))
    }
  }

  private val bridgeSettings = Map[String, BridgeSettings](
    THIM_TYPE -> CLIENT_ENGINE_BRIDGE_SETTINGS
  )

  override def engineTypeToBridgeSettings(engineType: String, metadata: String): Option[BridgeSettings] = {
    bridgeSettings.get(engineType)
  }
}
