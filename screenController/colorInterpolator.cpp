#include "colorInterpolator.h"

static hsv   rgb2hsv(rgb in);
static rgb   hsv2rgb(hsv in);

hsv ColorInterpolator::rgb2hsv(rgb in)
{
    hsv         out;
    double      min, max, delta;

    min = in.r < in.g ? in.r : in.g;
    min = min  < in.b ? min  : in.b;

    max = in.r > in.g ? in.r : in.g;
    max = max  > in.b ? max  : in.b;

    out.v = max;                                // v
    delta = max - min;
    if (delta < 0.00001)
    {
        out.s = 0;
        out.h = 0; // undefined, maybe nan?
        return out;
    }
    if( max > 0.0 ) { // NOTE: if Max is == 0, this divide would cause a crash
        out.s = (delta / max);                  // s
    } else {
        // if max is 0, then r = g = b = 0              
        // s = 0, h is undefined
        out.s = 0.0;
        return out;
    }
    if( in.r >= max )                           // > is bogus, just keeps compilor happy
        out.h = ( in.g - in.b ) / delta;        // between yellow & magenta
    else
    if( in.g >= max )
        out.h = 2.0 + ( in.b - in.r ) / delta;  // between cyan & yellow
    else
        out.h = 4.0 + ( in.r - in.g ) / delta;  // between magenta & cyan

    out.h *= 60.0;                              // degrees

    if( out.h < 0.0 )
        out.h += 360.0;

    return out;
}


rgb ColorInterpolator::hsv2rgb(hsv in)
{
    double      hh, p, q, t, ff;
    long        i;
    rgb         out;

    if(in.s <= 0.0) {       // < is bogus, just shuts up warnings
        out.r = in.v;
        out.g = in.v;
        out.b = in.v;
        return out;
    }
    hh = in.h;
    if(hh >= 360.0) hh = 0.0;
    hh /= 60.0;
    i = (long)hh;
    ff = hh - i;
    p = in.v * (1.0 - in.s);
    q = in.v * (1.0 - (in.s * ff));
    t = in.v * (1.0 - (in.s * (1.0 - ff)));

    switch(i) {
    case 0:
        out.r = in.v;
        out.g = t;
        out.b = p;
        break;
    case 1:
        out.r = q;
        out.g = in.v;
        out.b = p;
        break;
    case 2:
        out.r = p;
        out.g = in.v;
        out.b = t;
        break;

    case 3:
        out.r = p;
        out.g = q;
        out.b = in.v;
        break;
    case 4:
        out.r = t;
        out.g = p;
        out.b = in.v;
        break;
    case 5:
    default:
        out.r = in.v;
        out.g = p;
        out.b = q;
        break;
    }
    return out;     
}

hsv startColor;
hsv endColor;
uint8_t t;

ColorInterpolator::ColorInterpolator(uint8_t* color1, uint8_t* color2){
  startColor = rgb2hsv((rgb){color1[0], color1[1], color1[2]});
  endColor = rgb2hsv((rgb){color2[0], color2[1], color2[2]});
  t = 0.0;
}

uint8_t ColorInterpolator::interpolateLinearly(uint8_t a, uint8_t b, uint8_t t){
  return a * (1 - (t * 0.01)) + b * (t * 0.01);
}

uint8_t* ColorInterpolator::getColor(){
  static uint8_t color[3];
  hsv result;

  result.h = interpolateLinearly(startColor.h, endColor.h, t);
  result.s = interpolateLinearly(startColor.s, endColor.s, t);
  result.v = interpolateLinearly(startColor.v, endColor.v, t);

  t += 1;
  
  // if we've reached the end goal, the colors now flip and t resets
  if(t >= 100){
    hsv tmp = endColor;
    endColor = startColor;
    startColor = tmp;

    t = 0;
  }

  rgb conv = hsv2rgb(result);
  color[0] = conv.r;
  color[1] = conv.g;
  color[2] = conv.b;
  return color;
}