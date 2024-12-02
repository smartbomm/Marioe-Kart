#ifndef CARRERACONTROLL_H
#define CARRERACONTROLL_H

#include <Arduino.h>

class CarreraControll {
public:

    CarreraControll();
    static void IRAM_ATTR timerInterrupt();

    void conf(int pin);
    void pushWord(int CPin);
    
    void changeBits(int carNr, int value, int shifts, int mask);
    int Cpin;
    bool Crun;
    void cycle();
    void setID(int carID);
    bool setID();
    void drive(int CarNr, int Speed);
    void program(int carNr, int maxSpeed, int brake, int fuel);
    bool program();
    int reverseBits(int value, int length);
    void changeList(int Nr, int value, int carNr);
    void driveAll(int value);
private:
    void initTime();
    int IDtoCar(int CarID);







    
};

#endif
