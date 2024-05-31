#ifndef _COLOR_INTERPOLATOR_H
#define _COLOR_INTERPOLATOR_H
#include "colorProvider.h"

typedef struct {
    double r;       // a fraction between 0 and 1
    double g;       // a fraction between 0 and 1
    double b;       // a fraction between 0 and 1
} rgb;

typedef struct {
    double h;       // angle in degrees
    double s;       // a fraction between 0 and 1
    double v;       // a fraction between 0 and 1
} hsv;

class ColorInterpolator : public ColorProvider{
    public:
      // Takes in color1, color2 in RGB format
      ColorInterpolator(double* color1, double* color2);
      // getColor() returns the color value in RGB format
      virtual double* getColor();
    private:
      hsv startColor;
      hsv endColor;
      double t;
      hsv rgb2hsv(rgb in);
      rgb hsv2rgb(hsv in);
      double interpolateLinearly(double a, double b, double t);
};

#endif