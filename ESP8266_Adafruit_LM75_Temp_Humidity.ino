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

/************************ Example Starts Here *******************************/
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// pin connected to DH22 data line
#define DATA_PIN D3

// create DHT22 instance
DHT_Unified dht(DATA_PIN, DHT22);

Adafruit_SSD1306 display = Adafruit_SSD1306();

#if (SSD1306_LCDHEIGHT != 64)
 #error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// set up the 'temperature' and 'humidity' feeds
AdafruitIO_Feed *temperatureF = io.feed("vent-temperatureF");
float lastSavedTemperature = 0.0;

void setup() {

  // initialize dht22
  dht.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  

  // Show image buffer on the display hardware.
  // Since the buffer is intialized with an Adafruit splashscreen
  // internally, this will display the splashscreen.  display.display();
  display.display();
  delay(1000);

  
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
  io.run();

  sensors_event_t event;
  dht.temperature().getEvent(&event);

  float celsius = event.temperature;

  // Check if any reads failed and exit early (to try again).
  if (isnan(celsius)) {
    return;
  }
 
  float fahrenheit = (celsius * 1.8) + 32;

 // save temperatuere Adafruit IO if have gine up or down by 0.5f
 if (fahrenheit > lastSavedTemperature+0.5 || fahrenheit < lastSavedTemperature-0.5){
    temperatureF->save(fahrenheit);
    lastSavedTemperature = fahrenheit;
  }

  // Clear the buffer.
  display.clearDisplay();
  display.display();
  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.print("Temperature: ");
  display.print(fahrenheit  );
  display.println("F");

  display.display(); // actually display all of the above

  delay(2000);

}

