#ifndef PIR_SENSOR_H
#define PIR_SENSOR_H

#include <Arduino.h>
#include "sensors/sensor.h"

class PirSensor:public Sensor {
    private:
        int pin;
        int read();
    public:
        void config();
        PirSensor(int pin);   
};

PirSensor::PirSensor(int pin) : Sensor() {
    this->pin = pin;
}

void PirSensor::config() {
    pinMode(this->pin, INPUT);
}

int PirSensor::read() {
    return int(digitalRead(this->pin));
}

#endif