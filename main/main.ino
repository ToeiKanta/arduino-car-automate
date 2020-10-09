//#include <HCSR04.h>
int enableR = 13;
int pinA1 = 2;
int pinA2 = 3;
int pinB1 = 4;
int pinB2 = 5;
int enableL = 12;

int sensorF[5] = {0, 0, 0, 0, 0};
int PINsensorF[5] = {A1,A2,A3,A4};
int sensorB[5] = {0, 0, 0, 0, 0};
int PINsensorB[5] = {A8,A9,A9,A10,A11};

int SPEED = 65;
int targetState = 4;
int state = 0;
boolean isAtStopLocationYet = false;

String carDirection = "init";

// todo
// fowarding --
// fowarding right-left
// backwarding
// backwarding right-left

//HCSR04 hc(2,3);
void setup() {
  Serial.begin(9600);
  pinMode(enableR, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);

  pinMode(enableL, OUTPUT);
  pinMode(pinB1, OUTPUT);
  pinMode(pinB2, OUTPUT);
  
  for(int i=0;i<5;i++){
    pinMode(PINsensorF[i],INPUT);
  }
  for(int i=0;i<5;i++){
    pinMode(PINsensorB[i],INPUT);
  }
}

void loop() {
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
    stopCar();
  }else if(!isAtStopLocation()){
    if(carDirection == "init"){
      forward();
    }else if(targetState < state){
      backward();
    }else if(targetState > state){
      forward();
    }
  }
}

void stopCar()
{
    carDirection == "stop";
    //Serial.println("car stop");
    digitalWrite(pinA1, LOW);
    digitalWrite(pinA2, LOW);
    
    digitalWrite(pinB1, LOW);
    digitalWrite(pinB2, LOW);
}

void turnRight(){
  carDirection = "turn_right_forward";
  analogWrite(enableR, SPEED); 
  analogWrite(enableL, SPEED);

  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, LOW);
  
  digitalWrite(pinB1, LOW);
  digitalWrite(pinB2, LOW);
}

void turnLeft(){
  carDirection = "turn_left_forward";
  analogWrite(enableR, SPEED); 
  analogWrite(enableL, SPEED);

  digitalWrite(pinA1, LOW);
  digitalWrite(pinA2, LOW);
  
  digitalWrite(pinB1, HIGH);
  digitalWrite(pinB2, LOW);
}

void forward()
{
  carDirection = "forward";
  analogWrite(enableR, SPEED); 
  analogWrite(enableL, SPEED);

  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, LOW);
  
  digitalWrite(pinB1, HIGH);
  digitalWrite(pinB2, LOW);

  if(sensorF[1] == 0 && sensorF[3] != 0){
    turnRight();
  }else if(sensorF[3] == 0  && sensorF[1] != 0){
    turnLeft();
  }
  
}

void backward()
{
  carDirection = "backward";
  analogWrite(enableR, SPEED); 
  analogWrite(enableL, SPEED);

  digitalWrite(pinA2, HIGH);
  digitalWrite(pinA1, LOW);
  
  digitalWrite(pinB2, HIGH);
  digitalWrite(pinB1, LOW);
}

boolean getStopSensorByDirection(){
  if(carDirection == "forward" ||carDirection == "turn_right_forward" ||carDirection == "turn_left_forward"){
    return sensorF[1] == 0 && sensorF[2] == 0 && sensorF[3] == 0;
  }else if(carDirection == "backward"){
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
  }else if(carDirection == "backward"){
    state--;
    Serial.print("state -- is ");
    Serial.println(state);
  }
}
