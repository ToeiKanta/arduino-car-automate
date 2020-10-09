#include <HCSR04.h>
int enableA = 13;
int pinA1 = 2;
int pinA2 = 3;
int pinB1 = 4;
int pinB2 = 5;
int enableB = 12;

int sensorF[5] = {0, 0, 0, 0, 0};
int PINsensorF[5] = {A1,A2,A3,A4};
int sensorB[5] = {0, 0, 0, 0, 0};
int PINsensorB[5] = {A8,A9,A9,A10,A11};

int SPEED = 65;
int targetState = 3;
int state = 6;
boolean isAtStopLocationYet = false;

string carDirection = "init";

// todo
// fowarding --
// fowarding right-left
// backwarding
// backwarding right-left

HCSR04 hc(2,3);
void setup() {
  Serial.begin(9600);
  pinMode(enableA, OUTPUT);
  pinMode(pinA1, OUTPUT);
  pinMode(pinA2, OUTPUT);

  pinMode(enableB, OUTPUT);
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

  sensorB[5] = digitalRead(A8);
  sensorB[6] = digitalRead(A9);
  sensorB[7] = digitalRead(A10);
  sensorB[8] = digitalRead(A11);
  sensorB[9] = digitalRead(A12);
  
  if(targetState == state){
    stopCar();
  }else if(!isAtStopLocation() || carDirection == "init"){
    forward();
  }
  
}

void stopCar()
{
  if(carDirection == "stop")
    return ;
  analogWrite(enableA, 0); 
  analogWrite(enableB, 0);
}

void forward()
{
  if(carDirection == "forward")
    return ;
  analogWrite(enableA, SPEED); 
  analogWrite(enableB, SPEED);

  digitalWrite(pinA1, HIGH);
  digitalWrite(pinA2, LOW);
  
  digitalWrite(pinB1, HIGH);
  digitalWrite(pinB2, LOW);
  
}

void backward()
{
  if(carDirection == "backward")
    return ;
  analogWrite(enableA, SPEED); 
  analogWrite(enableB, SPEED);

  digitalWrite(pinA2, HIGH);
  digitalWrite(pinA1, LOW);
  
  digitalWrite(pinB2, HIGH);
  digitalWrite(pinB1, LOW);
}

boolean isAtStopLocation()
{
  if(isAtStopLocationYet == false){
      if( sensorF[1] == 0 && sensorF[2] == 0 && sensorF[3] == 0 ){
        isAtStopLocationYet = true;
        state++;
        Serial.print("state ++ is ");
        Serial.println(state);
        return true;
      }
  }else if(!(sensorF[1] == 0 && sensorF[2] == 0 && sensorF[3] == 0)){
    isAtStopLocationYet = false;
    return false;
  }
  
}

