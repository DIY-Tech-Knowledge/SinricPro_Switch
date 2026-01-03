#include <Arduino.h>
#if defined(ESP8266)
  #include <ESP8266WiFi.h>
#elif defined(ESP32) || defined(ARDUINO_ARCH_RP2040)
  #include <WiFi.h>
#endif

#include "SinricPro.h"
#include "SinricProSwitch.h"

#define WIFI_SSID         "DIY Tech Knowledge"  // Change WIFI_SSID to your WiFi Name.
#define WIFI_PASS         "Diy@123499"  // Change WIFI_PASS to your WiFi password.
#define APP_KEY           "0279a2ab-cd2a-4b1d-a27d-42b330bcc65a"   // Should look like "de0bxxxx-1x3x-4x3x-ax2x-5dabxxxxxxxx" 
#define APP_SECRET        "a403b0b2-cdfe-4a8d-a7ef-a45771910dd1-d428f758-0011-4f39-aae5-1846f222e155"  // Should look like "5f36xxxx-x3x7-4x3x-xexe-e86724a9xxxx-4c4axxxx-3x3x-x5xe-x9x3-333d65xxxxxx" 
#define SWITCH_ID_1       "63e489d51bb4e19c1196f887"  // Should look like "5dc1564130xxxxxxxxxxxxxx"
#define SWITCH_ID_2       "63e50fa91bb4e19c119766b8"
#define SWITCH_ID_3       "63e4d46e5ec7d92a47063dfd"

#define BAUD_RATE         115200                // Change baudrate to your need

#if defined(ESP8266)
  #define RELAYPIN_1        12 //D6
  #define RELAYPIN_2        13 //D7
  #define RELAYPIN_3        14 //D5
#elif defined(ESP32) 
  #define RELAYPIN_1        16
#elif (ARDUINO_ARCH_RP2040)
  #define RELAYPIN_1        6
#endif



bool onPowerState1(const String &deviceId, bool &state) {
 Serial.printf("Bulb turned %s", state?"on":"off");
 Serial.println(" ");
 digitalWrite(RELAYPIN_1, state ? HIGH:LOW);
 return true;
}

bool onPowerState2(const String &deviceId, bool &state) {
 Serial.printf("LED turned %s", state?"on":"off");
 Serial.println(" ");
 digitalWrite(RELAYPIN_2, state ? HIGH:LOW);
 return true;
}
bool onPowerState3(const String &deviceId, bool &state) {
 Serial.printf("TV turned %s", state?"on":"off");
 Serial.println(" ");
 digitalWrite(RELAYPIN_3, state ? HIGH:LOW);
 
 /* If your relay is activated with low signal, change the above to below code
  digitalWrite(RELAYPIN_1, state ? LOW : HIGH); */

 return true; // request handled properly
}

// setup function for WiFi connection
void setupWiFi() {
  Serial.printf("\r\n[Wifi]: Connecting");

  #if defined(ESP8266)
    WiFi.setSleepMode(WIFI_NONE_SLEEP); 
    WiFi.setAutoReconnect(true);
  #elif defined(ESP32)
    WiFi.setSleep(false); 
    WiFi.setAutoReconnect(true);
  #endif

  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    Serial.printf(".");
    delay(250);
  }

  Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
}

// setup function for SinricPro
void setupSinricPro() {
  // add devices and callbacks to SinricPro    
  SinricProSwitch& mySwitch1 = SinricPro[SWITCH_ID_1];
  mySwitch1.onPowerState(onPowerState1);

   SinricProSwitch& mySwitch2 = SinricPro[SWITCH_ID_2];
  mySwitch2.onPowerState(onPowerState2);

   SinricProSwitch& mySwitch3 = SinricPro[SWITCH_ID_3];
  mySwitch3.onPowerState(onPowerState3);
    
  // setup SinricPro
  SinricPro.onConnected([](){ Serial.printf("Connected to SinricPro\r\n"); }); 
  SinricPro.onDisconnected([](){ Serial.printf("Disconnected from SinricPro\r\n"); });
   
  SinricPro.begin(APP_KEY, APP_SECRET);
}

// main setup function
void setup() {
  Serial.begin(BAUD_RATE); 
  Serial.printf("\r\n\r\n");

  pinMode(RELAYPIN_1, OUTPUT);
  pinMode(RELAYPIN_2, OUTPUT);
  pinMode(RELAYPIN_3, OUTPUT);

  setupWiFi();
  setupSinricPro();
}

void loop() {
  SinricPro.handle();
}