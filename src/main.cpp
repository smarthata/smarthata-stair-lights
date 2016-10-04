#include "StairLights.h"

StairLights *stairLights;

void setup() {
    stairLights = new StairLights();
}

void loop() {
    stairLights->loop();
}
