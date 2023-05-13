#ifndef LIGHTHOUSE_DIPLAY_H
#define LIGHTHOUSE_DIPLAY_H

#include <Arduino.h>
#include "actuators/display.h"
#include "config/constants.h"
#include <Adafruit_NeoPixel.h>

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(LIGHTS_COUNT , LIGHTS_PIN, NEO_GRB + NEO_KHZ800);


class LighthouseDisplay : public DisplayABC {
    public:
        void setOccupancy(float occupancy);
        void config();
};

void LighthouseDisplay::setOccupancy(float occupancy) {
    if (occupancy >= 0.8) {
        for (int i = 0; i <= LIGHTS_COUNT; ++i) {
            pixels.setPixelColor(i, 255, 0, 0);
        }
        pixels.show();
    }
    else if ((0.8 > occupancy) && (occupancy >= 0.5)){
        for (int i = 0; i <= LIGHTS_COUNT; ++i) {
            pixels.setPixelColor(i, 255, 255, 9);
        }
        pixels.show();        
    }
    else if (occupancy < 0.5){
        for (int i = 0; i <= LIGHTS_COUNT; ++i) {
            pixels.setPixelColor(i, 0, 255, 0);
        }
        pixels.show();
    }
}

void LighthouseDisplay::config() {
    pinMode(LIGHTS_PIN, OUTPUT);
    pixels.begin();
    pixels.clear();
    pixels.show();
}

#endif