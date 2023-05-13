#ifndef SENSOR_H
#define SENSOR_H

#include <Arduino.h>

class SensorABC {
    private:
        virtual int read();
    public:
        virtual void loop();
        virtual bool hasChanged();
        virtual bool getValue();      
};

#endif
