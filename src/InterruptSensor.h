#ifndef STAIR_LIGHTS_INTERRUPT_SENSOR_H
#define STAIR_LIGHTS_INTERRUPT_SENSOR_H

#include <Ultrasonic.h>
#include <Timing.h>

class InterruptSensor {
public:

    static const int SAVE_INTERVAL = 3000;

    InterruptSensor(Ultrasonic ultrasonic) : ultrasonic(ultrasonic) {}

    bool isInterrupted() {
        bool interrupted = timeout.isReady() && ultrasonic.getDistance() < 150;
        if (interrupted) {
            timeout.start(SAVE_INTERVAL);
        }
        return interrupted;
    }

private:
    Ultrasonic ultrasonic;
    Timeout timeout = Timeout();
};

#endif