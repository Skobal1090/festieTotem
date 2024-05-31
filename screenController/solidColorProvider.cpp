#include "solidColorProvider.h"

double color[3]; 
SolidColorProvider::SolidColorProvider(double* inputColor){
  color[0] = inputColor[0]; //R
  color[1] = inputColor[1]; //G
  color[2] = inputColor[2]; //B
};

double* SolidColorProvider::getColor(){
  return color;
}