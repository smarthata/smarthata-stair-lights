#ifndef STAIR_LIGHTS_H
#define STAIR_LIGHTS_H

#include <FastLED.h>
#include <ButtonSafe.h>
#include <Led.h>

#define BUTTON_PIN_OK   A5
#define BUTTON_PIN_MODE A6
#define LED_PIN         13

#define DATA_PIN    7
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG

class StairLights {
public:

    static const int STAIRS_COUNT = 15;
    static const int LEDS_ON_STAIR = 10;
    static const int NUM_LEDS = STAIRS_COUNT * LEDS_ON_STAIR;

    static const int WAVE_SPEED = 1000;
    static const int WAVE_STAIRS = 5;

    static const int BRIGHTNESS = 255;
    static const int MAX_POWER = 100;
    static const int DUTY_POWER = 10;

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
        clear();

        showDutyLights();

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

    void clear() {
        for (int led = 0; led < NUM_LEDS; ++led) {
            leds[led] = CRGB::Black;
        }
    }

    void showDutyLights() {
        highlightStair(0, DUTY_POWER);
        highlightStair(STAIRS_COUNT - 1, DUTY_POWER);
    }

    void highlightStair(int stair, byte power) {
        if (stair >= 0 && stair < STAIRS_COUNT) {
            for (int led = 0; led < LEDS_ON_STAIR; ++led) {
                CRGB &crgb = leds[stair * LEDS_ON_STAIR + led];
                crgb.r = max(crgb.r, power);
                crgb.g = max(crgb.g, power);
                crgb.b = max(crgb.b, power);
            }
        }
    }

private:
    ButtonSafe okButton = ButtonSafe(new ButtonPullUp(BUTTON_PIN_OK));
    ButtonSafe modeButton = ButtonSafe(new ButtonPullUp(BUTTON_PIN_MODE));
    Led builtInLed = Led(LED_PIN);
    CRGB leds[NUM_LEDS];

    Interval main = Interval(20);
    Interval testRun = Interval(20000);
    Stopwatch sw = Stopwatch();
};

#endif
