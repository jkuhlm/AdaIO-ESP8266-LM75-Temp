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

LM75A lm75a_sensor; // Create I2C LM75A instance

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);


// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperatureF = io.feed("diningroom-temperatureF");
float lastSavedTemperature = 0.0;

void setup() {

  // initilize display object
  u8g2.begin();

  
  // connect to io.adafruit.com
  io.connect();

  // wait for a connection
  while(io.status() < AIO_CONNECTED) {
    delay(500);
  }

  

}

void loop() {

  // io.run(); is required for all sketches.
  // it should always be present at the top of your loop
  // function. it keeps the client connected to
  // io.adafruit.com, and processes any incoming data.
  WiFi.hostname("ESP_DiningRoom");
  io.run();

  float fahrenheit = lm75a_sensor.getTemperatureInFahrenheit();
  String displayMessage = "";
  


  // Check if any reads failed and exit early (to try again).
  if (fahrenheit == INVALID_LM75A_TEMPERATURE) {
    return;
  }
 

 // save temperatuere Adafruit IO if have gine up or down by 0.5f
 if (fahrenheit > lastSavedTemperature+0.5 || fahrenheit < lastSavedTemperature-0.5){
    temperatureF->save(fahrenheit);
    lastSavedTemperature = fahrenheit;
  }

  displayMessage = displayMessage + String(fahrenheit);
  displayMessage = displayMessage + "f";

  u8g2.clearBuffer();         // clear the internal memory
  u8g2.setFont(u8g2_font_logisoso32_tf); // choose a suitable font
  u8g2.setCursor(0, 50);
  u8g2.print(displayMessage);  // write something to the internal memory
//  u8g2.setCursor(0, 24);

//  u8g2.print(WiFi.hostname());
  u8g2.sendBuffer();          // transfer internal memory to the display

  delay(5000);

}

