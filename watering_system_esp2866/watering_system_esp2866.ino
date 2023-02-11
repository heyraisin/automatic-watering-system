/*
  Soil Moisture Sensor Calibration
  soil_calibrate.ino
  Gets raw reading from soil sensor and displays on Serial Monitor
  Use to establish minuimum and maximum values
  Works with Capacitive and Resistive Sensors
  Uses Raspberry Pi Pico

  DroneBot Workshop 2022
  https://dronebotworkshop.com
*/
#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

// const char* ssid = "Nha_cua_Hien_My_Ne";   
// const char* password = "MatKhaukhodoan813"; 

const char* ssid = "The Hideout cafe";   
const char* password = "onemoreplease"; 

WiFiClient  client;

unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "BKRWAHHFC7VCWECM";

int moisture_Pin= 0; // Soil Moisture Sensor input at Analog PIN A0
int moisture_value= 0, moisture_state = 0xFF;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

void setup() {
  // Open Serial Monitor
  Serial.begin(9600);

  ThingSpeak.begin(client);
}

void loop() {

  if ((millis() - lastTime) > timerDelay) {
    
    // Connect or reconnect to WiFi
    if(WiFi.status() != WL_CONNECTED){
      Serial.print("Attempting to connect");

      while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
      Serial.println("\nConnected.");
    }

    Serial.print("MOISTURE LEVEL : ");
    moisture_value= analogRead(moisture_Pin);
    moisture_value= moisture_value/10;
    Serial.println(moisture_value);

    if(moisture_value < 10)  // change this at what level the pump turns on
    {
      Serial.println("Nearly dry, Pump turning on");
      digitalWrite(D5,HIGH);  // Low percent high signal to relay to turn on pump
    }
    else if(moisture_value >85) // max water level should be
    {
      Serial.println("Nearly wet, Pump turning off");
      digitalWrite(D5,low);  // high percent water high signal to relay to turn on pump
    }

    int x = ThingSpeak.writeField(myChannelNumber, 1, moisture_value, myWriteAPIKey);
    
  // if(moisture_value > MOISTURE_THRESHOLD) moisture_state = 0;
  // else moisture_state = 1;

    delay(1000);

    if(x == 200){
        Serial.println("Channel update successful.");
    }
    else{
      Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
    lastTime = millis();

  }
}