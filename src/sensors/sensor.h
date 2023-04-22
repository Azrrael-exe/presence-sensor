#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class Sensor {
    private:
        int value = 0;
        bool changed;
        virtual int read();
    public:
        void loop();
        bool hasChanged();
        bool getValue();      
};

void Sensor::loop() {
    int actual_value = this->read();
    if (actual_value != value) {
        this->changed = true;
        this->value = actual_value;
    }
}

bool Sensor::getValue() {
    this->changed = false;
    return bool(this->value);
}

bool Sensor::hasChanged() {
    return this->changed;
}

#endif
