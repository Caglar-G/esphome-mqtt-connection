#include "custom_mqtt_connection.h"


#include "esphome/core/application.h"
#include "esphome/core/log.h"
#include "esphome/components/globals/globals_component.h"
#include "esphome/core/helpers.h"

#include "esphome/core/defines.h"
#include "esphome/core/component.h"
#include "esphome/components/mqtt/mqtt_client.h"


#include "esphome.h"

// Include global defines
#include "esphome/core/defines.h"


namespace esphome {
namespace custom_mqtt_connection {

    static const char *TAG = "custom_component";

    void CustomMQTTConnection::setup() {
        ESP_LOGD(TAG, "Setting up CustomComponent...");

    }
    void CustomMQTTConnection::loop() {

    }
}}