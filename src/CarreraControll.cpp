#include <Arduino.h>
#include <CarreraControll.h>
//! [CarreraControllInit]
hw_timer_t *timer = NULL;


CarreraControll *interruptPointer;

static void cycle_e(){
  interruptPointer -> cycle();
}

CarreraControll::CarreraControll(){
  interruptPointer = this;
}

void IRAM_ATTR CarreraControll::timerInterrupt() {
 cycle_e();
}

void CarreraControll::initTime(){
        timer = timerBegin(0, 80, true); 
        timerAttachInterrupt(timer, &CarreraControll::timerInterrupt, true); //all the interrupts
        timerAlarmWrite(timer, 50, true); 
        timerAlarmEnable(timer); 
}
//! [CarreraControllInit]

//! [CarreraControllVariables]
int states[] = 
{
 0b1000000000000,  //0 > Programming Word
 0b1111000000,  //1 > Pace Ang GHOST Car
 0b10000000,  //2 > Aktiv Word
 0b1000100000,  //3 > Car 0
 0b1100100000,  //4 > Car 4
 0b1001100000,  //5 > Car 1
 0b1101100000,  //6 > Car 5
 0b1010100000,  //7 > Car 2
 0b10000000,  //8 > Aktiv Word
 0b1011100000,  //9 > Car 3
 };

int size[] = {13 , 10 , 8, 10 , 10 , 10 , 10 , 10 , 8 , 10}; //standard lenghts of the words

int values[] =
{
  0b1000000000101,
  0b1000010000101,
  0b1000001000101
}; //the standard values for the 3 cylicaly writen programming words (speed breaks and fuel)


int loc = 0;
int restCycle = 150; //cycle to rest between words
int numStates = size[0];
int statesInt = states[0];
bool doneCycle = true; //done with word
bool halfDone = true; //is true if half of the bit is written (either hight or low)
bool doneProtCH=false;  //flash flag
bool doneProtProgramming = true; //true if the protocoll got programmed
int CycleCount = 0;

bool normalMode = true;
int flashCount = 0;
int flashCar=0;
bool cycleAllowed = true;

bool inverted = true; //to invert the protocoll (if hardware changes)

bool programmed = true;
int pCount = 0; //which programming word is chosen
//! [CarreraControllVariables]

//! [CarreraControllCycle]
void CarreraControll::cycle() {
    if(normalMode){
    pushWord(Cpin);
      if (doneCycle){
        doneCycle = false;
        statesInt= states[CycleCount];
        numStates = size[CycleCount];
        Serial.print(statesInt);
      }
    }
    else{
           if(flashCount==-1||flashCount==-2){
      changeBits(flashCar,0,5,0b1);
      pushWord(Cpin);  
     }
     if(flashCount==0||flashCount==1||flashCount ==4||flashCount==5||flashCount==13||flashCount==12||flashCount ==17||flashCount==16){
      changeBits(flashCar,1,5,0b1);
      pushWord(Cpin); 
     }
     if(flashCount ==2||flashCount ==3||flashCount ==6||flashCount==7||flashCount ==11||flashCount==10||flashCount ==15||flashCount==14||flashCount == 18){
       changeBits(flashCar,0,5,0b1);  
       pushWord(Cpin);
     }
     if(flashCount == 8){
      cycleAllowed = false;
       digitalWrite(Cpin,inverted); //set signal to low
       flashCount++;
       }

      if(flashCount==9){
        CycleCount++;
       if(CycleCount>10000){
        flashCount++;
        cycleAllowed=true;
        CycleCount = 0;
        doneProtProgramming = true;
      }
     }
  if (doneProtCH & cycleAllowed & flashCount != 8 & flashCount != 9){
    doneProtCH = false;
    flashCount++;
  }
  if (doneCycle & cycleAllowed){
    doneCycle = false;
    statesInt= states[CycleCount];
    numStates = size[CycleCount];
   }
   if (flashCount==19){
    cycleAllowed = true;
    normalMode = true;
    flashCount = -2;
    digitalWrite(10,!digitalRead(10));
   }
  }
}
//! [CarreraControllCycle]






void CarreraControll::conf(int pin) { //part of startup of the protocoll
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  Cpin = pin;
  initTime();
  }
