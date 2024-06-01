#include "Arduino.h"
#include <stdint.h>

class BleManager {
  public:
    BleManager();
    void begin();
    void scan();
};