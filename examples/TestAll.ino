#include <Arduino.h>
#include <Wire.h>
#include <i2cdetect.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_DotStar.h>
#include "Adafruit_SHT31.h"
#include "Adafruit_LTR329_LTR303.h"

// There is only one pixel on the board
#define NUMPIXELS 1

//Use these pin definitions for the ItsyBitsy M4
#define DATAPIN    33
#define CLOCKPIN   21

Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);

Adafruit_LTR329 ltr = Adafruit_LTR329();
Adafruit_SHT31 sht31 = Adafruit_SHT31();
Adafruit_LIS3DH lis = Adafruit_LIS3DH();

long firstPixelHue = 0;
int counter = 0;

void setup() {
  Serial.begin(9600);
  Wire.begin(3, 4);
  delay(1500);
  Serial.println("ready?");
  delay(1500);

  if (! lis.begin(0x18)) {   // change this to 0x19 for alternative i2c address
    Serial.println("Couldnt start I2C. Please make sure you have installed the 2.0.4 version og the esp32 library by Espressif Systems?");
    while (1) yield();
  }
  Serial.println("LIS3DH found!");

  if (! sht31.begin(0x44)) {   // Set to 0x45 for alternate i2c addr
    Serial.println("Couldn't find SHT31");
    while (1) delay(1);
  }
  Serial.println("Found SHT31 sensor!");

  if ( ! ltr.begin() ) {
    Serial.println("Couldn't find LTR sensor!");
    while (1) delay(10);
  }
  Serial.println("Found LTR sensor!");
  // Set gain of 1 (see advanced demo for all options!
  ltr.setGain(LTR3XX_GAIN_4);
  // Set integration time of 50ms (see advanced demo for all options!
  ltr.setIntegrationTime(LTR3XX_INTEGTIME_50);
  // Set measurement rate of 50ms (see advanced demo for all options!
  ltr.setMeasurementRate(LTR3XX_MEASRATE_50);

  strip.begin(); // Initialize pins for output
  strip.setBrightness(20);
  strip.show();  // Turn all LEDs off ASAP

  i2cdetect();

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  // RGB led
  rainbow();

  if( counter%50==0 ) 
  {
    //Blink builtin LED
    digitalWrite( 13, !digitalRead(13) );
    
    // Temp & humid
    float t = sht31.readTemperature();
    float h = sht31.readHumidity();
  
    if (! isnan(t)) {  // check if 'is not a number'
      Serial.print("Temp *C = "); Serial.print(t);
    } else {
      Serial.println("Failed to read temperature");
    }
  
    if (! isnan(h)) {  // check if 'is not a number'
      Serial.print("\tHum. % = "); Serial.print(h);
    } else {
      Serial.println("Failed to read humidity");
    }
  
    // Light
    uint16_t light = ltr.readVisible();
    Serial.print("\tLight: ");
    Serial.print(light);

    // Accel
    lis.read();      // get X Y and Z data at once
    Serial.print("\tX:  "); Serial.print(lis.x);
    Serial.print("\tY:  "); Serial.print(lis.y);
    Serial.print("\tZ:  "); Serial.print(lis.z);
  
    Serial.println("");
  }
  else
  {
    // Just wait a little so the LED have time to update
    delay(5);
  }
  
  counter++;
}

void rainbow() {
  strip.setPixelColor(0, strip.gamma32(strip.ColorHSV(firstPixelHue)));
  strip.show(); // Update strip with new contents
  firstPixelHue += 32;
  if ( firstPixelHue > 5 * 65536 ) { firstPixelHue = 0; }
}
