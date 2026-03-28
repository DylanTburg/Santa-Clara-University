#include <Servo.h>
#include <QTRSensors.h>
#include <Wire.h>
#include "Adafruit_TCS34725.h"

// Motor A (left)
#define PIN_EN_A 6  //change to whatever
#define PIN_IN1 34
#define PIN_IN2 36

// Motor B (right)
#define PIN_EN_B 7  //change to whatever
#define PIN_IN3 35
#define PIN_IN4 37

// Encoder
#define PIN_CHANNEL_A 2
#define PIN_CHANNEL_B 3


#define COUNTS_PER_REV 1600 //900 measured from previous, but now we double count
                            // with both rising and falling edge checking


uint8_t sensorPin[8] = {22,23,24,25,26,27,28,29};


#define LED_PIN 13
#define START_BUTTON_PIN 40  // CHANGE to actual pin


// Line following PD
int basePWM = 170;
int backPWM = 110;
float Kp = 0.1f; //proportional || used for small changes and normal line
float Kd = 0.00f; //derivative || used for sharp turns and corners || didnt end up needing


// robot dimensions
#define WHEEL_DIA 60.0f
#define WHEELBASE 200.0f


float COUNTS_PER_CM = (float)COUNTS_PER_REV / (PI * (WHEEL_DIA / 10.0f));
#define COUNTS_90_PIVOT ((long)((WHEELBASE * (float)COUNTS_PER_REV) / (4.0f * WHEEL_DIA)))
#define COUNTS_15_PIVOT (long) (COUNTS_90_PIVOT/6)
#define COUNTS_10_PIVOT (long) (COUNTS_90_PIVOT/9)

#define LINE_LOST_FRAMES 60 // Amount of lost frames before line is officially lost | TUNE
#define SLOW_PWM 170
#define BUTTON_PUSH_TIME_MS 2500


// Fill detection (delta lux from baseline when glass is full)
#define FILL_DELTA_LUX_TAP_1 -10 // TUNE | YELLOW
#define FILL_DELTA_LUX_TAP_2 40 // TUNE | BLUE
#define FILL_DELTA_LUX_TAP_3 44 // TUNE | RED
#define FILL_DELTA_LUX_TAP_4 10 // TUNE | GREEN


// Tap distances from order board (cm) | May need 4-5cm less on TAPs
#define BUTTON_BACKUP_CM 26
#define TAP_APPROACH_CM 20
#define TAP_1_DIST_CM 4 //YELLOW
#define TAP_2_DIST_CM 24 //BLUE
#define TAP_3_DIST_CM 44 //RED
#define TAP_4_DIST_CM 65 //GREEN

#define RUN_TIME_MS 60000UL
#define MAX_BEERS 4

#define SERVO_PIN 9
#define SERVO_RIGHT_ANG 0
#define SERVO_LEFT_ANG 45

Servo myservo;
int pourStarted = 0;

// Color ratios (R/C, G/C, B/C)
/*#define RED_R_MIN 0.75f
#define RED_G_MAX 0.1f
#define RED_B_MAX 0.15f

#define BLUE_B_MIN 0.60f
#define BLUE_G_MAX 0.35f
#define BLUE_R_MAX 0.1f

#define GREEN_G_MIN 0.45f
#define GREEN_R_MAX 0.25f
#define GREEN_B_MAX 0.25f

#define YELLOW_R_MIN 0.42f
#define YELLOW_G_MIN 0.3f
#define YELLOW_B_MAX 0.17f */

#define RED_R_MIN 0.50f
#define RED_G_MAX 0.2f
#define RED_B_MAX 0.2f

#define BLUE_B_MIN 0.50f
#define BLUE_G_MAX 0.35f 
#define BLUE_R_MAX 0.2f

#define GREEN_G_MIN 0.45f
#define GREEN_R_MAX 0.28f
#define GREEN_B_MAX 0.25f

#define YELLOW_R_MIN 0.42f
#define YELLOW_G_MIN 0.3f
#define YELLOW_B_MAX 0.2f

enum State {
  CALIBRATE,
  STREET_1,
  LEFT_PIVOT,
  TURN_1_SEEK,
  STREET_2,
  RIGHT_PIVOT,
  TURN_2_SEEK,
  STREET_3,
  APPROACH_BUTTON,
  BACK_OFF_BUTTON,
  READ_ORDER,
  DRIVE_TO_TAP,
  APPROACH_TAP,
  AT_TAP,
  BACK_FROM_TAP,
  RETURN_TO_READ,
  CELEBRATE,
  DONE
};

