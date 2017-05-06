//// Simple Nihongo(Japanese) text reader
//// consisting of.... ATMega1284P, OLED-display (SPI), japanese font SPI-ROM (GT20L16J1Y)
//// Author: kodera2t May 16, 2015. this software is distributed under LGPL.
/// usage: Put plain Japanese (JIS0208-coded) text on Fat formatted SD card. Half-width(single byte)
///  character, including 0x20 in text file will lead to display error.
#include <SPI.h> 
#include <Wire.h>
#include <SD.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h> 
#define ASC8x16S 255968
#define ASC8x16N 257504
#define kanji_CS 4
#define kSD_CS 13
byte rawdata[32];

  byte read_char=0x00, old_char;
  int count=0;
  boolean flag=false;
  long unsigned int charact=0,i=0;
  boolean jp_flag=false;
  int xpos, ypos;
// SPI PIN definition. Change to fit your board....
#define OLED_MOSI   5
#define OLED_CLK   7
#define OLED_DC    19
#define OLED_CS    18
#define OLED_RESET 20 ///v3:22, v2:20
//Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);


unsigned long calcAddr(unsigned short jiscode) {
  unsigned long MSB;   
  unsigned long LSB;
  unsigned long Address;
  MSB = (jiscode >> 8) - 0x20;
  LSB = (jiscode & 0xff) -0x20;
  if(MSB >=1 && MSB <= 15 && LSB >=1 && LSB <= 94)
    Address =( (MSB - 1) * 94 + (LSB - 01))*32;
  else if(MSB >=16 && MSB <= 47 && LSB >=1 && LSB <= 94)
    Address =( (MSB - 16) * 94 + (LSB - 1))*32+43584;
  else if(MSB >=48 && MSB <=84 && LSB >=1 && LSB <= 94)
    Address = ((MSB - 48) * 94 + (LSB - 1))*32+ 138464;
  else if(MSB ==85 && LSB >=0x01 && LSB <= 94)
    Address = ((MSB - 85) * 94 + (LSB - 1))*32+ 246944;
  else if(MSB >=88 && MSB <=89 && LSB >=1 && LSB <= 94)
    Address = ((MSB - 88) * 94 + (LSB - 1))*32+ 249952;
  
  return Address;
}

void getCharData(unsigned short code) {
    byte data;
    unsigned long addr=0;
    byte n;
    addr =calcAddr(code);
    n = 32;    
    digitalWrite(kanji_CS, HIGH);
    delayMicroseconds(4);   
    digitalWrite(kanji_CS, LOW);
    SPI.transfer(0x03);
    SPI.transfer((addr>>16) & 0xff);
    SPI.transfer((addr>>8) & 0xff);
    SPI.transfer(addr & 0xff);
    for(byte i = 0;i< n; i++)  {
      rawdata[i] = SPI.transfer(0x00);
    }
    digitalWrite(kanji_CS, HIGH);
}

void displayChar(unsigned short code, unsigned int offset_x,unsigned int offset_y) {
 offset_x=offset_x*16;
 offset_y=offset_y*16;
 getCharData(code);
      for(int x=0; x<32; x++)
    {
        for(int y=0; y<8; y++)
        {
        if (rawdata[x] & (1<<y))
        display.drawPixel(x%16 + offset_x, y+(8*(x>>4)) + offset_y, WHITE);
        else
        display.drawPixel(x%16 + offset_x, y+(8*(x>>4)) + offset_y, BLACK);
        }
    }
    display.display();
}
   
void setup() {
 pinMode(SS, OUTPUT);
 SPI.begin();
 Serial.begin(4800);
   display.begin();
  display.display();
  display.clearDisplay();
  display.fillScreen(0);
    display.clearDisplay();
      if (!SD.begin(kSD_CS)) {
    Serial.println("Card failed, or not present");
    while(1);
  }
  Serial.println("ok");


}

void loop() {
  File dataFile = SD.open("test3.txt");
    if (dataFile) {
     while (dataFile.available()) {
       old_char=read_char;
       read_char=dataFile.read();
        if(i>3){
          if(i%2==0){
        charact=(old_char<<8)|read_char;
        xpos=count-(count/8)*8;
        ypos=count/8;
        displayChar(charact,xpos,ypos);
        count++;//character number
          }
        }
        if(count==32){
          delay(7000);
          count=0;
        }
               i++;
     }   
    dataFile.close();
  }
}
