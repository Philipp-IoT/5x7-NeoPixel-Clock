#include "driver_led.h"

DriverLED::DriverLED(const int pin) : _pixels(LED_TOTAL_COUNT, pin, NEO_GRB + NEO_KHZ800) {
    for(int i=0; i<5; i++) {
        _chars[i] = 0;
    }

    gradientOffset = 0;

    UpdateColorGradient();

    //UpdateDisplay();
}

void DriverLED::Begin() {
    _pixels.begin();
    _pixels.clear();

    _pixels.show();
}

void DriverLED::SetChars(uint8_t *chars) {
    for (int i = 0; i < 5; i++)
    {
        _chars[i] = chars[i];
    }
}

void DriverLED::UpdateDisplay() {

    for(int c = 0; c < 5; c++) 
    {
        uint8_t currentChar = _chars[c];

        for (int i = 0; i < 7; i++)
        {
            for (int j = 0; j < 5; j++)
            {   
                uint8_t charBit = NUMERIC_CHARS[currentChar][j] & (1 << i);
                uint32_t col = Adafruit_NeoPixel::Color(0, 0, 0);
                if (charBit)
                {
                    col = colorGradient[c * 5 + i + j];
                }

                _pixels.setPixelColor((LED_MODULE_COUNT*c) + (i * 5) + j, col);
            }
        }
    }

    _pixels.show();
    //delay(50);
    UpdateColorGradient();
}

void DriverLED::UpdateColorGradient() {
    for (uint32_t i = 0; i < GRADIENT_MAX; i++)
    {
        colorGradient[i] = Adafruit_NeoPixel::ColorHSV(((i * GRADIENT_STEP) - gradientOffset) % 65535, 255, 32);
    }
    gradientOffset += GRADIENT_INC;
}