State currentState = CALIBRATE;

volatile int flag = 0;
volatile long count = 0;
long recentCount = 0;

QTRSensors qtr;
uint16_t raw[8];
int lastError = 0;
int lastPos = 3500;
int lineLostCount = 0;
int goneOnce = 0;


Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_24MS, TCS34725_GAIN_16X);


unsigned long runStartTime = 0;
int runTimerStarted = 0;


int beersServed = 0;
int currentOrder = -1; // 0=yellow, 1=blue, 2=red, 3=green
int currentTapDistCm = 0;
int baselineLux = 0;


long pivotStartCount = 0;
int turn1Count = 0;
int turn2Count = 0;
unsigned long stateStartTime = 0;


int branchSeen = 0;
int startedTapRun = 0;
long tapRunStartCount = 0; //startedTapRunCount
int branchCount = 0;


void isr0() {
  int a = digitalRead(PIN_CHANNEL_A);
  int b = digitalRead(PIN_CHANNEL_B);
  if (a==b) count++;
  else count--;
  flag=1;
}


int clip(int x) {
  if (x<-255) return -255;
  if (x>255) return 255;
  return x;
}


void setMotors(int leftPWM, int rightPWM) {
  leftPWM = clip(leftPWM);
  rightPWM = clip(rightPWM);


  if(leftPWM >= 0){
    digitalWrite(PIN_IN1, HIGH);
    digitalWrite(PIN_IN2, LOW);
  }
  else {
    digitalWrite(PIN_IN1, LOW);
    digitalWrite(PIN_IN2, HIGH);
    leftPWM = -1 * leftPWM;
  }


  if(rightPWM >= 0){
    digitalWrite(PIN_IN3, LOW);
    digitalWrite(PIN_IN4, HIGH);
  }
  else {
    digitalWrite(PIN_IN3, HIGH);
    digitalWrite(PIN_IN4, LOW);
    rightPWM = -1 * rightPWM;
  }


  analogWrite(PIN_EN_A, leftPWM);
  analogWrite(PIN_EN_B, rightPWM);
}


void stopMotors() {
  analogWrite(PIN_EN_A, 0);
  analogWrite(PIN_EN_B, 0);
  digitalWrite(PIN_IN1, LOW);
  digitalWrite(PIN_IN2, LOW);
  digitalWrite(PIN_IN3, LOW);
  digitalWrite(PIN_IN4, LOW);
}


void readLineSensors() {
  qtr.readCalibrated(raw);
}


int lineDetected() {
  for (int i=0; i<8; i++) if (raw[i] > 750) return 1;
  return 0;
}


int linePosBlack() {
  long numer = 0, denom = 0;
  for (int i=0; i<8; i++) {
    if (raw[i] > 750) {
      numer += (long)i * 1000; //0-7000
      denom += 1;
    }
  }
  if (denom == 0) return (lastPos < 3500) ? 0 : 7000;
  lastPos = (int) (numer/denom);
  return lastPos;
}


void lineFollowUpdate() {
  int pos = linePosBlack();
  int error = pos - 3500; // 3500 = center
  int deriv = error - lastError;
  int correction = (int)(Kp * error + Kd * deriv);
  lastError = error;


  int leftPWM = basePWM + correction; // if it steers the wrong way, swap +/-
  int rightPWM = basePWM - correction;
  setMotors(leftPWM, rightPWM);
}


// 0=yellow, 1=blue, 2=red, 3=green, -1=unknown
int readOrderColor() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  if (c < 20) return -1;
  float rR = (float)r / (float)c;
  float gR = (float)g / (float)c;
  float bR = (float)b / (float)c;


  if (rR > RED_R_MIN && gR < RED_G_MAX && bR < RED_B_MAX) return 2;
  if (bR > BLUE_B_MIN && rR < BLUE_R_MAX) return 1;
  if (gR > GREEN_G_MIN && rR < GREEN_R_MAX && bR < GREEN_B_MAX) return 3;
  if (rR > YELLOW_R_MIN && gR > YELLOW_G_MIN && bR < YELLOW_B_MAX) return 0;
  return -1;
}


int targetBranch(int color) {
  switch(color) {
    case 0: return 1; // yellow
    case 1: return 2; // blue
    case 2: return 3; // red
    case 3: return 4; // green
    default: return 1;
  }
}


