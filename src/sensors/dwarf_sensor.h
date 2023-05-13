#ifndef DWARF_SENSOR_H
#define DWARF_SENSOR_H

#include <Arduino.h>
#include "sensors/sensor.h"

class DwarfSensor:public SensorABC {
    private:
        int inPin;
        int outPin;
        int read();
        long timer;
        long threshold;
        int value;
        bool changed;
    public:
        bool hasChanged();
        void config();
        void loop();
        bool getValue();
        DwarfSensor(int inPin, int outPin, long threshold);   
};

DwarfSensor::DwarfSensor(int inPin, int outPin, long threshold) {
    this->inPin = inPin;
    this->outPin = outPin;
    this->timer = 0;
    this->threshold = threshold;
}

int DwarfSensor::read() {
    if (digitalRead(this->inPin)){
        return 1;
    } else if (digitalRead(this->outPin)) {
        return -1;
    } else {
        return 0;
    }  
}

void DwarfSensor::loop() {
    int actual_value = this->read();
    if (actual_value != 0) {
        if ((this->value != actual_value) || (millis() - this->timer >= this->threshold)) {
            this->timer = millis();
            this->changed = true;
            this->value = actual_value;
            Serial.println(this->hasChanged());
        }
    }
}

bool DwarfSensor::getValue() {
    this->changed = false;
    if(this->value == 1) {
        return true;
    }
    return false;
}

void DwarfSensor::config() {
    pinMode(this->inPin, INPUT);
    pinMode(this->outPin, INPUT);
}

bool DwarfSensor::hasChanged(){
    return this->changed;
}

#endif