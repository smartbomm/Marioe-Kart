#include <Arduino.h>

class LED {
  private:
    bool actualState;
    int pin;
  public:
    LED(int p);
    void toggle();

};