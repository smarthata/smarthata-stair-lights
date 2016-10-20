#ifndef STAIR_LIGHTS_H
#define STAIR_LIGHTS_H

#include <FastLED.h>
#include <ButtonSafe.h>
#include <Led.h>
#include "InterruptSensor.h"

#define BUTTON_PIN_OK   A5
#define BUTTON_PIN_MODE A6
#define LED_PIN         13

#define DATA_PIN    7
#define LED_TYPE    WS2811
#define COLOR_ORDER BRG

enum Direction {
    BOTTOM, TOP
};

struct Wave {
    Direction direction;
    unsigned long start;
};

class StairLights {
public:

    static const int STAIRS_COUNT = 15;
    static const int LEDS_ON_STAIR = 10;
    static const int NUM_LEDS = STAIRS_COUNT * LEDS_ON_STAIR;

    static const int WAVE_SPEED = 1000;
    static const int WAVE_STAIRS = 5;
    static const int WAVE_MAX_COUNT = 20;

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
            addWave(BOTTOM);
            addWave(TOP);
        }
        if (main.isReady()) {
            checkInterruptSensors();
            show();
        }
    }

    void show() {
        clear();

        showDutyLights();

        for (int wave = 0; wave < waveCount; ++wave) {
            processWave(waves[wave]);
        }

        FastLED.show();
    }

    void processWave(Wave wave) {
        unsigned long time = millis() - wave.start;
        int lastStair = 0;
        byte lastPower;
        if (wave.direction == TOP) {
            lastStair = (int) (time / WAVE_SPEED);
            lastPower = (byte) map(time % WAVE_SPEED, 0, WAVE_SPEED, 0, MAX_POWER);
        } else {
            lastStair = STAIRS_COUNT + WAVE_STAIRS - 1 - (int) (time / WAVE_SPEED);
            lastPower = (byte) map(time % WAVE_SPEED, WAVE_SPEED, 0, 0, MAX_POWER);
        }

        highlightStair(lastStair, lastPower);

        int firstStair = lastStair - WAVE_STAIRS;
        highlightStair(firstStair, MAX_POWER - lastPower);

        for (int stair = firstStair + 1; stair < lastStair; ++stair) {
            highlightStair(stair, MAX_POWER);
        }
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

    void addWave(Direction direction) {
        if (waveCount < WAVE_MAX_COUNT) {
            Wave &wave = waves[waveCount];
            wave.direction = direction;
            wave.start = millis();
            waveCount++;
        }
    }

    void checkInterruptSensors() {
        if (bottom.isInterrupted()) {
            addWave(TOP);
        }
        if (top.isInterrupted()) {
            addWave(BOTTOM);
        }
    }

private:
    ButtonSafe okButton = ButtonSafe(new ButtonPullUp(BUTTON_PIN_OK));
    ButtonSafe modeButton = ButtonSafe(new ButtonPullUp(BUTTON_PIN_MODE));
    Led builtInLed = Led(LED_PIN);
    CRGB leds[NUM_LEDS];

    Interval main = Interval(20);
    Interval testRun = Interval(20000);

    Wave waves[WAVE_MAX_COUNT];
    int waveCount = 0;

    InterruptSensor bottom = InterruptSensor(Ultrasonic(8, 9));
    InterruptSensor top = InterruptSensor(Ultrasonic(10, 11));

};

#endif
