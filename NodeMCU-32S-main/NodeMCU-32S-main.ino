/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

//#include <Wire.h>
#include <WiFi.h>
//#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <BlynkSimpleEsp32.h>

WidgetTerminal terminal(V50);

boolean DEBUG_LOG = true;
// Line Token
#define TokenLineRoom1 "FOAyf34EQQdDbNBrSoGjZLWYtOPVF9geBhLb9FarBkQ" 
#define TokenLineRoom2 "J9wVMSm1FTthoUJLhEsA0RKcknRixIIC5H3sIXSeg88"
#define TokenLineRoom3 "VjGub9wQpu6KunXccNyffVTosveGOEhCObmpippvcJz"
#define TokenLineRoom4 "sPAI8Qds06UyPbWRNUybDclg8KCwHq0aDFrsiKqTrIZ"
#define TokenLineRoom5 "olagkzAOHxEwrxPMqwrN7ynitjGCGJzcDKiT7PyaG67"
#define TokenLineRoom6 "dCfpnmQMxAtgEoj7g2zdl6Cg8jZ98ry80TNEbDQdBcG"
//const int resetMegaPin = 34;

char auth[] = "YpzVX9uZtN0lnd6cnyWbFJAMC7bAihIe";
char ssid[] = "Euangngam710";//"gkamspri"; // Euangngam710
char pass[] = "enplace710";//0979934765; // enplace710

BlynkTimer timer;
boolean isMegaConnected = false;
int count = 0;

int targetRoom = 0;
int tempTargetRoom = 0;
String serial1 = "";
int ledPin = 2;

void setup() { 
  Serial1.begin(115200);
  Serial1.println("NodeMCU connected...,");
//  pinMode(resetMegaPin, OUTPUT);
//  digitalWrite(resetMegaPin,HIGH);
  pinMode(ledPin, OUTPUT);
  pinMode(ledPin, LOW);
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
  digitalWrite(ledPin, HIGH);
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
// resetMegaBoard();
}
//
//void resetMegaBoard(){
// isMegaConnected = false;
// delay(2000);
// digitalWrite(resetMegaPin,LOW);//reset Mega board
//}
void NotifyLine(String t,String TokenLine){
  WiFiClientSecure client;
  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("Connection failed");
    return;
  }
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(TokenLine) + "\r\n";
  req += "Cache-Control: no-cache\r\n";
  req += "User-Agent: ESP32\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(String("message=" + t).length()) + "\r\n";
  req += "\r\n";
  req += "message=" + t;
  Serial.println(req);
  client.print(req);
  delay(20);
  Serial.println("-------------");
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      break;
    }
  }
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
//  Blynk.setProperty(V0,"onLabel","GO");
//  Blynk.setProperty(V0,"offLabel","GO");
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
  Blynk.setProperty(V6,"onLabel","GO");
  Blynk.setProperty(V6,"offLabel","GO");
}
void setWaitRoomBtn(){
//  Blynk.setProperty(V0,"onLabel","Wait...");
//  Blynk.setProperty(V0,"offLabel","Wait...");
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
  Blynk.setProperty(V6,"onLabel","Wait...");
  Blynk.setProperty(V6,"offLabel","Wait...");
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
              terminal.println("");
              terminal.print("receive: ");
              terminal.println(str);
              terminal.flush();
            }
            if(str == "success"){
               terminal.print("Going to ROOM ");
               terminal.print(String(targetRoom));
               String msg = "โปรดมารับอาหารและยาค่ะ (เมื่อรับเสร็จแล้ว กรุณากดที่ปุ่มข้างถาดนะคะ)";
               switch(targetRoom){
                 case 1: NotifyLine(msg,TokenLineRoom1);break;
                 case 2: NotifyLine(msg,TokenLineRoom2);break;
                 case 3: NotifyLine(msg,TokenLineRoom3);break;
                 case 4: NotifyLine(msg,TokenLineRoom4);break;
                 case 5: NotifyLine(msg,TokenLineRoom5);break;
                 case 6: NotifyLine(msg,TokenLineRoom6);break;
               }
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
            }else if(str == "R1"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 1");
              Blynk.setProperty(V51,"onLabel","ROOM 1");
              terminal.println("Reaching to Room 1");
              terminal.flush();
            }else if(str == "R2"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 2");
              Blynk.setProperty(V51,"onLabel","ROOM 2");
              terminal.println("Reaching to Room 2");
              terminal.flush();
            }else if(str == "R3"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 3");
              Blynk.setProperty(V51,"onLabel","ROOM 3");
              terminal.println("Reaching to Room 3");
              terminal.flush();
            }else if(str == "R4"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 4");
              Blynk.setProperty(V51,"onLabel","ROOM 4");
              terminal.println("Reaching to Room 4");
              terminal.flush();
            }else if(str == "R5"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 5");
              Blynk.setProperty(V51,"onLabel","ROOM 5");
              terminal.println("Reaching to Room 5");
              terminal.flush();
            }else if(str == "R6"){ // send current room
              Blynk.setProperty(V51,"offLabel","ROOM 6");
              Blynk.setProperty(V51,"onLabel","ROOM 6");
              terminal.println("Reaching to Room 6");
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


BLYNK_WRITE(V51){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("Toggle debug mode"); 
    DEBUG_LOG = !DEBUG_LOG;
    terminal.println("Toggle debug mode");
    terminal.flush();
  }
}
BLYNK_WRITE(V52){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("send command FT (Force Touch)"); 
    Serial1.print("FT,");
     Serial1.flush();
    terminal.print("Sending command Force Touch...");
    terminal.flush();
  }
}

BLYNK_WRITE(V6){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R6"); 
    Serial1.print("R6,");
    Serial1.flush();
    tempTargetRoom = 6;
    terminal.print("Set target to room 6...");
    terminal.flush();
  }
}
BLYNK_WRITE(V1){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R1"); 
    Serial1.print("R1,");
     Serial1.flush();
    tempTargetRoom = 1;
    terminal.print("Set target to room 1...");
    terminal.flush();
  }
}
BLYNK_WRITE(V2){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R2"); 
    Serial1.print("R2,");
     Serial1.flush();
    tempTargetRoom = 2;
    terminal.print("Set target to room 2...");
    terminal.flush();
  }
}
BLYNK_WRITE(V3){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R3"); 
    Serial1.print("R3,");
     Serial1.flush();
    tempTargetRoom = 3;
    terminal.print("Set target to room 3...");
    terminal.flush();
  }
}
BLYNK_WRITE(V4){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R4"); 
    Serial1.print("R4,");
     Serial1.flush();
    tempTargetRoom = 4;
    terminal.print("Set target to room 4...");
    terminal.flush();
    
  }
}
BLYNK_WRITE(V5){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R5"); 
    Serial1.print("R5,");
     Serial1.flush();
    tempTargetRoom = 5;
    terminal.print("Set target to room 5...");
    terminal.flush();
  }
}
void loop(){
//  if(isMegaConnected == false){
//    terminal.print(".");
//    delay(50);
//  }else if(count == 0){
//    terminal.println("Mega Connected!");
//    terminal.flush();
//    count++;
//  }
//  if(isMegaConnected){
//    digitalWrite(resetMegaPin, HIGH);
//  }
    recieveSerial1();
    Blynk.run();
}
