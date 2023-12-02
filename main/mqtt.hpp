#pragma once
#include <cstring>
#include <iostream>
#include "esp_event.h"
#include "mqtt_client.h"

using namespace std;

class Mqtt
{
 public:
  esp_mqtt_client_handle_t client = {};
  void app_start(void);
  void getName(void);
  static Mqtt* getInstance(std::string host, int port, std::string username, std::string password);
 private:
   static Mqtt* mInstancePtr;
   static int* val;
    string host;
    int port;
    string clientId;
    string username;
    string password;
    Mqtt(string host, int port, string username, string password);
};

  void Publisher_Task(void *params);
//extern  Mqtt MqttThingsBoard;