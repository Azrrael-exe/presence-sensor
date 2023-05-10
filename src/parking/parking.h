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
        uint16_t id;
        uint16_t size;
    public:
        Parking(uint16_t id, uint16_t size);
        uint16_t getID();
        uint16_t getSize();
        void loop(SensorABC &sensor, PubSubClient &mqtt, String &input, bool &queue, DisplayABC &display );
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

void Parking::loop(SensorABC &sensor, PubSubClient &mqtt, String &input, bool &queue, DisplayABC &display ) {
    if(sensor.hasChanged()){
        bool value = sensor.getValue();
        DynamicJsonDocument  output(20);
        output["sensor"] = sensor.getValue();
        String output_str;
        serializeJson(output, output_str);
        // TODO: Remove the dependendecy to Constats
        mqtt.publish(MQTT_OUT_TOPIC, output_str.c_str());
    }
    if (queue) {
        DynamicJsonDocument mem_allocation(200);
        DeserializationError error = deserializeJson(mem_allocation, input);
        float occupancy = mem_allocation["occupancy"];
        if (error) {
            Serial.print(F("deserializeJson() failed: "));
            Serial.println(error.f_str());
        }
        Serial.println(occupancy);
        display.setOccupancy(occupancy);
        queue = false;
    }
}

#endif