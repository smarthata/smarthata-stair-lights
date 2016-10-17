#ifndef STAIR_LIGHTS_H
#define STAIR_LIGHTS_H

#include <FastLED.h>
#include <ButtonSafe.h>
#include <Led.h>

#define BUTTON_PIN_OK A5
#define BUTTON_PIN_MODE A6
#define LED_PIN 13

#define DATA_PIN    7
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG
#define NUM_LEDS    10

#define BRIGHTNESS  255

class StairLights {
public:

    static const int STAIRS_COUNT = 5;
    static const int LEDS_ON_STAIR = 2;
    static const int WAVE_SPEED = 1000;
    static const int WAVE_STAIRS = 2;

    static const int MAX_POWER = 100;

    StairLights() {
        FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
    }

    void loop() {
        if (testRun.isReady()) {
            sw.start();
        }
        if (main.isReady()) {
            show();
        }
    }

    void show() {
        FastLED.clearData();
        unsigned long time = sw.time();
        int lastStair = (int) (time / WAVE_SPEED);
        byte lastPower = (byte) map(time % WAVE_SPEED, 0, WAVE_SPEED, 0, MAX_POWER);

        highlightStair(lastStair, lastPower);

        int firstStair = lastStair - WAVE_STAIRS;
        highlightStair(firstStair, MAX_POWER - lastPower);

        for (int stair = firstStair + 1; stair < lastStair; ++stair) {
            highlightStair(stair, MAX_POWER);
        }
        FastLED.show();
    }

    void highlightStair(int stair, byte power) {
        if (stair >= 0 && stair < STAIRS_COUNT) {
            for (int led = 0; led < LEDS_ON_STAIR; ++led) {
                leds[stair * LEDS_ON_STAIR + led] = CRGB(power, power, power);
            }
        }
    }

private:
    ButtonSafe okButton = ButtonSafe(new ButtonPullUp(BUTTON_PIN_OK));
    ButtonSafe modeButton = ButtonSafe(new ButtonPullUp(BUTTON_PIN_MODE));
    Led builtInLed = Led(LED_PIN);
    CRGB leds[NUM_LEDS];

    Interval main = Interval(20);
    Interval testRun = Interval(7500);
    Stopwatch sw = Stopwatch();
};

#endif
