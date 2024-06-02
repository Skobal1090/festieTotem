#ifndef _RANDOM_COLOR_PROVIDER_H
#define _RANDOM_COLOR_PROVIDER_H
#include "colorProvider.h"

class RandomColorProvider : public ColorProvider {
  public:
    RandomColorProvider();
    virtual uint8_t* getColor();
};

#endif