#include "Arduino.h"
#include <stdint.h>

class ConnectionListener {
  public:
    ConnectionListener(void (&onConnected)(), void (&onDisconnected)(), void (&onScanStarted)());
    void onConnected();
    void onDisconnected();
    void onScanStarted();
};

class TextUpdatedListener {
  public:
    TextUpdatedListener(void (&onTextUpdated)(String));
    void onTextUpdated(String newText);
};

class Attributes {
  public:
    Attributes(String parms) {}
    int Mode;
    int ScrollMode;
    int ScrollSpeed;
    uint8_t* colors;
};

class AttributesUpdatedListener {
  public:
    AttributesUpdatedListener(void (&onAttributesUpdated)(Attributes));
    void onAttributesUpdated(Attributes newVals);
};

class BleManager {
  public:
    BleManager();
    void begin();
    void scan();
    void setConnectionListener(ConnectionListener* listener);
    void setTextUpdatedListener(TextUpdatedListener* listener);
    void setAttributesUpdatedListener(AttributesUpdatedListener* listener);
};