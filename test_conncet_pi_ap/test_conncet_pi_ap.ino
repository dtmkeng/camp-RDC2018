#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

char auth[] = "d35111a6ae4d450aa6e40972fbab7840"; // Mobile SSID
char ssid[] = "RDC_2018-2.4";
char pass[] = "12345678";
void setup()
{ Serial.begin(115200);
  // Blynk.begin(auth, ssid, pass);
  Blynk.begin(auth, ssid, pass, IPAddress(192, 168, 2, 103), 8080); // Yellow_IP
}

void loop()
{ Blynk.run();
}