void CarreraControll::pushWord(int CPin) {

  if (halfDone == false && loc <= numStates) {  // Test for the 1st run and if smaller than the number of states 
      digitalWrite(CPin, !digitalRead(CPin));       // Flip state
      halfDone = true;
  }
  else if (loc > numStates-1 && loc != restCycle) { // Restart the cycle
      digitalWrite(CPin, !inverted);                   // Set pin LOW when restarting
      loc++;
  }
  else if (halfDone == true && loc <= numStates ) {  // Test for the 2nd run and if smaller than the number of states
      loc++;
      bool state = (statesInt >> numStates - loc) & 1;    // Extract the state from statesInt
      digitalWrite(CPin, state != inverted);              // Write the state from the bitshifted int
      halfDone = false;
  }
  else if (loc == restCycle) {                     // End of the cycle
      loc = 0;                                     // Reset the location
      doneCycle = true;
      CycleCount++;
  }
  if (CycleCount==10){
    CycleCount = 0;
    doneProtCH = true;
      if (pCount == 0||pCount ==1){
        states[0] = values[0];
        pCount++;
      }
      else if (pCount == 2||pCount==3){
        states[0] = values[1];
        pCount++;
      }
      else if (pCount == 4||pCount==5){
        states[0] = values[2];
        pCount++;
        if (pCount==5)
          pCount = 0;
        
      
    }
    
  }
}


void CarreraControll::changeBits(int carNr, int value, int shifts, int mask){ //it provides a more readable form of changing bits
  states[carNr] &= ~(mask << shifts);
  states[carNr] |= (value << shifts);
}





void CarreraControll::setID(int rID){ //starts the "other cycle" it will automaticly block all other actions till cylce is over
  if (normalMode == true){
  normalMode = false;
  flashCar = IDtoCar(rID);
  states[0]= 0b1110010000111; //change programming word (some cars dont accept the new id if the word is ment for them)
  doneProtCH = true;
  flashCount = -2;
  }
  

}
bool CarreraControll::setID(){
  if (doneProtProgramming){
  doneProtProgramming = false;
  return true;
  }
  else 
  return false;
}



int CarreraControll::IDtoCar(int carID){ //transform the external id into the internal id of the CarreraProtokoll
  int ID = 3;
    switch(carID) {
  case 0:
    ID = 3;
    break;
  case 1:
    ID = 5;
    break;
  case 2:
    ID = 7;
    break;
  case 3:
    ID = 9;
    break;
  case 4:
    ID = 4;
    break;
  case 5:
    ID = 6;
    break;
}
return ID;
}


void CarreraControll::drive(int CarNr, int Speed){ //change the speed bits and all the active word bits --> if you dont it will studder if you try to speedup
  int CarID = IDtoCar(CarNr);
  if (Speed==0){
    changeBits(CarID, Speed, 0b1, 0b1111);  
    changeBits(2, 0 , 6-CarNr ,0b1);
    changeBits(8, 0, 6-CarNr ,0b1);
    changeBits(2, 0 , 0 ,0b1);
    changeBits(8, 0, 0 ,0b1);
  }
  else{
    changeBits(CarID, Speed, 0b1, 0b1111);
    changeBits(2, 1, 6-CarNr ,0b1);
    changeBits(8, 1 , 6-CarNr ,0b1);
    changeBits(2, 1 , 0 ,0b1);
    changeBits(8, 1, 0 ,0b1);
  }
}
void CarreraControll::driveAll(int speed){
  for(int i=0; i<=5; i++){
    drive(i,speed);
  }
}


void CarreraControll::program(int carNr, int maxSpeed, int brake, int fuel){
  if (programmed){
    changeList(0, maxSpeed, carNr);
    changeList(1, brake, carNr);
    changeList(2, fuel, carNr);
    setID(carNr);
    programmed=false;
  }
  

}
bool CarreraControll::program(){
  if (setID() & !programmed){
    programmed=true;
    return true;
  }
  else
  return false;
  
}

void CarreraControll::changeList(int Nr, int value, int carNr){ //0 ist speed 1 is break 2 is fuel
  values[Nr] &= ~(0b1111 << 8);
  values[Nr] |= (reverseBits(value,4) << 8);
  values[Nr] &= ~(0b111 << 0);
  values[Nr] |= (reverseBits(carNr,3) << 0);
}

int CarreraControll::reverseBits(int value, int length) { 
    int reversed = 0;
    for (int i = 0; i < length; ++i) {
        reversed <<= 1;           
        reversed |= (value & 1);    
        value >>= 1;                
    }
    return reversed;
}