int leftBranchDetected() {
  int leftHits = 0;
  int mainHits = 0;
  if (raw[0] > 750) leftHits++;
  if (raw[1] > 750) leftHits++;
  if (raw[2] > 750) leftHits++;
  if (raw[3] > 750) mainHits++;
  if (raw[4] > 750) mainHits++;
  if (raw[5] > 750) mainHits++;
  if (raw[6] > 750) mainHits++;
  return (leftHits >= 2 && mainHits >= 2);
}


void lineFollowCmForwardNoBranch(int dist) {
  long start = count;
  long goal = (long) round(dist * COUNTS_PER_CM);
  while(labs(count-start) < goal) {
    readLineSensors();
    if (lineDetected()) {
      lineLostCount = 0;
      lineFollowUpdate();
    }
    else {
      lineLostCount++;
      setMotors(basePWM, basePWM);
    }
  }
  stopMotors();
}


int readLux() {
  uint16_t r, g, b, c;
  tcs.getRawData(&r, &g, &b, &c);
  return tcs.calculateLux(r, g, b);
}


int getFillDelta(int color) {
  switch(color) {
    case 0: return FILL_DELTA_LUX_TAP_1;
    case 1: return FILL_DELTA_LUX_TAP_2;
    case 2: return FILL_DELTA_LUX_TAP_3;
    case 3: return FILL_DELTA_LUX_TAP_4;
    default: return FILL_DELTA_LUX_TAP_1;
  }
}


bool runTimeUp() {
  if (!runTimerStarted) return false;
  return ((millis() - runStartTime) >= RUN_TIME_MS);
}


void cmForward(int dist) {
  long start = count;
  long goal = (long) round(dist * COUNTS_PER_CM);
  setMotors(basePWM, basePWM);
  while(labs(count-start) < goal) {}
  stopMotors();
}


void cmReverse(int dist) {
  long start = count;
  long goal = (long) round(dist * COUNTS_PER_CM);
  setMotors(-basePWM-25, -basePWM);
  while(labs(count-start) < goal) {}
  stopMotors();
}
void reverse(){
  setMotors(-basePWM-10, -basePWM);
}


// For longer drives, this keeps the robot on course
void lineFollowCmForward(int dist) {
  long start = count;
  long goal = (long) round(dist * COUNTS_PER_CM);
  while(labs(count-start) < goal) {
    readLineSensors();
    if (lineDetected()) {
      lineLostCount = 0;
      lineFollowUpdate();
    }
    else {
      lineLostCount++;
      if (lineLostCount >= LINE_LOST_FRAMES) {
        setMotors(SLOW_PWM, SLOW_PWM);
      } 
      else lineFollowUpdate();
    }
  }
  stopMotors();
}

void lineFollowReverseUpdate() {
  int pos = linePosBlack();
  int error = pos - 3500;
  int deriv = error - lastError;
  int correction = (int)(Kp * error + Kd * deriv);
  lastError = error;

  int leftPWM = -backPWM + correction;
  int rightPWM = -backPWM - correction;
  setMotors(leftPWM, rightPWM);
}

void lineFollowCmReverse(int dist) {
  long start = count;
  long goal = (long) round(dist * COUNTS_PER_CM);
  while(labs(count-start) < goal) {
    readLineSensors();
    if (lineDetected()) {
      lineLostCount = 0;
      lineFollowReverseUpdate();
    }
    else {
      lineLostCount++;
      if (lineLostCount >= LINE_LOST_FRAMES) {
        setMotors(-backPWM, -backPWM);
      }
      else lineFollowReverseUpdate();
    }
  }
  stopMotors();
}


void pivotRight15() {
  long start = count;
  while (labs(count - start) < COUNTS_15_PIVOT) {
    setMotors(basePWM, -basePWM);
  }
  stopMotors();
}

void pivotRight10() {
  long start = count;
  while (labs(count - start) < COUNTS_10_PIVOT) {
    setMotors(basePWM, -basePWM);
  }
  stopMotors();
}

void pivotRightTrial() {
  long start = count;
  while (labs(count - start) < (COUNTS_15_PIVOT-5)) {
    setMotors(basePWM, -basePWM);
  }
  stopMotors();
}


void pivotLeft15() {
  long start = count;
  while (labs(count - start) < COUNTS_15_PIVOT) {
    setMotors(-basePWM, basePWM);
  }
  stopMotors();
}

