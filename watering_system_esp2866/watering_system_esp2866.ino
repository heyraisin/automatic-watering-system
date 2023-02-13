#include <ESP8266WiFi.h>
#include "ThingSpeak.h"

#define waterpump D5 // d5
#define moisture_Pin A0 // d1

// Set up wifi
// const char* ssid = "Nha_cua_Hien_My_Ne";   
// const char* password = "MatKhaukhodoan813"; 

const char* ssid = "IKIGAI";   
const char* password = "tuoicaydi"; 
WiFiClient  client;

// Connect web application - ThinkSpeak
unsigned long myChannelNumber = 1;
const char * myWriteAPIKey = "BKRWAHHFC7VCWECM";

 // d1 - Soil Moisture Sensor input at Analog PIN A0
int moisture_value= 0;
int dry_value = 10, wet_value = 20;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 15000;

void setup() {
  // Open Serial Monitor
  pinMode(waterpump,OUTPUT);
  digitalWrite(waterpump, HIGH); 
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

    
    moisture_value= analogRead(moisture_Pin);
    Serial.print("MOISTURE LEVEL Origin: ");
    Serial.println(moisture_value);

    moisture_value = map(moisture_value, 0, 1024, 0, 100);
    // Calculate moisture level
    moisture_value = (moisture_value - 100) * -1;
    
    Serial.print("MOISTURE LEVEL : ");
    Serial.println(moisture_value);

    if(moisture_value <= dry_value)  // change this at what level the pump turns on
    {
      Serial.println("Nearly dry, Pump turning on");
      digitalWrite(waterpump, LOW);  // Low percent high signal to relay to turn on pump
    }
    else if(moisture_value >= wet_value) // max water level should be
    {
      Serial.println("Nearly wet, Pump turning off");
      digitalWrite(waterpump, HIGH);  // high percent water high signal to relay to turn on pump
    }

    int x = ThingSpeak.writeField(myChannelNumber, 1, moisture_value, myWriteAPIKey);

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