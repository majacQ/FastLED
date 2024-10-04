/// @file    Noise.ino
/// @brief   Demonstrates how to use noise generation on a 2D LED matrix
/// @example Noise.ino


#include <FastLED.h>

#include "fx/2d/noisepalette.hpp"
#include "fx/2d/animartrix.hpp"
#include "fx/fx_engine.h"

#define LED_PIN 2
#define BRIGHTNESS 96
#define LED_TYPE WS2811
#define COLOR_ORDER GRB

#define MATRIX_WIDTH 22
#define MATRIX_HEIGHT 22
#define GRID_SERPENTINE 1

#define NUM_LEDS (MATRIX_WIDTH * MATRIX_HEIGHT)

#define SCALE 20
#define SPEED 30

CRGB leds[NUM_LEDS];
XYMap xyMap(MATRIX_WIDTH, MATRIX_HEIGHT, GRID_SERPENTINE);
NoisePalette noisePalette(xyMap);
Animartrix animartrix(xyMap, POLAR_WAVES);

FxEngine fxEngine(NUM_LEDS);



void setup() {
    delay(1000); // sanity delay
    FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS)
        .setCorrection(TypicalLEDStrip);
    FastLED.setBrightness(96);
    noisePalette.lazyInit();
    noisePalette.setSpeed(SPEED);
    noisePalette.setScale(SCALE);
    fxEngine.addFx(&noisePalette);
    fxEngine.addFx(&animartrix);
}

void loop() {
    uint32_t now = millis();
    EVERY_N_MILLISECONDS(5000) {
        noisePalette.changeToRandomPalette();
    }

    EVERY_N_SECONDS(1) {
        fxEngine.nextFx(now, 0);
    }
    fxEngine.draw(millis(), leds);
    FastLED.show();
}
