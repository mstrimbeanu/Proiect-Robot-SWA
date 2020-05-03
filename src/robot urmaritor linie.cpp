#define senzor1 !digitalRead (9)
#define senzor2 !digitalRead (7)
#define senzor3 !digitalRead (4)
#define senzor4 !digitalRead (3)
#define senzor5 !digitalRead (2)

  //motoare
#define a1 5
#define a2 10
#define b1 6
#define b2 11

#define carSpeed 150

void inainte()
{
        analogWrite(a1,carSpeed);
        digitalWrite(a2,LOW);
        digitalWrite(b1,LOW);
        analogWrite(b2,carSpeed);
}

void inapoi()
{
        digitalWrite(a1,LOW);
        analogWrite(a2,carSpeed);
        analogWrite(b1,carSpeed);
        digitalWrite(b2,LOW);
}

void dreapta()
{
        digitalWrite(a1,LOW);
        digitalWrite(a2,LOW);
        digitalWrite(b1,LOW);
        analogWrite(b2,carSpeed);
}

void stanga()
{
        analogWrite(a1,carSpeed);
        digitalWrite(a2,LOW);
        digitalWrite(b1,LOW);
        digitalWrite(b2,LOW);
}
void stop()
{
         digitalWrite(a1,LOW);
         digitalWrite(a2,LOW);
         digitalWrite(b1,LOW);
         digitalWrite(b2,LOW);
}

void setup() {
 // put your setup code here, to run once:
 Serial.begin(9600);
 pinMode(senzor1, INPUT);
 pinMode(senzor2, INPUT);
 pinMode(senzor3, INPUT);
 pinMode(senzor4, INPUT);
 pinMode(senzor5, INPUT);
 
 //motoare
    pinMode(b1, OUTPUT);
    pinMode(b2, OUTPUT);
    pinMode(a1, OUTPUT);
    pinMode(a2, OUTPUT);
}

void loop() {
 // put your main code here, to run repeatedly:
 
  if (senzor3)
  {
    inainte();
  }
  else if (senzor2 && !senzor3 && !senzor4)
  {
    stanga();
  }
  else if (senzor1 && !senzor3 && !senzor4)
  {
    stanga();
  }
  else if (!senzor2 && !senzor3 && senzor4)
  {
    dreapta();
  }
  else if (!senzor2 && !senzor3 && senzor5)
  {
    dreapta();
  }
  else if(senzor2 && senzor3 && senzor4)
  {
    inainte();
  }
  else
  {
    stop();
  }
  
}
