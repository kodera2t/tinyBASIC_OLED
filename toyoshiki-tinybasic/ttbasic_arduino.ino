/*
  TOYOSHIKI Tiny BASIC for Arduino
 (C)2012 Tetsuya Suzuki
 GNU General Public License
 
 Tested in Arduino Uno R3.
 Use UART terminal or serial monitor.
 
 The grammar is the same as
 PALO ALTO TinyBASIC by Li-Chen Wang
 Except 3 point to show below.
 
 (1)The contracted form of the description is invalid.
 
 (2)Force abort key
 PALO ALTO TinyBASIC -> [Ctrl]+[C]
 TOYOSHIKI TinyBASIC -> [ESC]
 NOTE: There is no input means in serial monitor.
 
 (3)Other some beyond my expectations.
 */
 
// Kodera2t Feb. 12, 2015, SPI OLED(SSD1306) support is added..

#include "ttbasic.h"
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

void setup(void){
  	Serial.begin(4800);// opens serial port, sets data rate to 4800 bps (Xbox360 chatpad speed)
}

void loop(void){
  basic();
}


