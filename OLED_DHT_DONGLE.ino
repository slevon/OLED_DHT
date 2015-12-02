/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x64 size display using I2C to communicate
3 pins are required to interface (2 I2C and one reset)

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include "DHT.h"

#define DHTPIN  A0     // what digital pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
DHT dht(DHTPIN, DHTTYPE);

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

void setup()   {                
  Serial.begin(9600);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3D (for the 128x64)
  // init done
  //display.setRotation(2);
  // Clear the buffer.
  display.clearDisplay();

  //DHT
  dht.begin();

  delay(1000);
  Serial.println("Humidity\tTemperature");
}

unsigned long previousMillis = 0;        // will store last time LED was updated
const long interval = 30000;           // interval at which to blink (milliseconds)
char toggle =0;
int minHum  = 100;
float minTemp = 100;
int maxHum  = -1;
float maxTemp = -1;

void loop() {
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float hf = dht.readHumidity();
  int h = (int)(hf+0.5);
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  
  if (isnan(h) || isnan(t) ) {
    Serial.println("Failed to read from DHT sensor!");
    display.invertDisplay(true);
  }else{
      if(minHum >= h){
        minHum = h;
      }
      if(maxHum <= h){
        maxHum = h;
      }
      if(minTemp >= t){
        minTemp = t;
      }
      if(maxTemp <= t){
        maxTemp = t;
      }
    
    display.invertDisplay(false);
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.clearDisplay();
    display.print("Temp:");
    display.setCursor(60,0);
    display.print(t,1);
    display.setTextSize(1);
    display.print("o");
    display.setTextSize(2);
    display.print("C");
    display.setCursor(0,19);
    display.print("Hum:");
    display.setCursor(60,19);
    display.print(h);
    display.print("%");
  
    //Stat:
    display.setTextSize(1);
    display.setCursor(0,42);
    display.print("Min: T:");
    display.print(minTemp,1);
    display.print("C H:");
    display.print(minHum);
    display.print("%");
    display.setCursor(0,52);
    display.print("Max: T:");
    display.print(maxTemp,1);
    display.print("C H:");
    display.print(maxHum);
    display.print("%");
    display.display();
    
    unsigned long currentMillis = millis();
    if(currentMillis - previousMillis >= interval) {
      //show activity
      display.fillCircle(display.width()-4, display.height()-4, 2, WHITE);
      display.display();
      // save the last time 
      previousMillis = currentMillis;
      Serial.print(millis()/1000);
      Serial.print("\t");
      Serial.print(t,1);
      Serial.print("\t");
      Serial.print(h);
      Serial.println("");
      toggle++;
    }
  }

  // Wait a few seconds between measurements.
  delay(3000);

}

