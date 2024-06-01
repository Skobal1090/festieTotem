#include "bleManager.h"

#define PIN 7

BleManager manager;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  manager = BleManager();

  manager.begin();
}

void loop() {
  manager.scan();
}