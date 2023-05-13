#ifndef MQTT_CON_H
#define MQTT_CON_H

#include <PubSubClient.h>
#include "config/constants.h"

class MqttManager {
  private:
    String clientId;
    String inputTopic;
    String outputTopic;
  public:
    MqttManager(String clientId, String inputTopic, String outputTopic);
    void connect(PubSubClient &client, Stream &uart);
};

MqttManager::MqttManager(String clientId, String inputTopic, String outputTopic) {
  this->clientId = clientId; 
  this->inputTopic = inputTopic;
  this->outputTopic = outputTopic;
}

void MqttManager::connect(PubSubClient &client, Stream &uart) {
  while (!client.connected()) {
    uart.print("Attempting MQTT connection...");
    if (client.connect(this->clientId.c_str())) {
      uart.println("connected");
    } else {
      uart.print("failed, rc=");
      uart.print(client.state());
      uart.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

#endif