#ifndef PARKING_H
#define PARKING_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "sensors/sensor.h"
#include "actuators/display.h"
#include "config/constants.h"

class Parking {
    private:
        String clientId;
        int capacity;
        String inputTopic;
        String outputTopic;
        String registerInputtopic;
        String registerOutputtopic;
        bool connection_status = false;
    public:
        Parking(String clientId, int capacity, String inputTopic, String outputTopic, String registerInputtopic, String registerOutputtopic);
        void setupRegistration(PubSubClient &mqtt, String &input, bool &queue);
        void loop(SensorABC &sensor, PubSubClient &mqtt, String &input, bool &queue, DisplayABC &display);
};

Parking::Parking(String clientId, int capacity, String inputTopic, String outputTopic, String registerInputtopic, String registerOutputtopic){
    this->clientId = clientId; 
    this->capacity = capacity;
    this->inputTopic = inputTopic;
    this->outputTopic = outputTopic;
    this->registerInputtopic = registerInputtopic;
    this->registerOutputtopic = registerOutputtopic;
}

void Parking::setupRegistration(PubSubClient &mqtt, String &input, bool &queue){
    DynamicJsonDocument  output(200);
    output["id"] = this->clientId;
    output["capacity"] = this->capacity;
    String output_str;
    serializeJson(output, output_str);
    Serial.println(output_str);
    mqtt.publish(this->registerOutputtopic.c_str(), output_str.c_str());
    mqtt.subscribe(this->registerInputtopic.c_str());
}

void Parking::loop(SensorABC &sensor, PubSubClient &mqtt, String &input, bool &queue, DisplayABC &display) {
    if(sensor.hasChanged()){
        bool value = sensor.getValue();
        DynamicJsonDocument  output(20);
        output["sensor"] = sensor.getValue();
        String output_str;
        serializeJson(output, output_str);
        mqtt.publish(this->outputTopic.c_str(), output_str.c_str());
    }
  	else if (queue) {
      DynamicJsonDocument response(200); 
      DeserializationError error = deserializeJson(response, input);
      if (error) {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
      }
      if (response.containsKey("registered")) {
        mqtt.subscribe(this->inputTopic.c_str());
      }
      if (response.containsKey("occupancy")) {
        float occupancy = response["occupancy"];
        display.setOccupancy(occupancy);
      }
      queue = false;
    }
}

#endif