#ifndef STAIR_LIGHTS_LIGHT_SENSOR_H
#define STAIR_LIGHTS_LIGHT_SENSOR_H

#include <Timing.h>

class LightSensor {
public:

    static const int NIGHT_THRESHOLD = 512;

    LightSensor(byte pin) : pin(pin) {
        pinMode(pin, INPUT);
    }

    bool isNight() {
        if (readInterval.isReady()) {
            value = analogRead(pin) < NIGHT_THRESHOLD;
        }
        return value;
    }

private:
    byte pin;
    bool value;
    Interval readInterval = Interval(10000);
};

#endif