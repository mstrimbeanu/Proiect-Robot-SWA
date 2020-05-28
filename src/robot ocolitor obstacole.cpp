#include <Servo.h> 
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <FastLED.h> //librarie banda led
#include <IRremote.h> //librarie pt IR
#include <DHT.h> // librarie senzor temp 

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
int stangaDistanta = 0, dreaptaDistanta = 0, inainteDistanta = 0;

//defnirea pinului banda LED
#define LED_PIN A1

//definire senzori urmaritori linie
#define senzor1  !digitalRead (9)
#define senzor2  !digitalRead (7)  
#define senzor3  !digitalRead (4)  
#define senzor4  !digitalRead (3)  
#define senzor5  !digitalRead (2)

 //definire coduri IR
#define F 2064  // INAINTE 2 SONY
#define B 3600 // INAPOI 8 SONY
#define L 3088  // STANGA 4 SONY
#define R 2576  // DREAPTA 6 SONY
#define S 528  // STOP 5 SONY
#define buton_1 16 // buton mod evitare obstacole
#define buton_3 1040 //buton mod urmaritor de linie
#define buton_7 1552 //buton modul ir 
int pin_IR = 13;
IRrecv irrecv(pin_IR);
decode_results results;

unsigned long val;
unsigned long preMillis;

//definire info pentru senzor temp
#define DHTTYPE    DHT11
#define pin_temperatura A0
DHT dht(pin_temperatura, DHTTYPE);

//status masina
int stare_masina;
int stare_anterioara_masina;

/*
// caracter ecran lcd afisare sageti, nu apar mereu sub forma de sageata pe LCD
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
*/
//semnal banda LED RGB ok
void semnalVerde ()
{
  leds[0] = CRGB(0, 255, 68); // culoarea verde
  FastLED.show(); // afisare
  
  leds[1] = CRGB(0, 255, 68); // culoarea verde
  FastLED.show(); // afisare

  leds[2] = CRGB(0, 255, 68); // culoarea verde
  FastLED.show(); // afisare

  leds[3] = CRGB(0, 255, 68); // culoarea verde
  FastLED.show(); // afisare

  leds[4] = CRGB(0, 255, 68); // culoarea verde
  FastLED.show(); // afisare

  leds[5] = CRGB(0, 255, 68); // culoarea verde
  FastLED.show(); // afisare

  leds[6] = CRGB(0, 255, 68); // culoarea verde
  FastLED.show(); // afisare

  leds[7] = CRGB(0, 255, 68); // culoarea verde
  FastLED.show(); // afisare
}

//semnal banda LED RGB stop
void ledRosu()
{
  leds[0] = CRGB(255,0,0); // culoarea rosu
  FastLED.show(); // afisare
  
  leds[1] = CRGB(255,0,0); // culoarea rosu
  FastLED.show(); // afisare

  leds[2] = CRGB(255,0,0); // culoarea rosu
  FastLED.show(); // afisare

  leds[3] = CRGB(255,0,0); // culoarea rosu
  FastLED.show(); // afisare

  leds[4] = CRGB(255,0,0); // culoarea rosu
  FastLED.show(); // afisare

  leds[5] = CRGB(255,0,0); // culoarea rosu
  FastLED.show(); // afisare

  leds[6] = CRGB(255,0,0); // culoarea rosu
  FastLED.show(); // afisare

  leds[7] = CRGB(255,0,0); // culoarea rosu
  FastLED.show(); // afisare
}

void ledModComandaUrmaritorLinie()
{
  leds[0] = CRGB(255, 255, 0); // culoarea galben
  FastLED.show(); // afisare
  
  leds[1] = CRGB(255, 255, 0); // culoarea galben
  FastLED.show(); // afisare

  leds[2] = CRGB(255, 255, 0); // culoarea galben
  FastLED.show(); // afisare

  leds[3] = CRGB(255, 255, 0); // culoarea galben
  FastLED.show(); // afisare

  leds[4] = CRGB(255, 255, 0); // culoarea galben
  FastLED.show(); // afisare

  leds[5] = CRGB(255, 255, 0); // culoarea galben
  FastLED.show(); // afisare

  leds[6] = CRGB(255, 255, 0); // culoarea galben
  FastLED.show(); // afisare

  leds[7] = CRGB(255, 255, 0); // culoarea galben
  FastLED.show(); // afisare
}

