#include <Servo.h>
Servo servoMotor;

int Echo = A3;  
int Trig = A2; 
int vitezaSunet = 342;
long durata;
float distanta;

//definire motoare
#define a1 5
#define a2 10
#define b1 6
#define b2 11
#define vitezaMasina 150
//definire pin servomotor
#define PIN_Servo 12

int stangaDistanta = 0, dreaptaDistanta = 0, inainteDistanta = 0;;

void inainte(){ 
        analogWrite(a1,vitezaMasina);
        digitalWrite(a2,LOW);
        digitalWrite(b1,LOW);
        analogWrite(b2,vitezaMasina);
}

void inapoi() {
        digitalWrite(a1,LOW);
        analogWrite(a2,vitezaMasina);
        analogWrite(b1,vitezaMasina);
        digitalWrite(b2,LOW);
}

void stanga() {
        analogWrite(a1,vitezaMasina);
        digitalWrite(a2,LOW);
        digitalWrite(b1,LOW);
        digitalWrite(b2,LOW); 
}

void dreapta() {
        digitalWrite(a1,LOW);
        digitalWrite(a2,LOW);
        digitalWrite(b1,LOW);
        analogWrite(b2,vitezaMasina);
}

void stop() {
         digitalWrite(a1,LOW);
         digitalWrite(a2,LOW);
         digitalWrite(b1,LOW);
         digitalWrite(b2,LOW);
} 

//functie testare distanta 
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

void setup() { 
  servoMotor.attach(12);  
  Serial.begin(9600);     
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  stop();
} 

void loop() { 
    servoMotor.write(90);  //pozitionare servo motor in fata
    delay(500); 
    inainteDistanta = testDistanta();

    if(inainteDistanta <= 40) {     
      stop();
      delay(500);                         
      servoMotor.write(10);          
      delay(1000);      
      dreaptaDistanta = testDistanta();
      
     // delay(500);
     // servoMotor.write(90);              
      delay(1000);                                                  
      servoMotor.write(180);              
      delay(1000); 
      stangaDistanta = testDistanta();
      
      delay(500);
      servoMotor.write(90);              
      delay(1000);
      if(dreaptaDistanta > stangaDistanta) {
        dreapta();
        delay(360);
      }
      else if(dreaptaDistanta < stangaDistanta) {
        stanga();
        delay(360);
      }
      else if((dreaptaDistanta <= 40) || (stangaDistanta <= 40)) {
        inapoi();
        delay(180);
      }
      else {
        inainte();
      }
    }  
    else {
        inainte();
    }                     
}
