
#include <HCSR04.h>
HCSR04 hc(11,new int[4]{6,7,8,9},4);//initialisation class HCSR04 (trig pin , echo pin, number of sensor)
int ultraCount[4] = {0,0,0,0};
boolean enableUltra = false;
// Touch sensor
int touchSensor = 50;

String serial1 = "";
int enableR = 13;
int pinR1 = 2;
int pinR2 = 3;
int pinL1 = 4;
int pinL2 = 5;
int enableL = 12;

int sensorF[5] = {0, 0, 0, 0, 0};
int PINsensorF[5] = {A1,A2,A3,A4};
int sensorB[5] = {0, 0, 0, 0, 0};
int PINsensorB[5] = {A8,A9,A9,A10,A11};

int SPEED = 100;
int targetState = 4;
int x = 0;
int state = 0;
boolean isAtStopLocationYet = false;

String carDirection = "init";

void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  pinMode(touchSensor, INPUT);
  pinMode(enableR, OUTPUT);
  pinMode(pinR1, OUTPUT);
  pinMode(pinR2, OUTPUT);

  pinMode(enableL, OUTPUT);
  pinMode(pinL1, OUTPUT);
  pinMode(pinL2, OUTPUT);
  Serial.println("init");
  for(int i=0;i<5;i++){
    pinMode(PINsensorF[i],INPUT);
  }
  for(int i=0;i<5;i++){
    pinMode(PINsensorB[i],INPUT);
  }
  while(digitalRead(touchSensor) == LOW){
    }
    Serial.println("Start");
    forward();
}

void loop() {
  recieveSerial1();
  // check if ultrasonic sensor <= 5 cm
  ultrasonicHandle();
//  Serial.println(digitalRead(touchSensor));
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
  
  if(targetState == state){
    Serial.println("Stop");
    stopCar();
    while(digitalRead(touchSensor) == LOW){
    }
    // x++;
  }else if(!isAtStopLocation()){
    if(carDirection == "init" || state == 0){
//      Serial.println("Forward");
      forward();
    }else if(targetState < state){
//      Serial.println("Backward");
      backward();
    }else if(targetState > state){
//      Serial.println("Forward");
      forward();
    }
  }
}

void ultrasonicHandle(){
  if(enableUltra){
    for(int i=1;i<=3;i++){
      if(hc.dist(i) <= 5.0){
        ultraCount[i]++;
        if(ultraCount[i] >= 5){
          stopCar();
          ultraCount[i] = 0;
          Serial.println("Stop 2");
//          delay(100);
        }
      }else{
        ultraCount[i] = 0;
      }
    }
  }
}

void recieveSerial1(){
  if(Serial1.available()){
    serial1 = Serial1.readString();
    Serial.println(serial1);
    if(serial1 == "R01"){
      targetState = 1;
    }else if(serial1 == "R02"){
      targetState = 2;
    }else if(serial1 == "R03"){
      targetState = 3;
    }else if(serial1 == "R04"){
      targetState = 4;
    }else if(serial1 == "R05"){
      targetState = 5;
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

void turnLeftBackward(){
  carDirection = "turn_left_backward";
  if(sensorB[4] == 0){
    analogWrite(enableR, 0); 
    analogWrite(enableL, SPEED);
  }else{
    analogWrite(enableR, SPEED/2); 
    analogWrite(enableL, SPEED);
  }
  digitalWrite(pinR2, HIGH);
  digitalWrite(pinR1, LOW);
  
  digitalWrite(pinL2, HIGH);
  digitalWrite(pinL1, LOW);
}

void turnRightBackward(){
  carDirection = "turn_right_backward";
  if(sensorB[0] == 0){
    analogWrite(enableR, SPEED); 
    analogWrite(enableL, 0);
  }else {
    analogWrite(enableR, SPEED); 
    analogWrite(enableL, SPEED/2);
  }
  digitalWrite(pinR2, HIGH);
  digitalWrite(pinR1, LOW);
  
  digitalWrite(pinL2, HIGH);
  digitalWrite(pinL1, LOW);
}

void turnRightForward(){
  carDirection = "turn_right_forward";
  if(sensorF[4] == 0){
    analogWrite(enableR, 0); 
    analogWrite(enableL, SPEED);
  }else{
    analogWrite(enableR, SPEED/2); 
    analogWrite(enableL, SPEED);
  }
  digitalWrite(pinR1, HIGH);
  digitalWrite(pinR2, LOW);
  
  digitalWrite(pinL1, HIGH);
  digitalWrite(pinL2, LOW);
}

void turnLeftForward(){
  carDirection = "turn_left_forward";
  if(sensorF[0] == 0){
    analogWrite(enableR, SPEED); 
    analogWrite(enableL, 0);
  }else {
    analogWrite(enableR, SPEED); 
    analogWrite(enableL, SPEED/2);
  }
  digitalWrite(pinR1, HIGH);
  digitalWrite(pinR2, LOW);
  
  digitalWrite(pinL1, HIGH);
  digitalWrite(pinL2, LOW);
}

void forward()
{
  carDirection = "forward";
  analogWrite(enableR, SPEED); 
  analogWrite(enableL, SPEED);

  digitalWrite(pinR1, HIGH);
  digitalWrite(pinR2, LOW);
  
  digitalWrite(pinL1, HIGH);
  digitalWrite(pinL2, LOW);

  if((sensorF[1] == 0 && sensorF[3] != 0 ) || (sensorF[0] == 0 && sensorF[3] != 0) ){
    turnLeftForward();
  }else if((sensorF[3] == 0  && sensorF[1] != 0) || (sensorF[4] == 0 && sensorF[1] != 0) ){
    turnRightForward();
  }
  
}

void backward()
{
  carDirection = "backward";
  analogWrite(enableR, SPEED); 
  analogWrite(enableL, SPEED);

  digitalWrite(pinR2, HIGH);
  digitalWrite(pinR1, LOW);
  
  digitalWrite(pinL2, HIGH);
  digitalWrite(pinL1, LOW);

  if((sensorB[1] == 0 && sensorB[3] != 0) || (sensorB[0] == 0 && sensorB[3] != 0)){
    turnLeftBackward();
  }else if((sensorB[3] == 0  && sensorB[1] != 0) || (sensorB[4] == 0  && sensorB[1] != 0)){
    turnRightBackward();
  }
}

boolean getStopSensorByDirection(){
  if(carDirection == "forward" ||carDirection == "turn_right_forward" ||carDirection == "turn_left_forward"){
    return sensorF[1] == 0 && sensorF[2] == 0 && sensorF[3] == 0;
  }else if(carDirection == "backward" ||carDirection == "turn_right_backward" ||carDirection == "turn_left_backward"){
    return sensorB[1] == 0 && sensorB[2] == 0 && sensorB[3] == 0;
  }
}

boolean isAtStopLocation()
{
  if(isAtStopLocationYet == false){
      if(getStopSensorByDirection()){
        isAtStopLocationYet = true;
        updateState();
        return true;
      }
  }else if(!getStopSensorByDirection()){
    isAtStopLocationYet = false;
    return false;
  }
  
}

void updateState(){
  if(carDirection == "forward" ||carDirection == "turn_right_forward" ||carDirection == "turn_left_forward"){
    state++;
    Serial.print("state ++ is ");
    Serial.println(state);
  }else if(carDirection == "backward" ||carDirection == "turn_right_backward" ||carDirection == "turn_left_backward"){
    state--;
    Serial.print("state -- is ");
    Serial.println(state);
  }
}
