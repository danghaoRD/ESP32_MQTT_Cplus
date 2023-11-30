#include "mqtt.hpp"
// #include "esp_log.h"
// #include "esp_system.h"
// #include "esp_event.h"
// #include "esp_netif.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/semphr.h"
// #include "freertos/queue.h"


// #include "lwip/sockets.h"
// #include "lwip/dns.h"
// #include "lwip/netdb.h"


#include "esp_wifi.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "protocol_examples_common.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "freertos/queue.h"


#include "esp_log.h"
#include "mqtt_client.h"
// #include "esp_event.h"
// #include "mqtt_client.h"
static const char *TAG = "MQTT_EXAMPLE";
uint32_t mqtt_connected_flag = 0;
 Mqtt MqttThingsBoard("demo.thingsboard.io",1883, "Kj9nO6xI3Stzdn27st8L", "Kj9nO6xI3Stzdn27st8L");
 //Mqtt MqttThingsBoard;
//MqttThingsBoard = new Mqtt("demo.thingsboard.io",1883, "Kj9nO6xI3Stzdn27st8L", "Kj9nO6xI3Stzdn27st8L");
static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG, "Last error %s: 0x%x", message, error_code);
    }
}


/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
    esp_mqtt_event_handle_t event = (esp_mqtt_event_handle_t) event_data;
    esp_mqtt_client_handle_t client = event->client;
   // client = event->client;
    int msg_id;
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
    {
            ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
            //char json[100] = "{\"temp\": 21}";
            mqtt_connected_flag =1;
                    char json[100] ={0};
            sprintf(json, "{\"temp\": %d}", 11);
            msg_id = esp_mqtt_client_publish(client, "v1/devices/me/telemetry", json, 0, 1, 0);
            ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

            // msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
            // ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

        break;
    }
    case MQTT_EVENT_DISCONNECTED:
    {
        ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
        break;
    }
    case MQTT_EVENT_SUBSCRIBED:
    {
        // ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        // msg_id = esp_mqtt_client_publish(client, "/topic/qos0", "data", 0, 0, 0);
        // ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);
        break;
    }
    case MQTT_EVENT_UNSUBSCRIBED:
    {
        ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    }
    case MQTT_EVENT_PUBLISHED:
    {
        ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    }
    case MQTT_EVENT_DATA:
    {
        ESP_LOGI(TAG, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        break;
    }
    case MQTT_EVENT_ERROR:
    {
        ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    }
    default:{
        ESP_LOGI(TAG, "Other event id:%d", event->event_id);
        break;
    }
    }
}
 
Mqtt::Mqtt(string host, int port, string username, string password)
 {
    this->host = host;
    this->port = port;
    this->username = username;
    this->password = password;
    // esp_mqtt_client_config_t mqtt_cfg = { };
    // mqtt_cfg.host = "demo.thingsboard.io";
    // // device ID 5646f720-746e-11ee-bc19-a533898c4c2a
    // mqtt_cfg.username = "Kj9nO6xI3Stzdn27st8L";
    // mqtt_cfg.port     = 1883;

    // client = esp_mqtt_client_init(&mqtt_cfg);
    // /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    // esp_mqtt_client_register_event(client,(esp_mqtt_event_id_t) ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    // esp_mqtt_client_start(client);
 }

 void Mqtt::app_start(void)
 {
    esp_mqtt_client_config_t mqtt_cfg = { };
    mqtt_cfg.host = this->host.c_str();
    // device ID 5646f720-746e-11ee-bc19-a533898c4c2a
    mqtt_cfg.username = this->username.c_str();
    mqtt_cfg.port     = this->port;

    this->client = esp_mqtt_client_init(&mqtt_cfg);
    /* The last argument may be used to pass data to the event handler, in this example mqtt_event_handler */
    esp_mqtt_client_register_event(this->client,(esp_mqtt_event_id_t) ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    esp_mqtt_client_start(this->client);
 }

void Publisher_Task(void *params)
{
  while (true)
  {
     ESP_LOGI(TAG, "send mqtt out\n");
     static int temp =0;
    if(mqtt_connected_flag)
    {
        temp++;
        //char json[100] = "{\"temp\": 20}";
        char json[100] ={0};
        ESP_LOGI(TAG, "send mqtt\n");
        sprintf(json, "{\"temp\": %d}", temp);
        if(temp > 50) temp =0;
        esp_mqtt_client_publish(MqttThingsBoard.client, "v1/devices/me/telemetry", json , 0, 1, 0);
    }
    vTaskDelay(15000 / portTICK_PERIOD_MS);
  }
}