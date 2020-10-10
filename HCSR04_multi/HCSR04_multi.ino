#include <HCSR04.h>

HCSR04 hc(11,new int[4]{6,7,8,9},4);//initialisation class HCSR04 (trig pin , echo pin, number of sensor)

void setup()
{ Serial.begin(9600); }

void loop()
{ for (int i = 0; i <= 3; i++ ) {

  Serial.print(i);
    Serial.print("=>");
  Serial.print( hc.dist(i) );
  Serial.print(", ");
  } 
  Serial.println("");
  }//return curent distance in serial for sensor 1 to 6
