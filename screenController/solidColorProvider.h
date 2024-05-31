#ifndef _SOLID_COLOR_PROVIDER_H
#define _SOLID_COLOR_PROVIDER_H
#include "colorProvider.h"

class SolidColorProvider : public ColorProvider{
  public:
    SolidColorProvider(uint8_t* inputColor);
    virtual uint8_t* getColor();
};

#endif