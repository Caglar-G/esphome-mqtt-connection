#pragma once

#include "esphome/core/component.h"
#include "esphome/core/defines.h"
#include "esphome/core/helpers.h"
#include "esphome/core/preferences.h"

#include "esphome/components/globals/globals_component.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/gpio/switch/gpio_switch.h"
#include "esphome/components/mqtt/mqtt_switch.h"

#include "esp_timer.h"

#ifdef USE_BINARY_SENSOR
#include "esphome/components/binary_sensor/binary_sensor.h"
#endif

#ifdef USE_OUTPUT
#include "esphome/components/output/binary_output.h"
#endif

#include <vector>

#ifdef USE_ESP32


namespace esphome {
namespace custom_mqtt_connection {
    class CustomMQTTConnection : public Component {
        public:
        CustomMQTTConnection(); 
        globals::RestoringGlobalStringComponent<std::string, 64> *global_forced_addr;
        void set_global_addr(globals::RestoringGlobalStringComponent<std::string, 64> *ga_in) {
            global_forced_addr = ga_in;
        }
        //BrokerId
        globals::RestoringGlobalStringComponent<std::string, 64> *brokerAddress;
        void set_brokerAddress(globals::RestoringGlobalStringComponent<std::string, 64> *ga_in) {
            brokerAddress = ga_in;
        }
        globals::RestoringGlobalStringComponent<std::string, 64> *brokerUserName;
        void set_brokerUserName(globals::RestoringGlobalStringComponent<std::string, 64> *ga_in) {
            brokerUserName = ga_in;
        }
        globals::RestoringGlobalStringComponent<std::string, 64> *brokerPassword;
        void set_brokerPassword(globals::RestoringGlobalStringComponent<std::string, 64> *ga_in) {
            brokerPassword = ga_in;
        }
        //
        void loop() override;
        void setup() override;

        void set_test(esphome::gpio::GPIOSwitch *test_switch) { 
            ESP_LOGD("early", "setTest");
            test_switch_ = test_switch; 
        }

        protected:
        globals::GlobalsComponent<std::string>  *deviceId_;
        esphome::gpio::GPIOSwitch *test_switch_{nullptr};

        mqtt::MQTTSwitchComponent  *testmqtt;
        esp_timer_handle_t timer_handle;
    };
}}

#endif