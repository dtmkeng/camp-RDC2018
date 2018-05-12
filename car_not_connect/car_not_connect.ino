#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ArduinoOTA.h> 
#include <SimpleTimer.h>      // Essential for almost all sketches
SimpleTimer timer;

char auth[] = "d35111a6ae4d450aa6e40972fbab7840";
char ssid[] = "RDC_2018-2.4";
char pass[] = "12345678";
int motorA ; // Left motor connected at D2 and D4 WeMos Mini D1
int motorB ; // Right motor connected at D5 and D7 WeMos Mini D1
int X=512;
int Y=512;
int factor=0; // Variable to reduce speed of the motorA or B and turn.
int maximo=0;
bool isFirstConnect = true;


void reconnectBlynk() {                         // reconnect to server if disconnected
  if (!Blynk.connected()) {
    if(Blynk.connect()) {
      BLYNK_LOG("Reconnected");
    } else {
      BLYNK_LOG("Not reconnected");
    }
  }
}


BLYNK_CONNECTED() {
if (isFirstConnect) {
Blynk.syncAll();
Blynk.notify("LET'S GO MATE!!!!");
isFirstConnect = false;
}
}

void setup()
{
  Serial.begin(115200);
  Serial.println("\n Starting");
  //Blynk.begin(auth, ssid, pass);              // normal Blynk Cloud server connection     
  //Blynk.config(auth, cloudserver);            // for Blynk's cloud server if WiFi already connected
  Blynk.begin(auth, ssid, pass);   // for a local server requiring WiFi connection
  int mytimeout = millis() / 1000;
  while (Blynk.connect(1000) == false) {        // wait here until connected to the server
    if((millis() / 1000) > mytimeout + 8){      // try to connect to the server for less than 9 seconds
      break;                                    // continue with the sketch regardless of connection to the server
    }
  }  
//  ArduinoOTA.setHostname(OTAhost);              // for local OTA updates
  //ArduinoOTA.begin();                           // for local OTA updates
  timer.setInterval(15000, reconnectBlynk); // check every 15 seconds if we are connected to the server

 pinMode(D3, OUTPUT); 
 pinMode(D4, OUTPUT);
 pinMode(D5, OUTPUT); 
 pinMode(D6, OUTPUT);
}


BLYNK_WRITE(V0) 
{
 int X1 = param.asInt();
 X=X1;
 Blynk.virtualWrite(V3, X); 
}

 BLYNK_WRITE(V1)
{
 int Y1 = param.asInt();
 Y=Y1;
 Blynk.virtualWrite(V4, Y);
}

 BLYNK_WRITE(V2)//      slider  from 600 to 1023!!!!
{
 int vel = param.asInt(); 
 maximo=vel;
}

void loop(){
  
  if(X == 512  &&  Y == 512)  //  Both Motors stopped.
  {
   motorA = 0;
   motorB = 0;
   analogWrite(D3, motorA);  //Entrada del L293D
   analogWrite(D4, motorA);  //Entrada del L293D
   analogWrite(D5, motorB); //Entrada del L293D
   analogWrite(D6, motorB); //Entrada del L293D
   } 
   
   if(X >= 513 && Y == 512)   // Both Motors Moving forward
  {
    motorA = X;
    motorB = X;
    
    motorA = map(motorA, 513,1023,400,maximo);
    analogWrite(D3, motorA);
    analogWrite(D4,0);
    motorB = map(motorB, 513,1023,400,maximo);
    analogWrite(D6, motorB);
    analogWrite(D5,0);
  }
   if(X >= 513 && Y <= 511)   //Turning Right Forward
  {
    motorA = X;
    motorB = X;
    factor = Y;
    factor= map(factor,0,511,400,0);
    
    motorA = map(motorA, 513,1023,400,maximo);
    analogWrite(D3, motorA);
    analogWrite(D4,0);
    motorB = map(motorB, 513,1023,400,maximo);
    analogWrite(D6, (motorB-factor));
    analogWrite(D5,0);
  }
   if(X >= 513 && Y >= 513)   //Turning Left Forward
  {
    motorA = X;
    motorB = X;
    factor = Y;
    factor= map(factor,513,1013, 0,400);
    
    motorA = map(motorA, 513,1023 , 400,maximo);
    analogWrite(D3, (motorA-factor));
    analogWrite(D4,0);
    motorB = map(motorB, 513,1023 , 400,maximo);
    analogWrite(D6, motorB);
    analogWrite(D5,0);
  }
  if(X <= 511 && Y ==512)   //Backwards
  {
    motorA = X;
    motorB = X;
    
    motorA = map(motorA, 0,511 , maximo,400);
    analogWrite(D4, motorA);
    analogWrite(D3,0);
    motorB = map(motorB, 0,511 , maximo,400);
    analogWrite(D5, motorB);
    analogWrite(D6,0);
  }
  if(X <= 511 && Y <=511)   //Turning Right Backwards
  {
    motorA = X;
    motorB = X;
    factor = Y;
    factor= map(factor,0,511, 400,0);
    
    motorA = map(motorA, 0,511 , maximo,400);
    analogWrite(D4, motorA);
    analogWrite(D3,0);
    motorB = map(motorB, 0,511 , maximo,400);
    analogWrite(D5, (motorB-factor));
    analogWrite(D6,0);
  }
  if(X <= 511 && Y >=513)   //Turning Left Backwards
  {
    motorA = X;
    motorB = X;
    factor = Y;
    factor= map(factor,513,1023, 0,400);
    
    motorA = map(motorA, 0,511 , maximo,400);
    analogWrite(D4, (motorA-factor));
    analogWrite(D3,0);
    motorB = map(motorB, 0,511 , maximo,400);
    analogWrite(D5, motorB);
    analogWrite(D6,0);
  }

if (Blynk.connected()) {   // to ensure that Blynk.run() function is only called if we are still connected to the server
    Blynk.run();
  }
  timer.run();
  ArduinoOTA.handle();       // for local OTA updates
}
