#ifndef _COLOR_INTERPOLATOR_H
#define _COLOR_INTERPOLATOR_H
#include "colorProvider.h"

typedef struct {
    uint8_t r;       // a fraction between 0 and 1
    uint8_t g;       // a fraction between 0 and 1
    uint8_t b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    uint8_t h;       // angle in degrees
    uint8_t s;       // a fraction between 0 and 1
    uint8_t v;       // a fraction between 0 and 1
} hsv;

class ColorInterpolator : public ColorProvider{
    public:
      // Takes in color1, color2 in RGB format
      ColorInterpolator(uint8_t* color1, uint8_t* color2);
      // getColor() returns the color value in RGB format
      virtual uint8_t* getColor();
    private:
      hsv startColor;
      hsv endColor;
      hsv rgb2hsv(rgb in);
      rgb hsv2rgb(hsv in);
      uint8_t interpolateLinearly(uint8_t a, uint8_t b, uint8_t t);
};

#endif