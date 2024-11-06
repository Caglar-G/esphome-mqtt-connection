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


    void timer_callback(void *arg) {
    ESP_LOGD("core", "5 saniye geçti, işlem yapılıyor.");
    // Burada işleminizi gerçekleştirebilirsiniz. Örnek: MQTT mesajı gönderme
    // mqtt_client->publish("topic/test", "Hello World", 0, false);
    }

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



        esp_timer_create_args_t timer_args = {
            .callback = timer_callback,  // Callback fonksiyonu
            .name = "periodic_timer"
        };

        // Timer'ı oluşturuyoruz
        esp_timer_create(&timer_args, &timer_handle);

        // Timer'ı her 5 saniyede bir çalışacak şekilde başlatıyoruz
        esp_timer_start_periodic(timer_handle, 5000000);

    }
    void CustomMQTTConnection::loop() {

    }
    void CustomMQTTConnection::cleanup() {
        esp_timer_stop(timer_handle);  // Timer'ı durdur
        ESP_LOGD(TAG, "Timer durduruldu.");
    }

   
}}