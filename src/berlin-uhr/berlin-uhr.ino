#include <Adafruit_TLC5947.h>
#include <DS3231.h>
#include <Wire.h>

DS3231 clock;

void setup() {
  // Start the I2C interface
  Wire.begin();
 
  // Start the serial interface
  Serial.begin(57600);
}

void loop() {
  // put your main code here, to run repeatedly:

}
