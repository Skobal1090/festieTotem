#include "bleManager.h"
#include <ArduinoBLE.h>

BLEService totemService("6364e354-24f2-4048-881f-4943362d34d7"); // create service

// create switch characteristic and allow remote device to read and write
BLEStringCharacteristic textCharacteristic("7504932b-317e-4bb8-9998-5d2f5f3d18b3", BLERead | BLEWrite, 512);
BLEStringCharacteristic colorCharacteristic("0a704fd3-5dba-4194-9f40-ac8e2ab4ff06", BLERead | BLEWrite, 512);

void connectionHandler(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
}

void disconnectionHandler(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
}

void textCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  String val = textCharacteristic.value();
  Serial.print("Text Characteristic event, written: " + val);
}

void colorCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  String val = textCharacteristic.value();
  Serial.print("Color Characteristic event, written: " + val);
}

BleManager::BleManager(){
  if (!BLE.begin()) {
    Serial.println("Something has gone horrifically wrong");

    while (1);
  }

  BLE.setLocalName("Pineapple Fox");
  BLE.setAdvertisedService(totemService);
  totemService.addCharacteristic(textCharacteristic);
  totemService.addCharacteristic(colorCharacteristic);

  BLE.addService(totemService);

  BLE.setEventHandler(BLEConnected, connectionHandler);
  BLE.setEventHandler(BLEDisconnected, disconnectionHandler);

  textCharacteristic.setEventHandler(BLEWritten, textCharacteristicWritten);
  textCharacteristic.setValue("Ready to rage");

  colorCharacteristic.setEventHandler(BLEWritten, colorCharacteristicWritten);
  colorCharacteristic.setValue("0,0,0");
}


void BleManager::begin(){
  BLE.advertise();
  Serial.println(("Totem on the prowl..."));
}

void BleManager::scan(){
  BLE.poll();
}