//semnalizare dreapta banda LED RGB
void semnalDreapta()
{
    int a = 0 ;
    for (int a = 8; a >=0; a--)
    {
      leds[a] = CRGB(255, 166, 0);
      FastLED.show();
      delay(20);
      leds[a] = CRGB(0,0,0);
      FastLED.show();
      delay(20);
    }
}
//semnalizare stanga banda LED RGB
void semnalStanga()
{
     int a = 0 ;
     for (int a = 0; a <= 8; a++)
    {
      leds[a] = CRGB(255, 166, 0);
      FastLED.show();
      delay(20);;
      leds[a] = CRGB(0, 0, 0);
      FastLED.show();
      delay(20);
    }

}
//semnal stanga banda led rgb 3 ori
void semnalStanga3()
{
  semnalStanga();
  delay(200);
  semnalStanga();
  delay(200);
  semnalStanga();
  delay(200);
}

//semnal dreapta banda led rgb 3 ori
void semnalDreapta3()
{
  semnalDreapta();
  delay(200);
  semnalDreapta();
  delay(200);
  semnalDreapta();
  delay(200);
}
//functie mers inainte
void inainte(){ 
        digitalWrite(pin_motor_a1,HIGH);
        digitalWrite(pin_motor_a2,LOW);
        digitalWrite(pin_motor_b1,LOW);
        digitalWrite(pin_motor_b2,HIGH);
      //  analogWrite(pin_motor_a1,vitezaMasina); se foloseste pentru o viteza mai mica.
      //  analogWrite(pin_motor_b2,vitezaMasina);
}
//functie mers inapoi
void inapoi() {
        digitalWrite(pin_motor_a1,LOW);
        digitalWrite(pin_motor_a2,HIGH);
        digitalWrite(pin_motor_b1,HIGH);
        digitalWrite(pin_motor_b2,LOW);
      //  analogWrite(pin_motor_a2,vitezaMasina); se foloseste pentru o viteza mai mica.
      //  analogWrite(pin_motor_b1,vitezaMasina);
        
}
//functie mers stanga
void stanga() {
        digitalWrite(pin_motor_a1,LOW);
        digitalWrite(pin_motor_a2,LOW);
        digitalWrite(pin_motor_b1,LOW);
        digitalWrite(pin_motor_b2,HIGH); 
      //  analogWrite(pin_motor_b2,vitezaMasina); se foloseste pentru o viteza mai mica
}
//functie mers dreapta
void dreapta() {
        digitalWrite(pin_motor_a1,HIGH);
        digitalWrite(pin_motor_a22,LOW);
        digitalWrite(pin_motor_b1,LOW);
        digitalWrite(pin_motor_b2,LOW);
      //  analogWrite(pin_motor_a1,vitezaMasina);
        
}
//functie oprire
void stop() {
         digitalWrite(pin_motor_a1,LOW);
         digitalWrite(pin_motor_a2,LOW);
         digitalWrite(pin_motor_b1,LOW);
         digitalWrite(pin_motor_b2,LOW);
} 

