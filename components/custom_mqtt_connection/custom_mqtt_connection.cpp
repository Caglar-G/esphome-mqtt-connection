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
#include "esp_timer.h"



namespace esphome {
namespace custom_mqtt_connection {

    static const char *TAG = "custom_component";

    CustomMQTTConnection::CustomMQTTConnection() { }

    void CustomMQTTConnection::setup() {
        ESP_LOGD(TAG, "Setting up CustomComponent...");
        mqtt::global_mqtt_client->set_username(id(brokerUserName));
        mqtt::global_mqtt_client->set_password(id(brokerPassword));

        mqtt::global_mqtt_client->set_on_disconnect([this](mqtt::MQTTClientDisconnectReason reason) {
            const char* reason_text = "";
            switch (reason) {
                case mqtt::MQTTClientDisconnectReason::TCP_DISCONNECTED:
                    reason_text = "TCP Disconnected";
                    break;
                case mqtt::MQTTClientDisconnectReason::MQTT_UNACCEPTABLE_PROTOCOL_VERSION:
                    reason_text = "Unacceptable Protocol Version";
                    break;
                case mqtt::MQTTClientDisconnectReason::MQTT_IDENTIFIER_REJECTED:
                    reason_text = "Identifier Rejected";
                    break;
                case mqtt::MQTTClientDisconnectReason::MQTT_SERVER_UNAVAILABLE:
                    reason_text = "Server Unavailable";
                    break;
                case mqtt::MQTTClientDisconnectReason::MQTT_MALFORMED_CREDENTIALS:
                    reason_text = "Malformed Credentials";
                    break;
                case mqtt::MQTTClientDisconnectReason::MQTT_NOT_AUTHORIZED:
                    reason_text = "Not Authorized";
                    break;
                case mqtt::MQTTClientDisconnectReason::ESP8266_NOT_ENOUGH_SPACE:
                    reason_text = "ESP8266 Not Enough Space";
                    break;
                case mqtt::MQTTClientDisconnectReason::TLS_BAD_FINGERPRINT:
                    reason_text = "TLS Bad Fingerprint";
                    break;
                default:
                    reason_text = "Unknown Reason";
                    break;
            }

            ESP_LOGW("custom_mqtt_connection", "Disconnect reason: %s", reason_text);    
        });


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



       const esp_timer_create_args_t periodic_timer_args = {
            .callback = [](void *arg) {
                // 'arg' parametresi ile 'this' işaretçisini alıyoruz
                CustomMQTTConnection *component = static_cast<CustomMQTTConnection*>(arg);
                // Şimdi sınıfın metoduna erişebiliriz
                //ESP_LOGD("custom_switch", "Timer");
                const char *str = "alive";
                mqtt::global_mqtt_client->publish("devices/"+id(component->global_forced_addr)+"/heartbeat", str,strlen(str), 0, false);
  
            },            
            .arg             = this,
            .dispatch_method = ESP_TIMER_TASK,
            .name            = "lv_tick_action",
            .skip_unhandled_events = false,
        };

        // Timer'ı oluşturuyoruz
        esp_timer_create(&periodic_timer_args, &timer_handle);

        // Timer'ı her 5 saniyede bir çalışacak şekilde başlatıyoruz
        esp_timer_start_periodic(timer_handle, 5000000);

    }
    void CustomMQTTConnection::loop() {

    }
    /*
    void CustomMQTTConnection::cleanup() {
        esp_timer_stop(timer_handle);  // Timer'ı durdur
        ESP_LOGD(TAG, "Timer durduruldu.");
    }*/

   
}}