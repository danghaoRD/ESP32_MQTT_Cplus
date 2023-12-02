#include <wifi.hpp>
// #include "esp_log.h"
#include "mqtt.hpp"
// #include "esp_event.h"
// #include "esp_netif.h"
// #include "protocol_examples_common.h"



#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"

#include "lwip/sockets.h"
#include "lwip/dns.h"
#include "lwip/netdb.h"

#include "esp_log.h"
#include "mqtt_client.h"

// extern  Mqtt MqttThingsBoard;
static int retry_cnt = 0;
//static Mqtt *MqttThingsBoard = Mqtt::getInstance("demo.thingsboard.io",1883, "Kj9nO6xI3Stzdn27st8L", "Kj9nO6xI3Stzdn27st8L");

static const char *TAG = "Wifi";

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                                   int32_t event_id, void *event_data)
{
    static Mqtt *MqttThingsBoard = Mqtt::getInstance("demo.thingsboard.io",1883, "Kj9nO6xI3Stzdn27st8L", "Kj9nO6xI3Stzdn27st8L");

    switch (event_id)
    {
    case WIFI_EVENT_STA_START:
        esp_wifi_connect();  
        ESP_LOGI(TAG, "Trying to connect with Wi-Fi\n");
        break;

    case WIFI_EVENT_STA_CONNECTED:
        ESP_LOGI(TAG, "Wi-Fi connected\n");
        break;
    case IP_EVENT_STA_GOT_IP:
    {

        ESP_LOGI(TAG, "got ip: startibg MQTT Client\n");
       MqttThingsBoard->app_start();
       // mqtt_app_start();
      //  Mqtt Mqtt_things("demo.thingsboard.io",1883, "Kj9nO6xI3Stzdn27st8L", "Kj9nO6xI3Stzdn27st8L");
    }
        break;

    case WIFI_EVENT_STA_DISCONNECTED:
    {

        ESP_LOGI(TAG, "disconnected: Retrying Wi-Fi\n");
        if (retry_cnt++ < MAX_RETRY)
        {
            esp_wifi_connect();
        }
        else
        ESP_LOGI(TAG, "Max Retry Failed: Wi-Fi Connection\n");
    }
        break;

    default:
        break;
    }
    //return ESP_OK;
}

 Wifi::Wifi(const char ssid[32], const char pass[32])
{

  
    // esp_event_loop_create_default();
    esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL); // RD_Note
    esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL);

    wifi_config_t wifi_config = { }; 
    esp_err_t ret = esp_wifi_get_config(WIFI_IF_STA, &wifi_config);

    #if 0
        const char ssid_cfg[] = EXAMPLE_ESP_WIFI_SSID;
        const char pass_cfg[] = EXAMPLE_ESP_WIFI_PASS;


        for (int i = 0; i < ( sizeof(ssid_cfg)/sizeof(ssid_cfg[0]) ); ++i) {
            wifi_config.sta.ssid[i] = ssid_cfg[i];
        }

            for (int i = 0; i < ( sizeof(pass_cfg)/sizeof(pass_cfg[0]) ); ++i) {
            wifi_config.sta.password[i] = pass_cfg[i];
        }
    #else
        for (int i = 0; i < 32; i++) {
            wifi_config.sta.ssid[i] = ssid[i];
            this->ssid[i] = ssid[i];
        }

        for (int i = 0; i < 32; i++) {
            wifi_config.sta.password[i] = pass[i];
            this->pass[i] = pass[i];
        }
    #endif

    wifi_config.sta.threshold.authmode = WIFI_AUTH_WPA2_PSK;
    wifi_config.sta.pmf_cfg.required = false;
    esp_netif_init();
    esp_netif_create_default_wifi_sta();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_wifi_set_mode(WIFI_MODE_STA);
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    esp_wifi_start();
    ESP_LOGI(TAG, "SSID: %s", wifi_config.sta.ssid);
    ESP_LOGI(TAG, "PASS: %s", wifi_config.sta.password);
}