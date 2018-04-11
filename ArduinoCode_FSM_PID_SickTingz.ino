//=== \/ Initilise \/ ===========================

#include <PID_v1.h>
//#include <TFMini.h>
//TFMini tfmini;

const int topPlateSwitchesPin = 8; // switches at top of device
const int goPin = 6; // switch at base of robot
const int safetySwitch = 6; // toggle to allow any movement in the robot. when high, setPoint = 220.
int state = 0;
int numOfMoves = 0;
double setPoint = 220;
int timeAtStartOfPause = 0;
int timeCheckedInPause = 0;
bool pauseComplete = false;
const double Kp = 5, Ki = 0, Kd = 0;
double motorSpeed;
double distanceReading = 0;
unsigned int t1;
unsigned int t2;
PID myPID(&distanceReading, &motorSpeed, &setPoint, Kp, Ki, Kd, REVERSE);

const int motorADirection1 = 5;
const int motorADirection2 = 3;
const int motorAEnable = 2;

const int motorBDirection1 = 13;
const int motorBDirection2 = 11;
const int motorBEnable = 12;
//=== /\ initilise /\ ===========================
//=== \/ setUp \/ ===============================

void setup() {
  Serial.begin(9600);
  Serial1.begin(115200);
  pinMode(goPin, INPUT);
  pinMode(topPlateSwitchesPin, INPUT);
  pinMode(goPin, INPUT);
  pinMode(topPlateSwitchesPin, INPUT);

  pinMode(motorADirection1, OUTPUT);
  pinMode(motorADirection2, OUTPUT);
  pinMode(motorAEnable, OUTPUT);

  pinMode(motorBDirection1, OUTPUT);
  pinMode(motorBDirection2, OUTPUT);
  pinMode(motorBEnable, OUTPUT);

  myPID.SetSampleTime(200);
  myPID.SetMode(AUTOMATIC);

  digitalWrite(motorAEnable, HIGH);//enables motor A
  digitalWrite(motorBEnable, HIGH);//enables motor B
}

//=== /\ setUp /\ ==============================
//=== \/ stateMachine \/ =======================

void loop() {
  Serial.print("State at interation: ");
  Serial.println(state);
  Serial.print("setPoint at iteration: ");
  Serial.println(setPoint);
  if (digitalRead(safetySwitch) == LOW){
    if (state == 0) { // Ready State
      if (digitalRead(goPin) == LOW) {
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
  } else {
    setPoint = 220;
    movement();
  }
  //delay(500);  //delay for trouble shooting
}

//=== /\ stateMachine /\ =======================
//=== \/ functions \/ ==========================

void movement() {
  distanceReading = double(readSensor());
  if (distanceReading > setPoint) {
    myPID.SetControllerDirection(REVERSE);
    myPID.Compute();
    analogWrite(motorADirection1, abs(motorSpeed)); //Drives motor A forward at rate of Output
    digitalWrite(motorADirection2, LOW);
    analogWrite(motorBDirection1, abs(motorSpeed)); //Drives motor B forward at rate of Output
    digitalWrite(motorBDirection2, LOW);
  } else if (distanceReading < setPoint) {
    myPID.SetControllerDirection(DIRECT);
    myPID.Compute();
    digitalWrite(motorADirection1, LOW);
    analogWrite(motorADirection2, abs(motorSpeed)); //Drives motor A backwards at rate of Output
    digitalWrite(motorBDirection1, LOW);
    analogWrite(motorBDirection2, abs(motorSpeed)); //Drives motor B forward at rate of Output
  } else {
    analogWrite(motorADirection1, HIGH); //drives wheels in both directions
    digitalWrite(motorADirection2, HIGH);//drives wheels in both directions
    analogWrite(motorBDirection1, HIGH); //drives wheels in both directions
    digitalWrite(motorBDirection2, HIGH);//drives wheels in both directions
  }
  Serial.print("distanceReading: ");
  Serial.println(distanceReading);
  Serial.print("PID Output: ");
  Serial.println(motorSpeed);
  Serial.println("");
}

bool movementComplete() {
SafetyLoop:
  if (setPoint == 0) {
    if (digitalRead(topPlateSwitchesPin) == LOW) {
      return true;
    } else {
      return false;
    }
  } else if ((setPoint == 110) || (setPoint == 220)) {
    if (distanceReading == setPoint) {
      return true;
    } else {
      return false;
    }
  } else {
    setPoint = 220;
    goto SafetyLoop;
  }
}

int readSensor() {
  while (Serial1.available() >= 9) {
    if ((0x59 == Serial1.read()) && (0x59 == Serial1.read())) { //Byte1 & Byte2
      t1 = Serial1.read(); //Byte3
      t2 = Serial1.read(); //Byte4
      t2 <<= 8;
      t2 += t1;
    }
  }
  return t1;
}


