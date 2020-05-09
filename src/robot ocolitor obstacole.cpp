#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

Servo servoMotor;
LiquidCrystal_I2C lcd(0x3f,16,2);  // setare LCD display (0x27 for a 16 chars and 2 line display)

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

// caracter ecran lcd
byte sageataInainte[] = 
{
  B00000,
  B00100,
  B01110,
  B11111,
  B01110,
  B01110,
  B01110,
  B01110
};
// caracter ecran lcd
byte sageataInapoi[] = 
{
  B00000,
  B01110,
  B01110,
  B01110,
  B01110,
  B11111,
  B01110,
  B00100
};
// caracter ecran lcd
byte sageataDreapta[] = 
{
  B00000,
  B00100,
  B11110,
  B11111,
  B11110,
  B00100,
  B00000,
  B00000
};
// caracter ecran lcd
byte sageataStanga[] = 
{
  B00000,
  B00100,
  B01111,
  B11111,
  B01111,
  B00100,
  B00000,
  B00000
};

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

// functie pentru afisare mesaj la pornire robot
void robotPornit()
{                     
  lcd.init();// initialize the lcd
  // afisare mesaj
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print("Hello !");
}
//functie afisare lcd directie deplasare
void lcdStanga()
{
  lcd.init();
  lcd.backlight();
//  lcd.setCursor(0,0); 
//  lcd.print("Hello !");
  lcd.setCursor(0,1);
  lcd.print("Stanga ");
  lcd.setCursor(11, 1); //sageata stanga 
  lcd.write(2);
}
//functie afisare lcd directie deplasare
void lcdDreapta()
{
  lcd.init();
  lcd.backlight();
//  lcd.setCursor(0,0); 
//  lcd.print("Hello !");
  lcd.setCursor(0,1);
  lcd.print("Dreapta ");
  lcd.setCursor(15, 1); //sageata dreapta 
  lcd.write(3);
}
//functie afisare lcd directie deplasare
void lcdInainte()
{
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
 // lcd.setCursor(0,0); 
 // lcd.print("Hello !");
  lcd.setCursor(0,1);
  lcd.print("Inainte ");
  lcd.setCursor(13, 1); //sageata inaninte
  lcd.write(0);
}
//functie afisare lcd directie deplasare
void lcdInapoi()
{
  lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
//  lcd.setCursor(0,0); 
//  lcd.print("Hello !");
  lcd.setCursor(0,1);
  lcd.print("Inapoi ");
  lcd.setCursor(12, 1); //sageata inapoi 
  lcd.write(1);
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
  
  //initializare lcd display
  lcd.init();
  lcd.backlight();
//lcd.setCursor(1,0); 
//lcd.print("Hello !");

  //afisare caractere lcd
  lcd.createChar(0, sageataInainte);
  lcd.createChar(1, sageataInapoi);
  lcd.createChar (2,sageataStanga);
  lcd.createChar (3,sageataDreapta);

  
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
		lcdDreapta();
        delay(360);
      }
      else if(dreaptaDistanta < stangaDistanta) {
        stanga();
		lcdStanga();
        delay(360);
      }
      else if((dreaptaDistanta <= 40) || (stangaDistanta <= 40)) {
        inapoi();
		lcdInapoi();
        delay(180);
      }
      else {
        inainte();
		lcdInainte();
      }
    }  
    else {
        inainte();
		lcdInainte();
    }                     
}
