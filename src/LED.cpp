#include "LED.h"

    LED::LED(int p){
      pin = p;
      pinMode(pin, OUTPUT);
      digitalWrite(pin, false);
    }
    void LED::toggle() {
      if(actualState) digitalWrite(pin, false);
      else digitalWrite(pin, true);
    }

