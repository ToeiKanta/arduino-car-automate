
#include <HCSR04.h>
#include <stdio.h>

#define MAX_OUT_CHARS 16  //max nbr of characters to be sent on any one serial command

char   buffer[MAX_OUT_CHARS + 1];  //buffer used to format a line (+1 is for trailing 0)
// Ultrasonic
// pin 6 front , pin 7 back
HCSR04 hc(11, new int[2] {6, 7}, 2); //initialisation class HCSR04 (trig pin , echo pin, number of sensor)
int ultraCount[2] = {0, 0};
const boolean enableUltra = true;
const boolean enableUltraLog = false;
int prevCarMode = 0;
boolean forceStop = false;
// Touch sensor
int touchSensor[5] = {22,24,26,30,34};
boolean isTouched = false;

String serial1 = "";
int enableR = 13;
int pinR1 = 2;
int pinR2 = 3;
int pinL1 = 4;
int pinL2 = 5;
int enableL = 12;

int sensorF[5] = {0, 0, 0, 0, 0};
int PINsensorF[5] = {A1, A2, A3, A4};
int sensorB[5] = {0, 0, 0, 0, 0};
int PINsensorB[5] = {A8, A9, A9, A10, A11};

int carMode = 1;// 1 stop, 2 move forward, 3 move backward 

int SPEED = 155; // 50 -255
int prevState = 0;
int targetState = 0;
int x = 0;
int state = 0;
boolean isAtStopLocationYet = false;
String carDirection = "init";

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
 
  for (int i = 0; i < 5; i++) {
     pinMode(touchSensor[i], INPUT);
  }
  pinMode(enableR, OUTPUT);
  pinMode(pinR1, OUTPUT);
  pinMode(pinR2, OUTPUT);

  pinMode(enableL, OUTPUT);
  pinMode(pinL1, OUTPUT);
  pinMode(pinL2, OUTPUT);
  Serial.println("init");
  for (int i = 0; i < 5; i++) {
    pinMode(PINsensorF[i], INPUT);
  }
  for (int i = 0; i < 5; i++) {
    pinMode(PINsensorB[i], INPUT);
  }
 delay(2000);
  Serial1.print("init,");
  delay(2000);
  while (isSensorTouched() == false) {
    delay(100);
  }
  Serial.println("start");
  Serial1.print("start,");

 
}

void setupSensor(){
  sensorF[0] = digitalRead(A1);
  sensorF[1] = digitalRead(A2);
  sensorF[2] = digitalRead(A3);
  sensorF[3] = digitalRead(A4);
  sensorF[4] = digitalRead(A5);

  sensorB[0] = digitalRead(A8);
  sensorB[1] = digitalRead(A9);
  sensorB[2] = digitalRead(A10);
  sensorB[3] = digitalRead(A11);
  sensorB[4] = digitalRead(A12);
}

boolean isSensorTouched(){
  for(int i = 0 ; i<5; i++){
      if(digitalRead(touchSensor[i]) == HIGH){
        return true;
      }
    }
   return false;
}
void loop() {
  /** Check Touch Sensor **/
//  while(true){
//    for(int i = 0 ; i<5; i++){
//      Serial.print(digitalRead(touchSensor[i]));
//    }
//    Serial.println("");
//    delay(100);
//  }
  recieveSerial1();
  // check if ultrasonic sensor <= 5 cm
  ultrasonicHandle();
  setupSensor();
  if (isSensorTouched() && isTouched == false) {
    isTouched = true;
  }
  if (isTouched) {
    if (targetState == state && carDirection != "init" && prevState != state) {
      prevState = state;
      Serial.println("success... please touch the sensor");
      isTouched = false;
      stopCar();
      carMode = 1;
//      delay(1000);
      Serial1.print("success,");
    } else if(!forceStop){
      if (targetState < state) {
        carMode = 3;
      } else if (targetState > state) {
        carMode = 2;
      }
    }
  }
  
  if(carMode == 1 || forceStop == true){
    stopCar();
  }else if(carMode == 2){
    checkStopLocation();
    forward();
  }else if(carMode == 3){
    checkStopLocation();
    backward();
  }
}
void ultrasonicHandle() 
{
//  Serial.println(carMode);
//  delay(50);
  if (enableUltra) {
    if(enableUltraLog){
      Serial.print("ultra: ");
    }
    int hcSensor = 0;
    if(targetState < state){
      hcSensor = 1; //choose back ultrasonic
    }else{
      hcSensor = 0; //choose front ultrasonic
    }
//    for (int i = 1; i <= 1; i++) {
    float val = hc.dist(hcSensor);
    if(enableUltraLog){
      Serial.print(val);
      Serial.print(" ");
    }
      
      if (val <= 15.0) {
        ultraCount[hcSensor]++;
        if (carMode != 1 && ultraCount[hcSensor] >= 1) {
          prevCarMode = carMode;
          ultraCount[hcSensor] = 0;
          carMode = 1;
          forceStop = true;
          Serial.print("Stop by ultrasonic no.");
          Serial.print(hcSensor);
          Serial.print(" by ");
          Serial.println(val);
          if(hcSensor == 0){
            Serial1.print("STOP BY ULTRA FRONT,");
          }else if(hcSensor == 1){
            Serial1.print("STOP BY ULTRA BACK,");
          }
          
          //          delay(100);
        }
      } else {
        forceStop = false;
        ultraCount[hcSensor] = 0;
        if(carMode != 1){
          carMode = prevCarMode;
        }
      }
//    } //for
    if(enableUltraLog){
      Serial.println("");
    }
  }
}

