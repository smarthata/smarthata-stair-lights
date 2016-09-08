#include "ButtonSafe.h"
#include "Led.h"

#define BUTTON_PIN_OK A5
#define BUTTON_PIN_MODE A6
#define LED_PIN 13

ButtonPullUp okButton(BUTTON_PIN_OK);
ButtonPullUp modeButton(BUTTON_PIN_MODE);
Led led(LED_PIN);

void setup() {
}

void loop() {

    if (okButton.read()) {
        led.high();
    } else {
        led.low();
    }

    delay(10);
}
