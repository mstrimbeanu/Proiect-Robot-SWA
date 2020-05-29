#include <ESP8266WiFi.h>

char* ssid = "TOTOLINK N300RA";
char* password = "sb541505";

int ledRosuPin = D5;
int ledVerdePin = D6;
int ledAlbastruPin = D12;

WiFiServer server(80); // 0 - 65535

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledRosuPin, OUTPUT);
  digitalWrite(ledRosuPin, HIGH); 
  pinMode(ledVerdePin, OUTPUT);
  digitalWrite(ledVerdePin, HIGH); 
  pinMode(ledAlbastruPin, OUTPUT);
  digitalWrite(ledAlbastruPin, HIGH); 

  WiFi.begin(ssid, password);
  Serial.print("Connecting to: ");
  Serial.println(ssid);

  while(WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi Connected!");

  Serial.print("Connect to: ");
  Serial.print("http://");
  Serial.println( WiFi.localIP() );

  server.begin();
}

void loop() {
  WiFiClient client = server.available();
  if(!client) {
    return;
  }

  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

 

  client.println("HTTP/1.1 200 OK"); 
  // folsim protocol http si conexiunea e OK
  client.println("Content-type: text/html");
  client.println(""); // obligatoriu ca sa functioneze site-ul
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  
   if(request.indexOf("/LED=OFF") != -1 ) {
     client.println("<h1>LED ROSU OFF</h1>");
     digitalWrite(ledRosuPin, LOW);
  } else if(request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledRosuPin, HIGH);
    client.println("<h1>LED ROSU ON</h1>");
  }
  
  if(request.indexOf("/LED=OFF") != -1 ) {
     client.println("<h1>LED VERDE OFF</h1>");
     digitalWrite(ledVerdePin, LOW);
  }else if(request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledVerdePin, HIGH);
    client.println("<h1>LED VERDE ON</h1>");
  }  

  if(request.indexOf("/LED=OFF") != -1 ) {
     client.println("<h1>LED ALBASTRU OFF</h1>");
     digitalWrite(ledAlbastruPin, LOW);
  }else if(request.indexOf("/LED=ON") != -1) {
    digitalWrite(ledAlbastruPin, HIGH);
    client.println("<h1>LED ALBASTRU ON</h1>");

  }  
  client.println("<a href='/LED=ON'>Aprinde LED-uri</a><br>");
  client.println("<a href='/LED=OFF'>Stinge LED-uri</a><br>");

  client.println("</html>");
  
}