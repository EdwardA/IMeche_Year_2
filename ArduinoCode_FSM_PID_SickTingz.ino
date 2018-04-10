//=== \/ Initilise \/ ===========================
// CHANGE PIN NUMBERS! THESE ARE PLACEHOLDERS
int topPlateSwitchesPin = 1;
int goPin = 2;
int LiDARPin = 3;
int LiDARSensorValue = 0;
int state = 0;
int numOfMoves = 0;
int setPoint = 220;
int timeAtStartOfPause = 0;
int timeCheckedInPause = 0;
bool pauseComplete = false;

//=== /\ initilise /\ ===========================
//=== \/ setUp \/ ===============================

void setup() {
  for (int i = 2; i < 14; i++) {
    pinMode(i, OUTPUT);
  }
  pinMode(goPin, INPUT_PULLUP);
  pinMode(topPlateSwitchesPin, INPUT_PULLUP);
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

