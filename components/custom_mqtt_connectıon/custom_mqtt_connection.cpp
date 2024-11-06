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


        this->test_switch_->add_on_state_callback([this](bool state) {
            // Switch durumu değiştiğinde bu kod bloğu çalışır
            if (state) {
                ESP_LOGD("custom_switch", "Switch ON-TTTTT");
                const char *str = "ON";
                mqtt::global_mqtt_client->publish("devices/"+id(global_forced_addr)+"/0x0006/0/attributes/0x0000", str,strlen(str), 1, true);
            } else {
                ESP_LOGD("custom_switch", "Switch OFF-TTTT");
                const char *str = "OFF";
                mqtt::global_mqtt_client->publish("devices/"+id(global_forced_addr)+"/0x0006/0/attributes/0x0000", str,strlen(str), 1, true);
            }
        });
        //devices/e89a85eb-452e-4111-9494-619d9ddea73a/0x0006/0/attributes/0x0000
        mqtt::global_mqtt_client->subscribe("devices/"+id(global_forced_addr)+"/0x0006/0/commands",
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