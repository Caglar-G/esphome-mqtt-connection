import esphome.codegen as cg
from esphome.components import binary_sensor, esp32_ble_server, output, switch, gpio
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@jesserockz"]
DEPENDENCIES = ["wifi", "esp32"]
CONF_TEST = "test"

custom_ns = cg.esphome_ns.namespace("custom_mqtt_connection")
CustomComponent  = custom_ns.class_(
    "CustomMQTTConnection", cg.Component
)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(CustomComponent),
        cv.Optional("global_addr"): cv.use_id(globals),
        cv.Required("brokerAddress"): cv.use_id(globals),
        cv.Required("brokerUserName"): cv.use_id(globals),
        cv.Required("brokerPassword"): cv.use_id(globals),
        cv.Required(CONF_TEST): cv.use_id(switch.Switch),

    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    ga = await cg.get_variable(config["global_addr"])
    cg.add(var.set_global_addr(ga))

    ga = await cg.get_variable(config["brokerAddress"])
    cg.add(var.set_brokerAddress(ga))

    ga = await cg.get_variable(config["brokerUserName"])
    cg.add(var.set_brokerUserName(ga))

    ga = await cg.get_variable(config["brokerPassword"])
    cg.add(var.set_brokerPassword(ga))

    test_switch = await cg.get_variable(config[CONF_TEST])
    cg.add(var.set_test(test_switch))