#include <testDistanta.h> 
#include "Arduino.h" 

int testDistanta()
{
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  durata = pulseIn (Echo ,HIGH);
  distanta = (durata * 0.0342) / 2; //transformare in cm
  return distanta;
}