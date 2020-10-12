/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

WidgetTerminal terminal(V50);

const boolean DEBUG_LOG = true;

char auth[] = "YpzVX9uZtN0lnd6cnyWbFJAMC7bAihIe";
char ssid[] = "gkamspri";
char pass[] = "0979934765";

BlynkTimer timer;
boolean isMegaConnected = false;
int count = 0;

int targetRoom = 0;
int tempTargetRoom = 0;
String serial1 = "";
int pin = 2;

void setup() { 
  Serial1.begin(115200);
  Serial1.println("NodeMCU connected...,");
  
  pinMode(pin, OUTPUT);
  pinMode(pin, HIGH);
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting to ");
  count = 0;
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
 Serial.println("WiFi connected"); 
 Blynk.begin(auth, ssid, pass);
  delay(1000);
  while(!Blynk.connected()){
    delay(100);
  }
  delay(100);
 terminal.clear();
 terminal.print("WiFi connected to -> ");
 terminal.println(ssid);
 terminal.println("NodeMCU connected");
 terminal.flush();
 setDefaultRoomBtn(true);
 isMegaConnected = false;
}

//BLYNK_CONNECTED(){
//  Blynk.syncVirtual(V1);
//  Blynk.syncVirtual(V2);
//  Blynk.syncVirtual(V3);
//  Blynk.syncVirtual(V4);
//  Blynk.syncVirtual(V5);
//  Blynk.syncVirtual(V6);
//}
void setDefaultRoomBtn(bool resetRoom){
  if(resetRoom){
    targetRoom = 0;
    tempTargetRoom = 0;
    Blynk.setProperty(V51,"onLabel","ROOM 0");
    Blynk.setProperty(V51,"offLabel","ROOM 0");
    Blynk.setProperty(V52,"onLabel","ROOM 0");
    Blynk.setProperty(V52,"offLabel","ROOM 0");
  }
  Blynk.setProperty(V0,"onLabel","GO");
  Blynk.setProperty(V0,"offLabel","GO");
  Blynk.setProperty(V1,"onLabel","GO");
  Blynk.setProperty(V1,"offLabel","GO");
  Blynk.setProperty(V2,"onLabel","GO");
  Blynk.setProperty(V2,"offLabel","GO");
  Blynk.setProperty(V3,"onLabel","GO");
  Blynk.setProperty(V3,"offLabel","GO");
  Blynk.setProperty(V4,"onLabel","GO");
  Blynk.setProperty(V4,"offLabel","GO");
  Blynk.setProperty(V5,"onLabel","GO");
  Blynk.setProperty(V5,"offLabel","GO");
}
void setWaitRoomBtn(){
  Blynk.setProperty(V0,"onLabel","Wait...");
  Blynk.setProperty(V0,"offLabel","Wait...");
  Blynk.setProperty(V1,"onLabel","Wait...");
  Blynk.setProperty(V1,"offLabel","Wait...");
  Blynk.setProperty(V2,"onLabel","Wait...");
  Blynk.setProperty(V2,"offLabel","Wait...");
  Blynk.setProperty(V3,"onLabel","Wait...");
  Blynk.setProperty(V3,"offLabel","Wait...");
  Blynk.setProperty(V4,"onLabel","Wait...");
  Blynk.setProperty(V4,"offLabel","Wait...");
  Blynk.setProperty(V5,"onLabel","Wait...");
  Blynk.setProperty(V5,"offLabel","Wait...");
}
void recieveSerial1(){
    if(Serial1.available()){
      serial1 = Serial1.readString();
      delay(10);
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
            if(DEBUG_LOG){
              terminal.print("receive: ");
              terminal.println(str);
              terminal.flush();
            }
            if(str == "success"){
               terminal.print("Going to ROOM ");
               terminal.print(String(targetRoom));
               terminal.println(" success!!");
               terminal.println("Please touch the sensor to continue.");
               terminal.flush();
               setDefaultRoomBtn(false);
            }else if(str == "start"){
              isMegaConnected = true;
              terminal.println("start...");
               terminal.flush();
            }else if(str == "init"){
              isMegaConnected = true;
               terminal.clear();
               terminal.println("Connected...Please touch the sensor to start.");
               terminal.flush();
               setDefaultRoomBtn(true);
            }else if(str == "R"){ // confirm received room target
              targetRoom = tempTargetRoom;
              setWaitRoomBtn();
              String target = "ROOM " + String(targetRoom);
              Blynk.setProperty(V52,"onLabel",target);
              Blynk.setProperty(V52,"offLabel",target);
              terminal.println("OK!");
              terminal.flush();
            }else if(str == "R01"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 1");
              Blynk.setProperty(V51,"onLabel","ROOM 1");
              terminal.println("Reaching to Room 1");
              terminal.flush();
            }else if(str == "R02"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 2");
              Blynk.setProperty(V51,"onLabel","ROOM 2");
              terminal.println("Reaching to Room 2");
              terminal.flush();
            }else if(str == "R03"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 3");
              Blynk.setProperty(V51,"onLabel","ROOM 3");
              terminal.println("Reaching to Room 3");
              terminal.flush();
            }else if(str == "R04"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 4");
              Blynk.setProperty(V51,"onLabel","ROOM 4");
              terminal.println("Reaching to Room 4");
              terminal.flush();
            }else if(str == "R05"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 5");
              Blynk.setProperty(V51,"onLabel","ROOM 5");
              terminal.println("Reaching to Room 5");
              terminal.flush();
            }
          }
          str = "";
        }
        else
        {
          str += serial1.charAt(j);
        }
        j++;
      }
      
    }
}
BLYNK_WRITE(V6){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R06"); 
    Serial1.print("R06,");
    tempTargetRoom = 6;
    terminal.print("Set target to room 6...");
    terminal.flush();
  }
}
BLYNK_WRITE(V1){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R01"); 
    Serial1.print("R01,");
    
    tempTargetRoom = 1;
    terminal.print("Set target to room 1...");
    terminal.flush();
  }
}
BLYNK_WRITE(V2){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R02"); 
    Serial1.print("R02,");
    
    tempTargetRoom = 2;
    terminal.print("Set target to room 2...");
    terminal.flush();
  }
}
BLYNK_WRITE(V3){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R03"); 
    Serial1.print("R03,");
    
    tempTargetRoom = 3;
    terminal.print("Set target to room 3...");
    terminal.flush();
  }
}
BLYNK_WRITE(V4){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R04"); 
    Serial1.print("R04,");
    tempTargetRoom = 4;
    terminal.print("Set target to room 4...");
    terminal.flush();
    
  }
}
BLYNK_WRITE(V5){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R05"); 
    Serial1.print("R05,");
    tempTargetRoom = 5;
    terminal.print("Set target to room 5...");
    terminal.flush();
  }
}
void loop(){
//  if(isMegaConnected == false){
////    terminal.print(".......");
//  }else if(count == 0){
//    terminal.println("Mega Connected!");
//    terminal.flush();
//    count++;
//  }
    recieveSerial1();
    Blynk.run();
}
