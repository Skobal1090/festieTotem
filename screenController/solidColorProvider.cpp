#include "solidColorProvider.h"

uint8_t color[3]; 
SolidColorProvider::SolidColorProvider(uint8_t* inputColor){
  color[0] = inputColor[0]; //R
  color[1] = inputColor[1]; //G
  color[2] = inputColor[2]; //B
};

uint8_t* SolidColorProvider::getColor(){
  return color;
}