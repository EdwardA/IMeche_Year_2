//=== \/ Initilise \/ ===========================
//Libraries
#include <PID_v.1h>

// CHANGE PIN NUMBERS! THESE ARE PLACEHOLDERS
//int topPlateSwitchesPin = 8;
//int goPin = 6;
//int LiDARPin = 3;
//int LiDARSensorValue = 0;
//int state = 0;
//int numOfMoves = 0;
//int setPoint = 220;
//int timeAtStartOfPause = 0;
//int timeCheckedInPause = 0;
//bool pauseComplete = false;

//=== /\ initilise /\ ===========================

const int START = 6;     //Attached switch to start the robot
int START_state = 0;     //Stored state for the start condition
const int TRIG = 8;      //Attached micro to indicated top of tube
int TRIG_state = 0;      //Stored state for the trig condition

const int INPUT1 = 5;    //Motor A direction 1
const int INPUT2 = 3;    //Motor A direction 2
const int ENABLEA = 2;   //Motor A enable (enabled when high)

const int INPUT3 = 10;   //Motor B direction 1
const int INPUT4 = 11;   //Motor B direction 2
const int ENABLEB = 12; //Motor B enable (enabled when high)

//sensor read
unsigned char dta[100];
unsigned char len = 0;
unsigned int t1;
unsigned int distance;


//PID Setup
int Setpoint = 0;
double Kp=20, Ki=5, Kd=10;
PID myPID(&Input, &Output, &Setpoint, Kp, Ki, Kd, DIRECT);


//=== \/ setUp \/ ===============================

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

myPID.SetMode(AUTOMATIC);
}


//=== /\ setUp /\ ==============================
//=== \/ stateMachine \/ =======================

void loop() {
  if (state == 0) { // Ready State
    if (goPin == LOW) {
      state = 1;
    }
    movement();
  } else if (state == 1) { // Done State
    pauseComplete = false;
    if (numOfMoves == 0) { // Last: N/A. Next: full UP
      setPoint = 0;
    } else if (numOfMoves == 1) { // Last: full UP. Next: full DOWN.
      setPoint = 220;
    } else if (numOfMoves == 2) { // Last: full DOWN. Next: half UP.
      setPoint = 110;
    } else if (numOfMoves == 3) { // Last: half UP. Next: half UP.
      setPoint = 0;
    } else if (numOfMoves == 4) { // Last: half UP. Next: half DOWN.
      setPoint = 110;
    } else if (numOfMoves == 5) { // Last: half DOWN. Next half DOWN.
      setPoint = 220;
    } else if (numOfMoves == 6) { // Last: half DOWN. Next: half UP.
      setPoint = 110;
    } else {
      setPoint = 220;
    }
    state = 2;
  } else if (state == 2) { // Moving State
    if (movementComplete() == true) {
      if ((numOfMoves == 2) || (numOfMoves == 4) || (numOfMoves == 6)) {
        if (pauseComplete == false) {
          timeAtStartOfPause = millis();
          state = 3;
        } else {
          numOfMoves ++;
          state = 1;
        }
      } else {
        numOfMoves ++;
        state = 1;
      }
    } else {
      movement();
    }
  } else if (state == 3) { // 7s Pause State
    timeCheckedInPause = millis();
    if (timeCheckedInPause < (timeAtStartOfPause + 7000)) {
      movement();
    } else {
      pauseComplete = true;
      state = 2;
    }
  }
//======= \/ sensor read and manipulate (feel free to move if needed) \/ =======
while(Serial1.available()>=9)
    {
        if((0x59 == Serial1.read()) && (0x59 == Serial1.read())) //Byte1 & Byte2
        {
            t1 = Serial1.read(); //Byte3
            distance = Serial1.read(); //Byte4

            distance <<= 8;
            distance += t1;
            //Serial.println(distance);
        }
    }
    Serial.println(distance);

  
}

//=== /\ stateMachine /\ =======================
//=== \/ functions \/ ==========================
// MOVEMENT NEEDS TO BE WRITEN! FUNCTION HAS A DEFINITON OF PURPOSE:

void movement() {
  /*  move with PID to the setpoint.
      select direction based on value from sensor and setpoint given.
      (setPoint is a global variable (the reason I asked about efficiency))
      do not worry about stopping conditions here (see movementComplete())
  */

  Input = analogRead(distance);
  myPID.Compute();
  Serial.println(Output);
  if (Output > 0){
//======Drives forward==========
    digitalWrite(ENABLEA, HIGH);//enables motor A
    digitalWrite(ENABLEB, HIGH);//enables motor B
    
    analogWrite(INPUT1, Output); //Drives motor A forward at rate of Output
    digitalWrite(INPUT2, LOW);

    analogWrite(INPUT3, Output); //Drives motor B forward at rate of Output
    digitalWrite(INPUT4, LOW);
    else if(Output < 0){
//======Drives backwards==========
      digitalWrite(ENABLEA, HIGH);//enables motor A
      digitalWrite(ENABLEB, HIGH);//enables motor B
      
      digitalWrite(INPUT1, LOW);
      analogWrite(INPUT2, abs Output); //Drives motor A backwards at rate of Output

      digitalWrite(INPUT3, LOW);
      analogWrite(INPUT4, abs Output); //Drives motor B forward at rate of Output
    }else{
//==============Brakes============
    digitalWrite(ENABLEA, HIGH);//enables motor A
    digitalWrite(ENABLEB, HIGH);//enables motor B
    
    analogWrite(INPUT1, HIGH); //drives wheels in both directions
    digitalWrite(INPUT2, HIGH);//drives wheels in both directions

    analogWrite(INPUT3, HIGH); //drives wheels in both directions
    digitalWrite(INPUT4, HIGH);//drives wheels in both directions
    }
  }

  /*
   * Setup for the movement function. compare to the PID_Basic example sketch if you want to call other functions within the functions sketch 
   */
}

bool movementComplete() {
SafetyLoop:
  if (setPoint == 0) {
    if (topPlateSwitchesPin == LOW) {
      return true;
    } else {
      return false;
    }
  } else if ((setPoint == 110) || (setPoint == 220)) {
    if (LiDARSensorValue = setPoint) {
      return true;
    } else {
      return false;
    }
  } else {
    setPoint = 220;
    goto SafetyLoop;
  }
}

