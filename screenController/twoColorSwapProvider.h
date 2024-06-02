#ifndef _TWO_COLOR_SWAP_PROVIDER_H
#define _TWO_COLOR_SWAP_PROVIDER_H
#include "colorProvider.h"

class TwoColorSwapProvider : public ColorProvider {
  public:
    //color1, color2 are passed in RGB format
    TwoColorSwapProvider(uint8_t* color1, uint8_t* color2);
    virtual uint8_t* getColor();
    uint8_t* colorArray[2];
};
#endif