void pivotLeftTrial() {
  long start = count;
  while (labs(count - start) < (COUNTS_15_PIVOT - 2)) {
    setMotors(-basePWM, basePWM);
  }
  stopMotors();
}


int getTapDistance(int color) {
  switch(color) {
    case 0: return TAP_1_DIST_CM;
    case 1: return TAP_2_DIST_CM;
    case 2: return TAP_3_DIST_CM;
    case 3: return TAP_4_DIST_CM;
    default: return TAP_1_DIST_CM;
  }
}

/*
void pourSequence() {
  if (!pourStarted) {
    myservo.write(SERVO_LEFT_ANG);
    delay(1000);
    //myservo.write(SERVO_LEFT_ANG+10);
    cmReverse(10);
    pourStarted = 1;
  }

  int lux = readLux();
  int delta = lux - baselineLux;
  int threshold = getFillDelta(currentOrder);

  if (abs(delta) >= abs(threshold)) {

    lineFollowCmForward(20);
    delay(1000);
    myservo.write(SERVO_RIGHT_ANG);
    delay(400);

    pourStarted = 0;
    beersServed++;
    currentState = BACK_FROM_TAP;
  }
}
*/

int baselineLuxFlag = 0;
void pourSequence() {
  int pushDistance = 45;
  if (!pourStarted) {
    int servo = SERVO_LEFT_ANG;
    if(currentOrder==1){
      servo+=10;
      pushDistance+=15;
    }
    else if(currentOrder==2){
      servo+=10;
      pushDistance+=30;
    }
    else if(currentOrder==3){
      servo+=10;
    }
    else if(currentOrder==4){

    }
    else{
      servo+=10;
    }
    myservo.write(servo);
    delay(1000);
    // previously cmReverse(18);
    cmReverse(15);
    pourStarted = 1;

    // attempt to readjust before reading lux
    //pivotLeftTrial();
    //myservo.write(SERVO_LEFT_ANG);
  }

  if(!baselineLuxFlag){
    baselineLux = readLux();
    baselineLuxFlag = 1;
  }

  int lux = readLux();
  int delta = lux - baselineLux;
  int threshold = getFillDelta(currentOrder);

  // new time based pour
  unsigned long pourStart = millis();
  digitalWrite(LED_PIN, HIGH);

  //if (abs(delta) >= abs(threshold)) {
  //if(millis() - pourStart > 7000){
    delay(6500);
    digitalWrite(LED_PIN, LOW);
    setMotors(90, 90);
    unsigned long timeout = millis();
    while (millis() - timeout < 3000) {
      readLineSensors();
      if (lineDetected()) break;
      delay(5);
    }
    stopMotors();
    delay(200);

    setMotors(250, 250);
    //alignOnLine();
    basePWM = 254;
    // attempt to fix before push back
    // pivotRight15();
    // myservo.write(SERVO_LEFT_ANG);
    lineFollowCmForward(pushDistance); 
    //setMotors(250, 250);
    //cmForward(500); 
    delay(500);
    cmReverse(2);
    myservo.write(SERVO_RIGHT_ANG);
    delay(400);

    pourStarted = 0;
    beersServed++;
    baselineLuxFlag = 0;
    currentState = BACK_FROM_TAP;
  //}
}

void alignOnLine() {
  readLineSensors();

  if (!lineDetected()) {
    setMotors(90, 90);
    unsigned long timeout = millis();
    while (millis() - timeout < 2000) {
      readLineSensors();
      if (lineDetected()) break;
      delay(5);
    }
    stopMotors();
  }

  for (int i = 0; i < 200; i++) {
    readLineSensors();
    if (!lineDetected()) break;
    int pos = linePosBlack();
    if (abs(pos - 3500) < 500) break; 
    if (pos < 3500) setMotors(-60, 60);  
    else setMotors(60, -60);              
    delay(10);
  }
  stopMotors();
}
/*
void celebrationDance() {
  mario();
  setMotors(200, -200);  
  delay(400);
  setMotors(-200, 200);  
  delay(400);
  setMotors(255, -255);  
  delay(300);
  setMotors(-255, 255);  
  delay(300);
  for (int i=0; i<6; i++) {
    setMotors(150, -150);  
    delay(120);
    setMotors(-150, 150);  
    delay(120);
  }
  setMotors(255, -255);  
  delay(600);
  stopMotors();
  while (!runTimeUp()) {
    delay(50);
  }
} */


