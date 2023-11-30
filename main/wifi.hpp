#pragma once 

#include "esp_wifi.h"
// #include "esp_system.h"
// #include "nvs_flash.h"
// #include "esp_event.h"
// #include "esp_netif.h"
#define  EXAMPLE_ESP_WIFI_SSID "Haoshoku Haki" //TTS"
#define  EXAMPLE_ESP_WIFI_PASS  "123456789"//  "TTS123456"
#define MAX_RETRY 10
class Wifi
{
private:
    char ssid[32] = EXAMPLE_ESP_WIFI_SSID;
    char pass[32] = EXAMPLE_ESP_WIFI_PASS;
    wifi_auth_mode_t authMode = WIFI_AUTH_WPA2_PSK;
public:
    Wifi(const char ssid[32], const char pass[32]);
   // ~wifi();
};



// wifi::~wifi()
// {
// }
