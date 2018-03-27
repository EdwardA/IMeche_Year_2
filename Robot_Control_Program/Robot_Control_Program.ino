#include <PID_v.1h>

const int START = 6;     //Attached switch to start the robot
int START_state = 0;     //Stored state for the start condition
const int TRIG = 8;      //Attached micro to indicated top of tube
int TRIG_state = 0;      //Stored state for the trig condition

const int INPUT1 = 5;    //Motor A direction 1
const int INPUT2 = 3;    //Motor A direction 2
const int ENABLEA = 2;   //Motor A enable (enabled when high)

const int INPUT3 = 10;   //Motor B direction 1
const int INPUT4 = 11;   //Motor B direction 2
const int ENABLEB = 12;  //Motor B enable (enabled when high)

void setup() {
Serial.begin(9600);      //Initatate comunication between robot and computer via USB
Serial1.begin(115200);   //Initated comunication between robot and sensor

pinMode(START, INPUT);
pinMode(TRIG, INPUT);

pinMode(INPUT1, OUTPUT);
pinMode(INPUT2, OUTPUT);
pinMode(ENABLEA, OUTPUT);

pinMode(INPUT3, OUTPUT);
pinMode(INPUT4, OUTPUT);
pinMode(ENABLEB, OUTPUT);

}

void loop() {
  START_state = digitalRead(START);
  TRIG_state = digitalRead(TRIG);

}
