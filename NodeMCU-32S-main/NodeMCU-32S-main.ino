/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#include <Wire.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

WidgetTerminal terminal(V50);

char auth[] = "YpzVX9uZtN0lnd6cnyWbFJAMC7bAihIe";
char ssid[] = "Euangngam710";
char pass[] = "enplace710";

BlynkTimer timer;

int pin = 2;

void setup() { 
  Serial1.begin(115200);
  Serial1.println("NodeMCU connected...");
  pinMode(pin, OUTPUT);
  pinMode(pin, HIGH);
  Serial.begin(115200);
  delay(10);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);
  int wifi_ctr = 0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
 Serial.println("WiFi connected"); 
 Blynk.begin(auth, ssid, pass);

 terminal.clear();
terminal.println("NodeMCU connected");
//terminal.flush();
}

BLYNK_CONNECTED(){
  Blynk.syncVirtual(V1);
}
BLYNK_WRITE(V1){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R01"); 
    Serial1.print("R01");
    terminal.println("Set target to room 1");
    terminal.flush();
  }
}
BLYNK_WRITE(V2){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R02"); 
    Serial1.print("R02");
    terminal.println("Set target to room 2");
    terminal.flush();
  }
}
BLYNK_WRITE(V3){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R03"); 
    Serial1.print("R03");
    terminal.println("Set target to room 3");
    terminal.flush();
  }
}
BLYNK_WRITE(V4){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R04"); 
    Serial1.print("R04");
    terminal.println("Set target to room 4");
    terminal.flush();
  }
}
BLYNK_WRITE(V5){
  int buttonState = param.asInt();
  if (buttonState == 1) {
    Serial.println("R05"); 
    Serial1.print("R05");
    terminal.println("Set target to room 5");
    terminal.flush();
  }
}
void loop(){
    Blynk.run();
}
