
/// @reddit: reddit.com/u/ZachVorhies

#pragma once

#include "sdkconfig.h"

#ifndef CONFIG_IDF_TARGET_ESP32S3
#error "This file is only for ESP32-S3"
#endif

#include "cpixel_ledcontroller.h"
#include "pixel_iterator.h"
#include "fl/vector.h"
#include "eorder.h"


namespace fl {

/// @brief Internal driver interface for I2S_Esp32. Use this
class InternalI2SDriver {
  public:
    static InternalI2SDriver* create();
    virtual ~InternalI2SDriver() {};
    virtual void initled(
      uint8_t* led_block,
      const int* pins,  // array of ints representing the gpio pins.
      int number_of_strips,  // the number of strips, also describes the size of the pins array.
      int number_of_leds_per_strip) = 0;
    virtual void setBrightness(uint8_t brightness) = 0;
    virtual void show() = 0;
};

class I2S_Esp32 {
  public:
    void beginShowLeds(int data_pin, int nleds);
    void showPixels(uint8_t data_pin, PixelIterator& pixel_iterator);
    void endShowLeds();
};

// TODO: RGBW support, should be pretty easy except the fact that I2S_Esp32
// either supports RGBW on all pixels strips, or none.
template <int DATA_PIN, EOrder RGB_ORDER = RGB>
class ClocklessController_I2S_Esp32_WS2812
    : public CPixelLEDController<RGB_ORDER> {
  private:
    typedef CPixelLEDController<RGB_ORDER> Base;
    I2S_Esp32 mI2S_Esp32;

  public:
    ClocklessController_I2S_Esp32_WS2812() = default;
    void init() override {}
    virtual uint16_t getMaxRefreshRate() const { return 800; }

  protected:
    // Wait until the last draw is complete, if necessary.
    virtual void *beginShowLeds(int nleds) override {
        void *data = Base::beginShowLeds(nleds);
        mI2S_Esp32.beginShowLeds(DATA_PIN, nleds);
        return data;
    }

    // Prepares data for the draw.
    virtual void showPixels(PixelController<RGB_ORDER> &pixels) override {
        auto pixel_iterator = pixels.as_iterator(this->getRgbw());
        mI2S_Esp32.showPixels(DATA_PIN, pixel_iterator);
    }

    // Send the data to the strip
    virtual void endShowLeds(void *data) override {
        Base::endShowLeds(data);
        mI2S_Esp32.endShowLeds();
    }
};

} // namespace fl