/* SPEAKER PASTED CODE */
#include <SPI.h>
#define PRINTRATE 167 // (1 / (60 samples per cycle * 40hz)) in us
#define PHASEINC ((64 * 256)/20)  /* 20 Samples per full cycle */
#define QUARTER_NOTE 400000L
#define CS 10
#define SQ_WAVE_PIN 8

#define A1 55 
#define A2 110 
#define A3 220
#define A4 440
#define B1 62 
#define B2 123 
#define B3 247
#define B4 494
#define C1 65 
#define C2 130 
#define C3 262
#define C4 524
#define D1 73 
#define D2 147 
#define D3 294
#define D4 588
#define E1 82 
#define E2 165 
#define E3 330
#define E4 660
#define F1 87 
#define F2 175 
#define F3 349
#define F4 698
#define G1 98 
#define G2 196 
#define G3 392
#define G4 784

#define AB4 831
#define BB4 932
#define C5 1047
#define D5 1175
#define EB5 1245
#define E5 1319
#define F5 1397
#define G5 1568
#define AB5 1661
#define BB5 1865
#define C6 2093
#define D6 2349
#define EB6 2489
#define E6 2637
#define F6 2794
#define G6 3136
#define AB6 3322
#define BB6 3729

int inval;
int phase;
long lastDACTime;
int waveTable[256];  



/* dacval must be between 0 and 1023 or there will be an error */
/* Vref is set at 4.096V */

void DACout(unsigned int dacval) {
  dacval = dacval*4 + 0x1000;
  digitalWrite(CS, LOW);
  SPI.transfer16(dacval); 
  digitalWrite(CS, HIGH);            
}

  
int wave(int phase) {
    if ((phase < 0) || (phase >= (256 * 64))) {
     return 0;
   }
   return waveTable[phase/64];  
}

void mytone (int freq, long tonelength){ // all in us
  if (freq <=0 || tonelength <=0) return;
  unsigned long halfPeriod = 1000000UL / (2UL*(unsigned long) (freq/2));
  unsigned long start = micros();
  while ((long)(micros() - start) < tonelength) {
    digitalWrite(SQ_WAVE_PIN, HIGH);
    delayMicroseconds(halfPeriod);
    digitalWrite(SQ_WAVE_PIN, LOW);
    delayMicroseconds(halfPeriod);
  }
}

//if pausing is needed
void pause(long pause){
  digitalWrite(SQ_WAVE_PIN, LOW);
  delayMicroseconds(pause);
}

void mario(){
  long NOTE = 270000L;

  setMotors(200, -200);
  mytone(G4, NOTE); 
  pause(15000);
  mytone(C5, NOTE); 
  pause(15000);
  mytone(E5, NOTE); 
  pause(15000);
  mytone(G5, NOTE); 
  pause(15000);
  mytone(C6, NOTE); 
  pause(15000);
  mytone(E6, NOTE); 
  pause(15000);
  mytone(G6, NOTE); 
  pause(15000);
  mytone(E6, NOTE); 
  pause(30000);


  setMotors(-200, 200);
  mytone(AB4, NOTE); 
  pause(15000);
  mytone(C5, NOTE); 
  pause(15000);
  mytone(EB5, NOTE); 
  pause(15000);
  mytone(AB5, NOTE); 
  pause(15000);
  mytone(C6, NOTE); 
  pause(15000);
  mytone(EB6, NOTE); 
  pause(15000);
  mytone(AB6, NOTE); 
  pause(15000);
  mytone(EB6, NOTE); 
  pause(30000);


  setMotors(255, -255);
  mytone(BB4, NOTE); 
  pause(15000);
  mytone(D5, NOTE); 
  pause(15000);
  mytone(F5, NOTE); 
  pause(15000);

  setMotors(-255, 255);
  mytone(BB5, NOTE); 
  pause(15000);
  mytone(D6, NOTE); 
  pause(15000);
  mytone(F6, NOTE); 
  pause(15000);

  setMotors(255, -255);
  mytone(BB6, NOTE); 
  pause(15000);

  setMotors(-255, 255);
  mytone(BB6, NOTE); 
  pause(15000);

  setMotors(255, -255);
  mytone(BB6, NOTE); 
  pause(15000);

  setMotors(255, 255);
  mytone(C6*2, 2 * NOTE); 
  pause(60000);
  stopMotors();
}


/* SPEAKER PASTED CODE*/