// functie pentru afisare mesaj la pornire robot
void robotPornit()
{                     
  lcd.init();// initialize LCD
  lcd.backlight();
  lcd.setCursor(0,0); 
  lcd.print("Hello !");
}
//functie afisare lcd directie deplasare
void lcdStanga()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(9,0); //afisare mesaj randul 1, casuta a 9 a.
  lcd.print("Stanga ");
}
//functie afisare lcd directie deplasare
void lcdDreapta()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(9,0);
  lcd.print("Dreapta ");
}
//functie afisare lcd directie deplasare
void lcdInainte()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(9,0);
  lcd.print("Inainte ");
}
//functie afisare lcd directie deplasare
void lcdInapoi()
{
  lcd.init();
  lcd.backlight();
  lcd.setCursor(9,0);
  lcd.print("Inapoi ");
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

//functie preuare si afisare temperatura LCD
void afisare_temperatura()
{ 
  lcd.backlight();
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  lcd.setCursor(0,1); // Setare cursor pentru incepere scriere pe LCD, randul 2 ,prima casuta.
  lcd.print("T-");
  lcd.print(t); // afisare temperatura
  lcd.print("C");
  lcd.setCursor(9,1); // Setare cursor pentru incepere scriere pe LCD
  lcd.print("H-");
  lcd.print(h); // afisare umiditate
  lcd.print("%");
}



// functie pentru robotul ocolitor de obstacole
void ocolitor_obstacole()
{
  servoMotor.attach(12,700,2400);  // attach servo on pin 3 to servo object
  robotPornit(); // afisare mesaj la pornire robot
  afisare_temperatura();
  stare_masina = buton_1;
    servoMotor.write(90);  //setservo position according to scaled value
    delay(500); 
    inainteDistanta = testDistanta();

    if(inainteDistanta <= 30) {     
      stop();
      delay(500); 
      ledRosu();                      
      servoMotor.write(0);          
      delay(1000);      
      dreaptaDistanta = testDistanta();
      
      delay(500);
      servoMotor.write(90);              
      delay(1000);                                                  
      servoMotor.write(180);              
      delay(1000); 
      stangaDistanta = testDistanta();
      
      delay(500);
      servoMotor.write(90);              
      delay(1000);
      if(stangaDistanta > dreaptaDistanta) {
        semnalStanga3();
        stanga();
        lcdStanga();       
        
        delay(150);       
      }
      else if(dreaptaDistanta > stangaDistanta) {
        semnalDreapta3(); 
        dreapta();
        lcdDreapta();      
        
        delay(150);       
      }
      else if((dreaptaDistanta <= 30) || (stangaDistanta <= 30)) {
        inapoi();
        lcdInapoi();       
        delay(180);       
      }
      else {
        semnalVerde();
        inainte();     
        lcdInainte();
         
      }
    }  
    else {
        semnalVerde ();
        inainte();    
        lcdInainte();       
          
    }
    servoMotor.detach();                      
}

//urmaritor de linie
void urmaritor_linie()
{
  
    ledModComandaUrmaritorLinie(); //culoare galbena
    stare_masina = buton_3;
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

// comandare din telecomanda SONY
void comanda_ir()
{
   
  if (irrecv.decode(&results)){ 
    preMillis = millis();
    val = results.value;
    Serial.println(val); 
    stare_masina = buton_7;
    irrecv.resume();  
    switch(val){
      case F: inainte(); semnalVerde(); break;

      case B: inapoi(); break;

      case L: stanga(); semnalStanga3(); break;

      case R: dreapta(); semnalDreapta3(); break;

      case S: stop(); ledRosu();   break;

      default: break;
    }
    irrecv.resume();
  }
  
}

void setup() { 

  Serial.begin(9600);     
  pinMode(Echo, INPUT);    
  pinMode(Trig, OUTPUT);  
  pinMode(pin_motor_b1, OUTPUT);
  pinMode(pin_motor_b2, OUTPUT);
  pinMode(pin_motor_a1, OUTPUT);
  pinMode(pin_motor_a2, OUTPUT);
  stop();
  
  //initializare lcd display
  lcd.init();
  lcd.backlight();
  lcd.setCursor(1,0); 
  lcd.print("Hello !");

  // initializare senzori linie
   pinMode(senzor1, INPUT);
   pinMode(senzor2, INPUT);
   pinMode(senzor3, INPUT);
   pinMode(senzor4, INPUT);
   pinMode(senzor5, INPUT);
   
   dht.begin(); //initializare senzor temp
   
   FastLED.addLeds<WS2812, LED_PIN, GRB>(leds ,NUM_LEDS); // se introduc info legate de led plus formatul oblicatoriu
   
   irrecv.enableIRIn(); //activare IR
   
} 
void loop() { 
  
  // se incarca modul dorit pe arduino
  
	//ocolitor_obstacole();
	//comanda_ir();
	//urmaritor_linie(); 
   
  //sau
  
  if (irrecv.decode(&results)){
    preMillis = millis();
    val = results.value;
    Serial.println(val); 
    irrecv.resume();
  switch(stare_masina)
  {
    case buton_3: urmaritor_linie(); break;
    
    case buton_1: ocolitor_obstacole(); break;

    case buton_7: comanda_ir(); break;

    }
    default:
    stop(); 
 }
}
  
