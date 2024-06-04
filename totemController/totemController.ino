#include "bleManager.h"

void setup(){
    Serial.begin(9600);
    while (!Serial);

    manager.init();
}

void loop(){

}