void setup() {
  pinMode(PIN_EN_A, OUTPUT);
  pinMode(PIN_IN1, OUTPUT);
  pinMode(PIN_IN2, OUTPUT);
  pinMode(PIN_EN_B, OUTPUT);
  pinMode(PIN_IN3, OUTPUT);
  pinMode(PIN_IN4, OUTPUT);
  stopMotors();


  pinMode(PIN_CHANNEL_A, INPUT_PULLUP);
  pinMode(PIN_CHANNEL_B, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_CHANNEL_A), isr0, CHANGE);


  qtr.setTypeRC();
  qtr.setSensorPins(sensorPin, 8);
  qtr.setTimeout(2500); //us


  if (!tcs.begin()) {
    while (1); // sensor not found
  }


  pinMode(LED_PIN, OUTPUT);
  pinMode(START_BUTTON_PIN, INPUT_PULLUP);

  myservo.attach(SERVO_PIN);
  myservo.write(SERVO_RIGHT_ANG);

  phase = 0;
  lastDACTime = 0;
  init_waveTable();
  SPI.begin();   // initiate SPI as controller
  SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0)) ;
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);

  pinMode(SQ_WAVE_PIN, OUTPUT);
  digitalWrite(SQ_WAVE_PIN, LOW);

  //Serial.begin(9600);
  //currentState = BACK_OFF_BUTTON;
  currentState = CALIBRATE;
  //currentState = CELEBRATE;
}


