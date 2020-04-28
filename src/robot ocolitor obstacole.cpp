#include <Servo.h>

//definire senzor distanta
#define ECHO_PIN A4
#define TRIG_PIN A5

//definire motoare
#define a1 5
#define a2 10
#define b1 6
#define b2 11


//definire pin servomotor
#define PIN_Servo 9
//servomotor
Servo servoMotor;

int stangaDistanta = 0, dreaptaDistanta = 0, inainteDistanta = 0;
int Trig;
int Echo;
int vitezaSunet = 342;
long durata;
float distanta;

void inainte()
{
        digitalWrite(a1,HIGH);
        digitalWrite(a2,LOW);
        digitalWrite(b1,LOW);
        digitalWrite(b2,HIGH);
}

void inapoi()
{
        digitalWrite(a1,LOW);
        digitalWrite(a2,HIGH);
        digitalWrite(b1,HIGH);
        digitalWrite(b2,LOW);
}

void dreapta()
{
        digitalWrite(a1,LOW);
        digitalWrite(a2,LOW);
        digitalWrite(b1,LOW);
        digitalWrite(b2,HIGH);
}

void stanga()
{
        digitalWrite(a1,HIGH);
        digitalWrite(a2,LOW);
        digitalWrite(b1,LOW);
        digitalWrite(b2,LOW);
}
//functia stop pentru ca masina sa fie orpita cand incepem testarea
void stop()
{
         digitalWrite(a1,LOW);
         digitalWrite(a2,LOW);
         digitalWrite(b1,LOW);
         digitalWrite(b2,LOW);
}

// test de masurare snezor ultrasonic
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
  // put your setup code here, to run once:
  //servo motor
  servoMotor.attach(9);
  // senzor ultrasunete
  pinMode(Echo, INPUT);
  pinMode(Trig , OUTPUT); 
  //motoare 
  pinMode(b1, OUTPUT);
  pinMode(b2, OUTPUT);
  pinMode(a1, OUTPUT);
  pinMode(a2, OUTPUT);
  stop();
}

void loop() {
  // put your main code here, to run repeatedly:
 
  servoMotor.write(90); //initializarea servo inainte
  delay(500);
  inainteDistanta = testDistanta();

  if(inainteDistanta <= 10)
  {
    stop();
    delay(500);
    servoMotor.write(0);
    delay(1000);
    dreaptaDistanta = testDistanta();
    delay(500);
    servoMotor.write(180);
    delay(1000);
    stangaDistanta = testDistanta();
    //mutam servo la 90 grade, inainte
    delay(500);
    servoMotor.write(90);

    if(dreaptaDistanta > stangaDistanta)
    {
      dreapta(); 
      delay(360);
    }
    else if(dreaptaDistanta < stangaDistanta)
    {
      stanga(); 
      delay(360);
    }
    else if((stangaDistanta <=10 || dreaptaDistanta <= 10))
    {
      inapoi(); 
      delay(180);
    }
    else
    {
      inainte();
    }
  }
}
