#include "Arduino.h"
#include <stdint.h>

class Attributes {
  public:
    Attributes(String parms) {}
    int Mode;
    int ScrollMode;
    int ScrollSpeed;
    uint8_t* colors;
};

class BleManager {
  public:
    void init();
    void scan();
    void setConnectionListener(void (&onConnected)(), void (&onDisconnected)());
    void setTextUpdatedListener(void (&onTextUpdated)(String));
    void setAttributesUpdatedListener(void (&onAttributesUpdated)(Attributes));
};