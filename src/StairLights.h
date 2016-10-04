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

#define BRIGHTNESS  96

class StairLights {
public:

    StairLights() {
        FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
        FastLED.setBrightness(BRIGHTNESS);
        FastLED.show();
    }

    void loop() {
        led.set(modeButton.read());

        if (main.isReady()) {
            if (okButton.read()) {
                for (int j = 0; j < NUM_LEDS; ++j) {
                    leds[j] = CRGB(255 * rand(), 255 * rand(), 255 * rand());
                }
                FastLED.show();
            }
        }
    }

private:
    ButtonSafe okButton = ButtonSafe(new ButtonPullUp(BUTTON_PIN_OK));
    ButtonSafe modeButton = ButtonSafe(new ButtonPullUp(BUTTON_PIN_MODE));
    Led led = Led(LED_PIN);
    CRGB leds[NUM_LEDS];

    Interval main = Interval(50);
};

#endif
