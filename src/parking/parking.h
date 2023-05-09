#ifndef PARKING_H
#define PARKING_H

#include <Arduino.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "sensors/sensor.h"
#include "config/constants.h"

class Parking {
    private:
        uint16_t id;
        uint16_t size;
    public:
        Parking(uint16_t id, uint16_t size);
        uint16_t getID();
        uint16_t getSize();
        void loop(SensorABC &sensor, PubSubClient &mqtt);
};

Parking::Parking(uint16_t id, uint16_t size) {
    this->id = id;
    this->size = size;
}

uint16_t Parking::getID() {
    return this->id;
}

uint16_t Parking::getSize() {
    return this->size;
}

void Parking::loop(SensorABC &sensor, PubSubClient &mqtt) {
    if(sensor.hasChanged()){
        bool value = sensor.getValue();
        DynamicJsonDocument  output(20);
        output["sensor"] = sensor.getValue();
        String output_str;
        serializeJson(output, output_str);
        // TODO: Remove the dependendecy to Constats
        mqtt.publish(MQTT_OUT_TOPIC, output_str.c_str());
    }
}

#endif