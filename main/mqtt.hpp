#pragma once
#include <cstring>
#include <iostream>
#include "esp_event.h"
#include "mqtt_client.h"

using namespace std;

class Mqtt
{
 private:
    string host;
    int port;
    string clientId;
    string username;
    string password;
 public:
 esp_mqtt_client_handle_t client = {};
  Mqtt(string host, int port, string username, string password);
  void app_start(void);
};

  void Publisher_Task(void *params);
extern  Mqtt MqttThingsBoard;