void recieveSerial1() {
  if (Serial1.available()>0) {
    serial1 = Serial1.readString();
    Serial.print("receive-ori: ");
    Serial.println(serial1);
    String str = "";
    int j = 0;
    while ( j < serial1.length())
    {
      if (serial1.charAt(j) == ',')
      {
        if(str.length()>0){
          // command here
          Serial.print("receive: ");
          Serial.println(str);
          if (str == "R1") {
            targetState = 1;
            Serial.println("send R to NodeMCU");
            Serial1.print("R,");
            
          } else if (str == "R2") {
            targetState = 2;
            Serial.println("send R to NodeMCU");
            Serial1.print("R,");
            
          } else if (str == "R3") {
            targetState = 3;
            Serial.println("send R to NodeMCU");
            Serial1.print("R,");
            
          } else if (str == "R4") {
            targetState = 4;
            Serial.println("send R to NodeMCU");
            Serial1.print("R,");
            
          } else if (str == "R5") {
            Serial.println("send R to NodeMCU");
            targetState = 5;
            Serial1.print("R,");
          } else if (str == "R6") {
            Serial.println("send R to NodeMCU");
            targetState = 6;
            Serial1.print("R,");
          } else if (str == "FT") {
            isTouched = true;
            Serial.println("send R to NodeMCU");
            Serial1.print("R,");
          }
        }
        str = "";
      }else{
        str += serial1.charAt(j);
      }
      j++;
    }
  }
}

void stopCar()
{
  carDirection == "stop";
  //Serial.println("car stop");
  digitalWrite(pinR1, LOW);
  digitalWrite(pinR2, LOW);

  digitalWrite(pinL1, LOW);
  digitalWrite(pinL2, LOW);
}

void turnLeftBackwardFast() {
  carDirection = "turn_left_backward";
  analogWrite(enableR, SPEED*(0.0/5));//0
  analogWrite(enableL, SPEED*(5.0/5));
  
  digitalWrite(pinR2, LOW);//x
  digitalWrite(pinR1, HIGH);//x

  digitalWrite(pinL2, HIGH);
  digitalWrite(pinL1, LOW);
}

void turnRightBackwardFast() {
  carDirection = "turn_right_backward";
  analogWrite(enableR, SPEED*(5.0/5));
  analogWrite(enableL, SPEED*(0.0/5));//0
  
  digitalWrite(pinR2, HIGH);
  digitalWrite(pinR1, LOW);

  digitalWrite(pinL2, LOW);//x
  digitalWrite(pinL1, HIGH);//x
}
void turnLeftBackward() {
  carDirection = "turn_left_backward";
  analogWrite(enableR, SPEED*(2.0/5.0));//0
  analogWrite(enableL, SPEED*(4.0/5.0));
  
  digitalWrite(pinR2, HIGH);//x
  digitalWrite(pinR1, LOW);//x

  digitalWrite(pinL2, HIGH);
  digitalWrite(pinL1, LOW);
}
void turnRightBackward() {
  carDirection = "turn_right_backward";
  analogWrite(enableR, SPEED*(4.0/5));
  analogWrite(enableL, SPEED*(2.0/5));//0
  
  digitalWrite(pinR2, HIGH);
  digitalWrite(pinR1, LOW);

  digitalWrite(pinL2, HIGH);//x
  digitalWrite(pinL1, LOW);//x
}

