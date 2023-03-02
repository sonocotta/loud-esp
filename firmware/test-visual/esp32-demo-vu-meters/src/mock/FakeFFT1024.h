#pragma once

#include "Arduino.h"

class FakeFFT1024
{
private:
    int16_t value = 0;
    const int16_t mean = 1 << 8;
    const int16_t shiftMax = 1 << 9;
public:
    FakeFFT1024() {};
    ~FakeFFT1024() {};

    bool available() {
		return true;
	}

	// float read(unsigned int binNumber) {
    //     value = mean + (random(shiftMax) - shiftMax/2);
    //     return abs((float)value / 32768.0f);
	// }

	float read(unsigned int binFirst, unsigned int binLast) {
        value = mean + (random(shiftMax) - shiftMax/2);
        // Serial.printf("%d -> %.2f \n", value, (float)value / 32768.0f);
		return abs((float)value / 32768.0f);
	}
};
