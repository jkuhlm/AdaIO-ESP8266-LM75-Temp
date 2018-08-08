// Adafruit IO Temperature & Humidity Example
// Tutorial Link: https://learn.adafruit.com/adafruit-io-basics-temperature-and-humidity
//
// Adafruit invests time and resources providing this open source code.
// Please support Adafruit and open source hardware by purchasing
// products from Adafruit!
//
// Written by Todd Treece for Adafruit Industries
// Copyright (c) 2016-2017 Adafruit Industries
// Licensed under the MIT license.
//
// All text above must be included in any redistribution.

/************************** Configuration ***********************************/

// edit the config.h tab and enter your Adafruit IO credentials
// and any additional configuration needed for WiFi, cellular,
// or ethernet clients.
#include "config.h"
#include <U8g2lib.h>
#include <Wire.h>
#include <LM75A.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Adafruit_BME280.h>
#define SEALEVELPRESSURE_HPA (1013.25)


// pin connected to DH22 data line
#define DATA_PIN D3

Adafruit_BME280 bme; // I2C

// create DHT22 instance
DHT_Unified dht(DATA_PIN, DHT22);

LM75A lm75a_sensor; // Create I2C LM75A instance

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperatureF = io.feed("diningroom-temperatureF");
float lastSavedTemperature = 0.0;

void setup() {
  Serial.begin(9600);

//  bool status = bme.begin();  
//  if (!status) {
//      Serial.println("Could not find a valid BME280 sensor, check wiring!");
//      while (1);
//  }

  // initialize dht22
  dht.begin();
  
  // initilize display object
  u8g2.begin();
  
  // connect to io.adafruit.com
  io.connect();
  
  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    delay(500);
  }

  //Set hostname
  WiFi.hostname("ESP_DiningRoom");
  

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.

  io.run();

  // read DHT22/11
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  float celsius = event.temperature;
  // Check if any reads failed and exit early (to try again).
  if (isnan(celsius)) {
    return;
  }

  float dth22fahrenheit = (celsius * 1.8) + 32;


  float fahrenheit = lm75a_sensor.getTemperatureInFahrenheit();
  String displayMessage = "LM75a = ";
  
  // Check if any reads failed and exit early (to try again).
  if (fahrenheit == INVALID_LM75A_TEMPERATURE) {
    return;
  }

  //float bmefahrenheit = (bme.readTemperature() * 1.8) + 32;
 

 // save temperatuere Adafruit IO if have gine up or down by 0.5f
 if (dth22fahrenheit > lastSavedTemperature+0.5 || dth22fahrenheit < lastSavedTemperature-0.5){
    temperatureF->save(dth22fahrenheit);
    lastSavedTemperature = dth22fahrenheit;
  }

  displayMessage = displayMessage + String(fahrenheit);
  displayMessage = displayMessage + "f";

  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_crox3h_tf); // choose a suitable font
  u8g2.setCursor(0, 16);
  u8g2.print(displayMessage);  // write something to the internal memory

  displayMessage = "DHT = ";
  displayMessage = displayMessage + String(dth22fahrenheit);
  displayMessage = displayMessage + "f";
  u8g2.setCursor(0, 32);
  u8g2.print(displayMessage);  // write something to the internal memory

//  displayMessage = "BME = ";
//  displayMessage = displayMessage + String(bmefahrenheit);
//  displayMessage = displayMessage + "f";
//  u8g2.setCursor(0, 48);
//  u8g2.print(displayMessage);  // write something to the internal memory

  u8g2.sendBuffer();          // transfer internal memory to the display

  delay(5000);

}

