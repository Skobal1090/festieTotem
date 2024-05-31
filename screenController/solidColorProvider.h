#ifndef _SOLID_COLOR_PROVIDER_H
#define _SOLID_COLOR_PROVIDER_H
#include "colorProvider.h"

class SolidColorProvider : public ColorProvider{
  public:
    SolidColorProvider(double* inputColor);
    virtual double* getColor();
  private:
    //double inputColor[3];
};

#endif