void loop() {


  if (runTimerStarted && runTimeUp() && currentState != DONE && currentState != CELEBRATE) {
    stopMotors();
    if(currentState == AT_TAP || currentState == APPROACH_TAP || currentState == BACK_FROM_TAP){
      myservo.write(SERVO_RIGHT_ANG);
      cmReverse(15);
    }
    currentState = CELEBRATE;
  }

  switch(currentState) {
    // Auto-calibration | use lab 8 procedure to remove if necessary
    case CALIBRATE: {
      while (digitalRead(START_BUTTON_PIN) == HIGH) {}
      delay(30);
      while (digitalRead(START_BUTTON_PIN) == LOW) {}
      delay(30);
      //delay(3000);
      digitalWrite(LED_PIN, HIGH);
      delay(1000);
     
      setMotors(-100, 100);
      for(int i=0; i < 30; i++) {
        qtr.calibrate();
        delay(2);
      }
      setMotors(100, -100);
      for(int i=0; i < 60; i++) {
        qtr.calibrate();
        delay(2);
      }
      setMotors(-100, 100);
      for(int i=0; i < 30; i++) {
        qtr.calibrate();
        delay(2);
      }


      stopMotors();
      delay(2000);
      digitalWrite(LED_PIN, LOW);
      lineLostCount = 0;
      lastError = 0;
      lastPos = 3500;
      currentState = STREET_1;
      //turn2Count = 2; //REMOVE LATER
      //turn1Count = 2; //REMOVE LATER
      //currentState = STREET_3;
      //currentState = CELEBRATE;
      break;
     
    }


    case STREET_1: {
      readLineSensors();
      if (lineDetected()) {
        lineLostCount = 0;
        lineFollowUpdate();
      }
      else {
        lineLostCount++;
        if (lineLostCount >= LINE_LOST_FRAMES) {
          stopMotors();
          pivotStartCount = count;
          lineLostCount = 0;
          currentState = LEFT_PIVOT;
        }
        else setMotors(basePWM, basePWM);
      }
      delay(10);
      break;
    }


    case LEFT_PIVOT: {
      setMotors(-basePWM, basePWM);
      if (labs(count - pivotStartCount) >= COUNTS_90_PIVOT) {
        stopMotors();
        turn1Count++;
        if(turn1Count == 1) currentState = TURN_1_SEEK;
        else if(turn1Count == 2) currentState = STREET_2;
        else if(turn1Count % 2 == 1) {
          currentState = APPROACH_TAP;
          delay(200);
          while (1) {
              readLineSensors();
              if (lineDetected()) break;
              setMotors(-basePWM, basePWM);
              delay(20);
            }
            stopMotors();
          }
        else {
          currentState = RETURN_TO_READ;
          delay(200);
          while (1) {
              readLineSensors();
              if (lineDetected()) break;
              setMotors(-basePWM, basePWM);
              delay(20);
            }
            stopMotors();
        }
      }
      break;
    }


    case TURN_1_SEEK: {
      setMotors(SLOW_PWM-60, SLOW_PWM+60);
      readLineSensors();
      if (lineDetected()) {
        delay(150);
        stopMotors();
        lastError = 0;
        lineLostCount = 0;
        pivotStartCount = count;
        currentState = LEFT_PIVOT;


        //currentState = STREET_2;
       
      }
      delay(10);
      break;
    }


    case STREET_2: {
      Kd= 0.2f;
      readLineSensors();
      if (lineDetected()) {
        lineLostCount = 0;
        lineFollowUpdate();
      }
      else {
        lineLostCount++;
        if (lineLostCount >= LINE_LOST_FRAMES) {
          stopMotors();
          pivotStartCount = count;
          lineLostCount = 0;
          currentState = RIGHT_PIVOT;
        }
        else setMotors(basePWM, basePWM);
      }
      delay(10);
      break;
    }


    case RIGHT_PIVOT: {
      setMotors(basePWM, -basePWM);
      if (labs(count - pivotStartCount) >= COUNTS_90_PIVOT) {
        stopMotors();
        turn2Count++;
        if(turn2Count == 1) currentState = TURN_2_SEEK;
        else if(turn2Count == 2) currentState = STREET_3;
        else if(turn2Count % 2 == 1) {
          currentState = DRIVE_TO_TAP;
          while (1) {
            readLineSensors();
            if (lineDetected()) break;
            setMotors(basePWM, -basePWM);
            delay(20);
          }
          stopMotors();
        }
        else currentState = READ_ORDER;
      }
      break;
    }


    case TURN_2_SEEK: {
      setMotors(SLOW_PWM+60, SLOW_PWM-70);
      readLineSensors();
      if (lineDetected()) {
        delay(200);
        stopMotors();
        lastError = 0;
        lineLostCount = 0;
        pivotStartCount = count;
        currentState = RIGHT_PIVOT;
      }
      delay(10);
      break;
    }


    // PD handles the lined turn at top
    case STREET_3: {
      readLineSensors();
      if (lineDetected()) {
        lineLostCount = 0;
        lineFollowUpdate();
        if(raw[6]+raw[5]+raw[4]+raw[3]+raw[2]+raw[1]+raw[0] > 5400)  {
          stateStartTime = millis();
          currentState = APPROACH_BUTTON;
        }
      }
      else {
        lineLostCount++;
        if (lineLostCount >= LINE_LOST_FRAMES) { // Line Shouldn't be lost ever
          stopMotors();
          lineLostCount = 0;
          stateStartTime = millis();
          currentState = APPROACH_BUTTON;
        }
        else lineFollowUpdate();
      }
      delay(10);
      break;
    }


    case APPROACH_BUTTON: {
      setMotors(SLOW_PWM, SLOW_PWM);
      //Kd=.02f;
      lineFollowCmForward(27);
      /*if (millis() - stateStartTime > BUTTON_PUSH_TIME_MS) {
        stopMotors();
        runStartTime = millis();
        runTimerStarted = 1;
        delay(300);
        currentState = BACK_OFF_BUTTON;
      }*/
      stopMotors();
      delay(300);
      runStartTime = millis();
      runTimerStarted = 1;
      currentState = BACK_OFF_BUTTON;
      break;
    }


    /*
    case BACK_OFF_BUTTON: {
      cmReverse(BUTTON_BACKUP_CM);
      delay(200);
      currentState = READ_ORDER;
      break;
    }
    */
    case BACK_OFF_BUTTON: {
      if(!goneOnce){
        setMotors(-175, -120);
        delay(300);
        while (1) {
          readLineSensors();
          int hits = 0;
          for (int i = 0; i < 8; i++) if (raw[i] > 750) hits++;
          if (hits >= 5) break; 
        }
        stopMotors();
        delay(200);
        currentState = READ_ORDER;
        break;
      }
      else{
        cmReverse(BUTTON_BACKUP_CM);
        delay(200);
        currentState = READ_ORDER;
        break;
      }
    }

    case READ_ORDER: {
      Kp=0.1f;
      Kd = 0.0f;
      basePWM=170;
      //Kd=.05f;
      if(!goneOnce) pivotRight15();
      else pivotRight10();
      //if(goneOnce) pivotRight15();
      cmForward(24-goneOnce);
      //cmForward(25-goneOnce);
      stopMotors();
      //pivotLeft15();

      int votes[4] = {0, 0, 0, 0};
      for (int sample = 0; sample < 7; sample++) {
        int color = readOrderColor();
        if (color >= 0 && color <= 3) votes[color]++;
        delay(30);
      }
      

      int bestColor = -1;
      int bestVotes = 0;
      for (int i = 0; i < 4; i++) {
        if (votes[i] > bestVotes) {
          bestVotes = votes[i];
          bestColor = i;
        }
      }
      

      // debug
      /* if (bestColor < 0 || bestVotes < 3) {
        delay(300);
        break;
      }*/


      currentOrder = bestColor;
      // 1=yellow, 2=blue, 3=red, 4=green
      for (int i = 0; i <= currentOrder; i++) {
        digitalWrite(LED_PIN, HIGH);
        delay(250);
        digitalWrite(LED_PIN, LOW);
        delay(250);
      }
      delay(1000);
      //Serial.println(bestColor);
      currentTapDistCm = getTapDistance(currentOrder);

      startedTapRun = 0;
      branchSeen = 0;
      /*
      reverse();
      while (1) {
        readLineSensors();
        if ((raw[7] > 750 || raw[6] > 750) && (raw[0] > 750 || raw[1] > 750)) break;
      }
      stopMotors();
      delay(100);
      pivotStartCount = count;
      currentState = RIGHT_PIVOT;
      */
      // attempt to fix back into corner after read
      if(!goneOnce) pivotLeft15();
      cmReverse(23-goneOnce);
      // end of attempt
      //cmReverse(25);
      stopMotors();
      pivotStartCount = count;
      currentState = RIGHT_PIVOT;
      break;
    }

    case DRIVE_TO_TAP: {
      if (!startedTapRun) {
        lineFollowCmForwardNoBranch(7);
        branchCount = 0;
        branchSeen = 0;
        startedTapRun = 1;
      }

      readLineSensors();

      if (lineDetected()) {
        lineLostCount = 0;
        lineFollowUpdate();

        if (leftBranchDetected()) {
          if (!branchSeen) {
            branchSeen = 1;
            branchCount++;
          }
        } 
        else {
          if (branchSeen) {
            branchSeen = 0;
            if (branchCount >= targetBranch(currentOrder)) {
              stopMotors();
              delay(100);
              pivotStartCount = count;
              startedTapRun = 0;
              currentState = LEFT_PIVOT;
            }
          }
        }
      }
      else {
        lineLostCount++;
        if (lineLostCount >= LINE_LOST_FRAMES-45) {
          stopMotors();
          lineLostCount = 0;
          //If we've seen 3 branches and lost the line, we're at green
          if (branchCount >= 3 && targetBranch(currentOrder) == 4) {
            pivotStartCount = count;
            startedTapRun = 0;
            currentState = LEFT_PIVOT;
          }
        }
        else setMotors(basePWM, basePWM);
      }
      delay(10);
      break;
    }

    case APPROACH_TAP: {
      Kp = 0.05f;
      Kd = 0.50f;
      lineFollowCmForward(TAP_APPROACH_CM); 
      stopMotors();
      delay(200);
      currentState = AT_TAP;
      break;
    }

    case AT_TAP: {
      pourSequence();
      delay(50);
      break;
    }


    case BACK_FROM_TAP: {
      // cmReverse(TAP_APPROACH_CM)
      cmReverse(22);
      delay(200);
      pivotStartCount = count;
      currentState = LEFT_PIVOT;
      break;
    }

    case RETURN_TO_READ: {
      goneOnce = 4;
      basePWM = 170;
      readLineSensors();

      if (lineDetected()) {
        lineLostCount = 0;
        lineFollowUpdate();

        if (raw[7] > 750 && raw[0] > 750) {
          stopMotors();
          delay(200);
          if (beersServed >= MAX_BEERS) currentState = CELEBRATE;
          else {
            pivotRight15();
            pivotStartCount = count;
            currentState = RIGHT_PIVOT;
          }
        }
      }
      else {
        lineLostCount++;
        if (lineLostCount >= LINE_LOST_FRAMES) {
          stopMotors();
          lineLostCount = 0;
        }
        else setMotors(basePWM, basePWM);
      }

      delay(10);
      break;
    }

    case CELEBRATE: {
      //celebrationDance();
      mario();
      currentState = DONE;
      break;
    }

    case DONE: {
      stopMotors();
      digitalWrite(LED_PIN, HIGH);
      break;
    }
  }
}

