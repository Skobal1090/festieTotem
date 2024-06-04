#include "bleManager.h"
#include <ArduinoBLE.h>

static BLEService totemService("6364e354-24f2-4048-881f-4943362d34d7"); // create service

// create switch characteristic and allow remote device to read and write
static BLEStringCharacteristic textCharacteristic("7504932b-317e-4bb8-9998-5d2f5f3d18b3", BLERead | BLEWrite, 512);
static BLEStringCharacteristic colorCharacteristic("0a704fd3-5dba-4194-9f40-ac8e2ab4ff06", BLERead | BLEWrite, 512);

static ConnectionListener* connectionListener;
static AttributesUpdatedListener* attributesListener;
static TextUpdatedListener* textListener;

void connectionHandler(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());

  if(connectionListener != NULL){
    connectionListener->onConnected();
  }
}

void disconnectionHandler(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());

  if(connectionListener != NULL){
    connectionListener->onDisconnected();
  }
}

void textCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  String val = textCharacteristic.value();
  Serial.print("Text Characteristic event, written: " + val);

  if(textListener != NULL){
    textListener->onTextUpdated(val);
  }
}

void colorCharacteristicWritten(BLEDevice central, BLECharacteristic characteristic) {
  String val = textCharacteristic.value();
  Serial.print("Color Characteristic event, written: " + val);

  if(attributesListener != NULL){
    attributesListener->onAttributesUpdated(Attributes(val));
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
  totemService.addCharacteristic(colorCharacteristic);

  BLE.addService(totemService);

  BLE.setEventHandler(BLEConnected, connectionHandler);
  BLE.setEventHandler(BLEDisconnected, disconnectionHandler);

  textCharacteristic.setEventHandler(BLEWritten, textCharacteristicWritten);
  textCharacteristic.setValue("Ready to rage");

  colorCharacteristic.setEventHandler(BLEWritten, colorCharacteristicWritten);
  colorCharacteristic.setValue("0,0,0");

  BLE.advertise();
  Serial.println(("Totem on the prowl..."));
}

void BleManager::scan(){
  BLE.poll();
}

void BleManager::setConnectionListener(ConnectionListener* listener){
  connectionListener = listener;
}

void BleManager::setAttributesUpdatedListener(AttributesUpdatedListener* listener){
  attributesListener = listener;
}

void BleManager::setTextUpdatedListener(TextUpdatedListener* listener){
  textListener = listener;
}

TextUpdatedListener::TextUpdatedListener(void (&onTextUpdated)(String)){}

void TextUpdatedListener::onTextUpdated(String newText){}

AttributesUpdatedListener::AttributesUpdatedListener(void (&onAttributesUpdated)(Attributes)) {}

void AttributesUpdatedListener::onAttributesUpdated(Attributes newVals) {}

ConnectionListener::ConnectionListener(void (&onConnected)(), void (&onDisconnected)(), void (&onScanStarted)()) {}
void ConnectionListener::onConnected() {}
void ConnectionListener::onDisconnected() {}
void ConnectionListener::onScanStarted() {}