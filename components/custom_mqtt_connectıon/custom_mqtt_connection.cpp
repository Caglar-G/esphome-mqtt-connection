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
    CustomMQTTConnection::CustomMQTTConnection() { }

    void CustomMQTTConnection::setup() {
        ESP_LOGD(TAG, "Setting up CustomComponent...");
        mqtt::global_mqtt_client->subscribe("test",
        [this](const std::string &topic, const std::string &payload) {
            ESP_LOGW(TAG, "Can't convert '%s' to number!", payload.c_str());
            
            if(payload == "ON"){
            this->test_switch_->turn_on();
            }else if(payload == "OFF"){
            this->test_switch_->turn_off();
            }else if(payload == "Toggle"){
            this->test_switch_->toggle();
            }
            /*
            auto val = parse_number<float>(payload);
            if (!val.has_value()) {
            ESP_LOGW(TAG, "Can't convert '%s' to number!", payload.c_str());
            this->publish_state(NAN);
            return;
            }*/

            //this->publish_state(*val);
        },0);

    }
    void CustomMQTTConnection::loop() {

    }
}}