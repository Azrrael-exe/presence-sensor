#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>

class DisplayABC {
    public:
        virtual void setOccupancy(float occupancy); 
};

#endif