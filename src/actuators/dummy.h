#ifndef DUMMY_H
#define DUMMY_H

#include <Arduino.h>
#include "actuators/display.h"

class Dummy : public DisplayABC {
    public:
        void setOccupancy(float occupancy);
};

void Dummy::setOccupancy(float occupancy) {
    Serial.println(occupancy);
}

#endif