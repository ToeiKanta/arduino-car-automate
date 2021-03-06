/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial


#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "YpzVX9uZtN0lnd6cnyWbFJAMC7bAihIe";
char ssid[] = "Euangngam710";
char pass[] = "enplace710";

BlynkTimer timer;

int pin = 2;

void sendSensor()
{
  //Blynk.virtualWrite(V5, h);
  //Blynk.virtualWrite(V6, t);
}

void setup() { 
  pinMode(pin, OUTPUT);
  pinMode(pin, HIGH);
  Serial.begin(9600);
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
}

BLYNK_CONNECTED(){
  Blynk.syncVirtual(V1);
}
BLYNK_WRITE(V1){
  int buttonState = param.asInt();
  if (buttonState == 1) {
         Serial.println("go to Room1"); 
    }
}

void loop(){
    Blynk.run();
    
}
