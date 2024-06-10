#include "bleManager.h"
#include <ArduinoBLE.h>

static BLEService totemService("6364e354-24f2-4048-881f-4943362d34d7"); // create service

// create switch characteristic and allow remote device to read and write
static BLEStringCharacteristic textCharacteristic("7504932b-317e-4bb8-9998-5d2f5f3d18b3", BLERead | BLEWrite, 512);
static BLEByteCharacteristic attributesCharacteristic("0a704fd3-5dba-4194-9f40-ac8e2ab4ff06", BLERead | BLEWrite, 512);

static void (*onConnected)();
static void (*onDisconnected)();
static void (*onAttributesUpdated)(Attributes);
static void (*onTextUpdated)(String);

void connectionHandler(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());

  if(onConnected != NULL){
    onConnected();
  }
}

void disconnectionHandler(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());

  if(onDisconnected != NULL){
    onDisconnected();
  }
}

void textCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  String val = textCharacteristic.value();
  Serial.print("Text Characteristic event, written: " + val);

  if(onTextUpdated != NULL){
    onTextUpdated(val);
  }
}

void attributesCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  uint8_t* val = attributesCharacteristic.value();
  Serial.print("Attribute characteristic event, written: " + val);

  if(onAttributesUpdated != NULL){
    onAttributesUpdated(Attributes(val));
  }
}

void BleManager::init(){
  if (!BLE.begin()) {
    Serial.println("Something has gone horrifically wrong");

    while (1);
  }

  BLE.setLocalName("Pineapple Fox");
  BLE.setAdvertisedService(totemService);
  totemService.addCharacteristic(textCharacteristic);
  totemService.addCharacteristic(attributesCharacteristic);

  BLE.addService(totemService);

  BLE.setEventHandler(BLEConnected, connectionHandler);
  BLE.setEventHandler(BLEDisconnected, disconnectionHandler);

  textCharacteristic.setEventHandler(BLEWritten, textCharacteristicWritten);
  textCharacteristic.setValue("Ready to rage");

  attributesCharacteristic.setEventHandler(BLEWritten, attributesCharacteristicWritten);
  attributesCharacteristic.setValue("0,0,0");



  BLE.advertise();
  Serial.println(("Totem on the prowl..."));
}

void BleManager::scan(){
  BLE.poll();
}

void BleManager::setConnectionListener(void (&connected)(), void (&disconnected)()){
  onConnected = connected;
  onDisconnected = disconnected;
}

void BleManager::setAttributesUpdatedListener(void (&attributesUpdated)(Attributes)){
  onAttributesUpdated = attributesUpdated;
}

void BleManager::setTextUpdatedListener(void (&textUpdated)(String)){
  onTextUpdated = textUpdated;
}