void turnRightForward() {
  analogWrite(enableR, SPEED*(3.5/5));//0
  analogWrite(enableL, SPEED*(4.5/5));
  
  digitalWrite(pinR1, HIGH);//x
  digitalWrite(pinR2, LOW);//x

  digitalWrite(pinL1, HIGH);
  digitalWrite(pinL2, LOW);
}

void turnLeftForward() {
  analogWrite(enableR, SPEED*(4.5/5));
  analogWrite(enableL, SPEED*(3.5/5));//0
    
  digitalWrite(pinR1, HIGH);
  digitalWrite(pinR2, LOW);

  digitalWrite(pinL1, HIGH);//x
  digitalWrite(pinL2, LOW);//x
}

void turnLeftForwardFast(){
  analogWrite(enableR, SPEED*(4.0/5));
  analogWrite(enableL, SPEED*(1.0/5));//0
    
  digitalWrite(pinR1, HIGH);
  digitalWrite(pinR2, LOW);

  digitalWrite(pinL1, HIGH);//x
  digitalWrite(pinL2, LOW);//x
}
void turnRightForwardFast(){
  analogWrite(enableR, SPEED*(1.0/5));//0
  analogWrite(enableL, SPEED*(4.0/5));
  
  digitalWrite(pinR1, HIGH);//x
  digitalWrite(pinR2, LOW);//x

  digitalWrite(pinL1, HIGH);
  digitalWrite(pinL2, LOW);
}
void forward()
{
  if (sensorF[0] == 0 && sensorF[3] != 0) {
    turnLeftForwardFast();
  }else if(sensorF[1] == 0 && sensorF[3] != 0 ){
    turnLeftForward();
  }else if (sensorF[4] == 0  && sensorF[1] != 0) {
    turnRightForwardFast();
  }else if(sensorF[3] == 0 && sensorF[1] != 0){
    turnRightForward();
  }else if(sensorF[0] != 0 && sensorF[1] != 0 && sensorF[2] != 0 && sensorF[3] != 0 && sensorF[4] != 0){
    if(carDirection == "turn_left_forward"){
      turnLeftForwardFast();
    }else{
      turnRightForwardFast();
    }
  }else{
    analogWrite(enableR, SPEED);
    analogWrite(enableL, SPEED);
  
    digitalWrite(pinR1, HIGH);
    digitalWrite(pinR2, LOW);
  
    digitalWrite(pinL1, HIGH);
    digitalWrite(pinL2, LOW);
    carDirection = "forward";
  }

}

void backward()
{
  if (sensorB[0] == 0 && sensorB[3] != 0) {
    turnLeftBackwardFast();
  }else if(sensorB[1] == 0 && sensorB[3] != 0){
    turnLeftBackward();
  }else if (sensorB[4] == 0  && sensorB[1] != 0) {
    turnRightBackwardFast();
  }else if(sensorB[3] == 0  && sensorB[1] != 0){
    turnRightBackward();
  }
  else if(sensorB[0] != 0 && sensorB[1] != 0 && sensorB[2] != 0 && sensorB[3] != 0 && sensorB[4] != 0){
    if(carDirection == "turn_left_backward"){
      turnLeftBackwardFast();
    }else{
      turnRightBackwardFast();
    }
  }else{
    carDirection = "backward";
    analogWrite(enableR, SPEED);
    analogWrite(enableL, SPEED);
  
    digitalWrite(pinR2, HIGH);
    digitalWrite(pinR1, LOW);
  
    digitalWrite(pinL2, HIGH);
    digitalWrite(pinL1, LOW);
  }
}

boolean getStopSensorByDirection() {
  if (carMode == 2) {
    return (sensorF[0] == 0 && sensorF[4] == 0);
  } else if (carMode == 3) {
    return (sensorB[0] == 0 && sensorB[4] == 0);
  }else{
    return true;
  }
}

void checkStopLocation()
{
  if (isAtStopLocationYet == false) {
    if (getStopSensorByDirection()) {
      isAtStopLocationYet = true;
      updateState();
    }
  } else if (getStopSensorByDirection() == false) {
    isAtStopLocationYet = false;
  }

}

void updateState() {
  if (carMode == 2) {
    prevState = state;
    state++;
    String room = "R" + String(state) + ",";
    Serial1.print(room);
    
    Serial.print("state ++ is ");
    Serial.println(state);
  } else if (carMode == 3) {
    prevState = state;
    state--;
    if(state < 0){
      state = 0;
    }
    delay(100);
    String room = "R" + String(state) + ",";
    Serial1.print(room);
    
    Serial.print("state -- is ");
    Serial.println(state);
  }
}
