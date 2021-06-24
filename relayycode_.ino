 /* #include <Arduino.h>
  #ifdef ESP32
  #include <WiFi.h>
  #define RELAY_PIN_2 12//D6
  #define RELAY_PIN_3 14//D5
  #else
  */
  #include <ESP8266WiFi.h>
  //#define RF_RECEIVER 5
  #define RELAY_PIN_2 15 //D8  
  #define RELAY_PIN_3 3 //RX
  // #endif
#include "fauxmoESP.h"
#define SERIAL_BAUDRATE 115200

#define WIFI_SSID " "
#define WIFI_PASS " "
#define app1 "light one"
#define app2 "light two"
fauxmoESP fauxmo;
 
void wifiSetup() {
  // Set WIFI module to STA mode
  WiFi.mode(WIFI_STA);
  Serial.printf("[WIFI] Connecting to %s ", WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.printf("[WIFI] STATION Mode, SSID: %s, IP address: %s\n", WiFi.SSID().c_str(), WiFi.localIP().toString().c_str());
}

void setup() {
  // Init serial port and clean garbage
  Serial.begin(SERIAL_BAUDRATE);
  Serial.println();

  // Wi-Fi connection
  wifiSetup();

  pinMode(RELAY_PIN_2, OUTPUT);
  digitalWrite(RELAY_PIN_2, HIGH);

  pinMode(RELAY_PIN_3, OUTPUT);
  digitalWrite(RELAY_PIN_3, HIGH);
  
  fauxmo.createServer(true); 
  fauxmo.setPort(80); 
  fauxmo.enable(true);
  fauxmo.addDevice(app1);
  fauxmo.addDevice(app2);

  fauxmo.onSetState([](unsigned char device_id, const char * device_name, bool state, unsigned char value) {    
    Serial.printf("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    if ( (strcmp(device_name, app1) == 0) ) {
      Serial.println("RELAY 2 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_2, LOW);
      } else {
        digitalWrite(RELAY_PIN_2, HIGH);
      }
    }
    if ( (strcmp(device_name, app2) == 0) ) {
      Serial.println("RELAY 3 switched by Alexa");
      if (state) {
        digitalWrite(RELAY_PIN_3, LOW);
      } else {
        digitalWrite(RELAY_PIN_3, HIGH);
      }
    }
  });

}

void loop() {
  fauxmo.handle();

  static unsigned long last = millis();
  if (millis() - last > 5000) {
    last = millis();
    Serial.printf("[MAIN] Free heap: %d bytes\n", ESP.getFreeHeap());
  }
}
 
