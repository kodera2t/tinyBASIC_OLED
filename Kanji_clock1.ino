// Kanji Clock with OLED display
// (c) kodera2t June 14 2015
// Referenced website: http://tronixstuff.com/2013/08/13/tutorial-arduino-and-pcf8563-real-time-clock-ic/
//Example 54.1 - PCF2129 RTC write/read demonstration
#include <SPI.h> 
#include "Wire.h"
#define PCF2129address 0xA2>>1
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h> 
#define ASC8x16S 255968
#define ASC8x16N 257504
#define kanji_CS 4 
byte rawdata[32];
int i=0;
int toneflag=0;
byte second, minute, hour, dayOfWeek, dayOfMonth, month, year;
String days[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

// SPI PIN definition. Change to fit your board....
#define OLED_MOSI   5
#define OLED_CLK   7
#define OLED_DC    19
#define OLED_CS    18
#define OLED_RESET 22 ///v3:22, v2:20
//Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
Adafruit_SSD1306 display(OLED_DC, OLED_RESET, OLED_CS);

#include <Tone.h>
Tone tone1;
#define piezo 14
#define OCTAVE_OFFSET 0
#define isdigit(n) (n >= '0' && n <= '9')
int notes[] = { 0,
NOTE_C4, NOTE_CS4, NOTE_D4, NOTE_DS4, NOTE_E4, NOTE_F4, NOTE_FS4, NOTE_G4, NOTE_GS4, NOTE_A4, NOTE_AS4, NOTE_B4,
NOTE_C5, NOTE_CS5, NOTE_D5, NOTE_DS5, NOTE_E5, NOTE_F5, NOTE_FS5, NOTE_G5, NOTE_GS5, NOTE_A5, NOTE_AS5, NOTE_B5,
NOTE_C6, NOTE_CS6, NOTE_D6, NOTE_DS6, NOTE_E6, NOTE_F6, NOTE_FS6, NOTE_G6, NOTE_GS6, NOTE_A6, NOTE_AS6, NOTE_B6,
NOTE_C7, NOTE_CS7, NOTE_D7, NOTE_DS7, NOTE_E7, NOTE_F7, NOTE_FS7, NOTE_G7, NOTE_GS7, NOTE_A7, NOTE_AS7, NOTE_B7
};

////// copy-pasted from Arduino Tone library RTTTL sample//////
char *song1 = "The Simpsons:d=4,o=5,b=160:c.6,e6,f#6,8a6,g.6,e6,c6,8a,8f#,8f#,8f#,2g,8p,8p,8f#,8f#,8f#,8g,a#.,8c6,8c6,8c6,c6";
char *song2 = "Indiana:d=4,o=5,b=250:e,8p,8f,8g,8p,1c6,8p.,d,8p,8e,1f,p.,g,8p,8a,8b,8p,1f6,p,a,8p,8b,2c6,2d6,2e6,e,8p,8f,8g,8p,1c6,p,d6,8p,8e6,1f.6,g,8p,8g,e.6,8p,d6,8p,8g,e.6,8p,d6,8p,8g,f.6,8p,e6,8p,8d6,2c6";
char *song3 = "TakeOnMe:d=4,o=4,b=160:8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5,8f#5,8e5,8f#5,8f#5,8f#5,8d5,8p,8b,8p,8e5,8p,8e5,8p,8e5,8g#5,8g#5,8a5,8b5,8a5,8a5,8a5,8e5,8p,8d5,8p,8f#5,8p,8f#5,8p,8f#5,8e5,8e5";
char *song4 = "Entertainer:d=4,o=5,b=140:8d,8d#,8e,c6,8e,c6,8e,2c.6,8c6,8d6,8d#6,8e6,8c6,8d6,e6,8b,d6,2c6,p,8d,8d#,8e,c6,8e,c6,8e,2c.6,8p,8a,8g,8f#,8a,8c6,e6,8d6,8c6,8a,2d6";
char *song9 = "Muppets:d=4,o=5,b=250:c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,8a,8p,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,8e,8p,8e,g,2p,c6,c6,a,b,8a,b,g,p,c6,c6,a,8b,a,g.,p,e,e,g,f,8e,f,8c6,8c,8d,e,8e,d,8d,c";
char *song8 = "Xfiles:d=4,o=5,b=125:e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,g6,f#6,e6,d6,e6,2b.,1p,g6,f#6,e6,d6,f#6,2b.,1p,e,b,a,b,d6,2b.,1p,e,b,a,b,e6,2b.,1p,e6,2b.";
char *song10 = "Looney:d=4,o=5,b=140:32p,c6,8f6,8e6,8d6,8c6,a.,8c6,8f6,8e6,8d6,8d#6,e.6,8e6,8e6,8c6,8d6,8c6,8e6,8c6,8d6,8a,8c6,8g,8a#,8a,8f";
char *song5 = "20thCenFox:d=16,o=5,b=140:b,8p,b,b,2b,p,c6,32p,b,32p,c6,32p,b,32p,c6,32p,b,8p,b,b,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,b,32p,g#,32p,a,32p,b,8p,b,b,2b,4p,8e,8g#,8b,1c#6,8f#,8a,8c#6,1e6,8a,8c#6,8e6,1e6,8b,8g#,8a,2b";
char *song11 = "Bond:d=4,o=5,b=80:32p,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d#6,16d#6,16c#6,32d#6,32d#6,16d#6,8d#6,16c#6,16c#6,16c#6,16c#6,32e6,32e6,16e6,8e6,16d#6,16d6,16c#6,16c#7,c.7,16g#6,16f#6,g#.6";
char *song0 = "MASH:d=8,o=5,b=140:4a,4g,f#,g,p,f#,p,g,p,f#,p,2e.,p,f#,e,4f#,e,f#,p,e,p,4d.,p,f#,4e,d,e,p,d,p,e,p,d,p,2c#.,p,d,c#,4d,c#,d,p,e,p,4f#,p,a,p,4b,a,b,p,a,p,b,p,2a.,4p,a,b,a,4b,a,b,p,2a.,a,4f#,a,b,p,d6,p,4e.6,d6,b,p,a,p,2b";
char *song6 = "StarWars:d=4,o=5,b=45:32p,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#.6,32f#,32f#,32f#,8b.,8f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32c#6,8b.6,16f#.6,32e6,32d#6,32e6,8c#6";
//char *song = "GoodBad:d=4,o=5,b=56:32p,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,d#,32a#,32d#6,32a#,32d#6,8a#.,16f#.,16g#.,c#6,32a#,32d#6,32a#,32d#6,8a#.,16f#.,32f.,32d#.,c#,32a#,32d#6,32a#,32d#6,8a#.,16g#.,d#";
//char *song = "TopGun:d=4,o=4,b=31:32p,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,16f,d#,16c#,16g#,16g#,32f#,32f,32f#,32f,16d#,16d#,32c#,32d#,16f,32d#,32f,16f#,32f,32c#,g#";
//char *song = "A-Team:d=8,o=5,b=125:4d#6,a#,2d#6,16p,g#,4a#,4d#.,p,16g,16a#,d#6,a#,f6,2d#6,16p,c#.6,16c6,16a#,g#.,2a#";
//char *song = "Flinstones:d=4,o=5,b=40:32p,16f6,16a#,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,d6,16f6,16a#.,16a#6,32g6,16f6,16a#.,32f6,32f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c6,a#,16a6,16d.6,16a#6,32a6,32a6,32g6,32f#6,32a6,8g6,16g6,16c.6,32a6,32a6,32g6,32g6,32f6,32e6,32g6,8f6,16f6,16a#.,16a#6,32g6,16f6,16a#.,16f6,32d#6,32d6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#,16c.6,32d6,32d#6,32f6,16a#6,16c7,8a#.6";
//char *song = "Jeopardy:d=4,o=6,b=125:c,f,c,f5,c,f,2c,c,f,c,f,a.,8g,8f,8e,8d,8c#,c,f,c,f5,c,f,2c,f.,8d,c,a#5,a5,g5,f5,p,d#,g#,d#,g#5,d#,g#,2d#,d#,g#,d#,g#,c.7,8a#,8g#,8g,8f,8e,d#,g#,d#,g#5,d#,g#,2d#,g#.,8f,d#,c#,c,p,a#5,p,g#.5,d#,g#";
//char *song = "Gadget:d=16,o=5,b=50:32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,32d#,32f,32f#,32g#,a#,d#6,4d6,32d#,32f,32f#,32g#,a#,f#,a,f,g#,f#,8d#";
//char *song = "Smurfs:d=32,o=5,b=200:4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8f#,p,8a#,p,4g#,4p,g#,p,a#,p,b,p,c6,p,4c#6,16p,4f#6,p,16c#6,p,8d#6,p,8b,p,4g#,16p,4c#6,p,16a#,p,8b,p,8f,p,4f#";
//char *song = "MahnaMahna:d=16,o=6,b=125:c#,c.,b5,8a#.5,8f.,4g#,a#,g.,4d#,8p,c#,c.,b5,8a#.5,8f.,g#.,8a#.,4g,8p,c#,c.,b5,8a#.5,8f.,4g#,f,g.,8d#.,f,g.,8d#.,f,8g,8d#.,f,8g,d#,8c,a#5,8d#.,8d#.,4d#,8d#.";
//char *song = "LeisureSuit:d=16,o=6,b=56:f.5,f#.5,g.5,g#5,32a#5,f5,g#.5,a#.5,32f5,g#5,32a#5,g#5,8c#.,a#5,32c#,a5,a#.5,c#.,32a5,a#5,32c#,d#,8e,c#.,f.,f.,f.,f.,f,32e,d#,8d,a#.5,e,32f,e,32f,c#,d#.,c#";
char *song7 = "MissionImp:d=16,o=6,b=95:32d,32d#,32d,32d#,32d,32d#,32d,32d#,32d,32d,32d#,32e,32f,32f#,32g,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,g,8p,g,8p,a#,p,c7,p,g,8p,g,8p,f,p,f#,p,a#,g,2d,32p,a#,g,2c#,32p,a#,g,2c,a#5,8c,2p,32p,a#5,g5,2f#,32p,a#5,g5,2f,32p,a#5,g5,2e,d#,8d";











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







byte bcdToDec(byte value)
{
  return ((value / 16) * 10 + value % 16);
}

byte decToBcd(byte value){
  return (value / 10 * 16 + value % 10);
}

void setPCF2129()
// this sets the time and date to the PCF2129
{
  Wire.beginTransmission(PCF2129address);
  Wire.write(0x03);
  Wire.write(decToBcd(second));  
  Wire.write(decToBcd(minute));
  Wire.write(decToBcd(hour));     
  Wire.write(decToBcd(dayOfMonth));
  Wire.write(decToBcd(dayOfWeek));  
  Wire.write(decToBcd(month));
  Wire.write(decToBcd(year));
  Wire.endTransmission();
}

void readPCF2129()
// this gets the time and date from the PCF2129
{
  Wire.beginTransmission(PCF2129address);
  Wire.write(0x03);
  Wire.endTransmission();
  Wire.requestFrom(PCF2129address, 7);
  second     = bcdToDec(Wire.read() & B01111111); // remove VL error bit
  minute     = bcdToDec(Wire.read() & B01111111); // remove unwanted bits from MSB
  hour       = bcdToDec(Wire.read() & B00111111); 
  dayOfMonth = bcdToDec(Wire.read() & B00111111);
  dayOfWeek  = bcdToDec(Wire.read() & B00000111);  
  month      = bcdToDec(Wire.read() & B00011111);  // remove century bit, 1999 is over
  year       = bcdToDec(Wire.read());
}

void setup()
{
    tone1.begin(14);
   pinMode(kanji_CS,OUTPUT);
 digitalWrite(kanji_CS,HIGH);
 //pinMode(6, INPUT);
 SPI.begin();
     display.begin();
  display.display();
  display.clearDisplay();
  display.fillScreen(0);
    display.clearDisplay();
  
  Wire.begin();
  Serial.begin(4800);
  // change the following to set your initial time
  second = 30;
  minute = 45;
  hour = 17;
  dayOfWeek = 0;
  dayOfMonth = 14;
  month = 6;
  year = 15;
  // comment out the next line and upload again to set and keep the time from resetting every reset
  //if(year!=2015){
  //setPCF2129();
  //}

}

void loop()
{
  int second2,second1;
  int minute2,minute1;
  int hour2,hour1;
  int month2,month1;
  int day2,day1;
  int wareki2,wareki1;
  int heisei;
  int tmpsec;
  int tmpday;
  int tmphour;

  readPCF2129();




  if(second!=tmpsec){
  second2=second/10;
  second1=second-second2*10;
  
  switch(second1){
  case(0):
  displayChar(0x4E6D,6,3);
  break;
  case(1):
  displayChar(0x306C,6,3);
  break;
        case(2):
  displayChar(0x4673,6,3);
    break;
        case(3):
  displayChar(0x3B30,6,3);
    break;
        case(4):
  displayChar(0x3B4D,6,3);
    break;
        case(5):
  displayChar(0x385E,6,3);
    break;
        case(6):
  displayChar(0x4F3B,6,3);
    break;
        case(7):
  displayChar(0x3C37,6,3);
    break;
        case(8):
  displayChar(0x482C,6,3);
    break;
        case(9):
  displayChar(0x3665,6,3);
    break;
  }

  switch(second2){
  case(0):
  displayChar(0x2121,4,3);
  displayChar(0x2121,5,3);
  break;
  case(1):
  displayChar(0x2121,4,3);
  displayChar(0x3D3D,5,3);
  break;
        case(2):
  displayChar(0x4673,4,3);
  displayChar(0x3D3D,5,3);
    break;
        case(3):
  displayChar(0x3B30,4,3);
  displayChar(0x3D3D,5,3);
    break;
        case(4):
  displayChar(0x3B4D,4,3);
    displayChar(0x3D3D,5,3);
    break;
        case(5):
  displayChar(0x385E,4,3);
    displayChar(0x3D3D,5,3);
    break;
  }


  
  displayChar(0x4943,7,3);


  minute2=minute/10;
  minute1=minute-minute2*10;
  
  switch(minute1){
  case(0):
  displayChar(0x4E6D,6,2);
  break;
  case(1):
  displayChar(0x306C,6,2);
  break;
        case(2):
  displayChar(0x4673,6,2);
    break;
        case(3):
  displayChar(0x3B30,6,2);
    break;
        case(4):
  displayChar(0x3B4D,6,2);
    break;
        case(5):
  displayChar(0x385E,6,2);
    break;
        case(6):
  displayChar(0x4F3B,6,2);
    break;
        case(7):
  displayChar(0x3C37,6,2);
    break;
        case(8):
  displayChar(0x482C,6,2);
    break;
        case(9):
  displayChar(0x3665,6,2);
    break;
  }

  switch(minute2){
  case(0):
  displayChar(0x2121,4,2);
  displayChar(0x2121,5,2);
  break;
  case(1):
  displayChar(0x2121,4,2);
  displayChar(0x3D3D,5,2);
  break;
        case(2):
  displayChar(0x4673,4,2);
  displayChar(0x3D3D,5,2);
    break;
        case(3):
  displayChar(0x3B30,4,2);
  displayChar(0x3D3D,5,2);
    break;
        case(4):
  displayChar(0x3B4D,4,2);
    displayChar(0x3D3D,5,2);
    break;
        case(5):
  displayChar(0x385E,4,2);
    displayChar(0x3D3D,5,2);
    break;
  }
  displayChar(0x4A2C,7,2);
    


  hour2=hour/10;
  hour1=hour-hour2*10;
  
  switch(hour1){
  case(0):
  displayChar(0x4E6D,2,2);
  break;
  case(1):
  displayChar(0x306C,2,2);
  break;
        case(2):
  displayChar(0x4673,2,2);
    break;
        case(3):
  displayChar(0x3B30,2,2);
    break;
        case(4):
  displayChar(0x3B4D,2,2);
    break;
        case(5):
  displayChar(0x385E,2,2);
    break;
        case(6):
  displayChar(0x4F3B,2,2);
    break;
        case(7):
  displayChar(0x3C37,2,2);
    break;
        case(8):
  displayChar(0x482C,2,2);
    break;
        case(9):
  displayChar(0x3665,2,2);
    break;
  }

  switch(hour2){
  case(0):
  displayChar(0x2121,0,2);
  displayChar(0x2121,1,2);
  break;
  case(1):
  displayChar(0x2121,0,2);
  displayChar(0x3D3D,1,2);
  break;
        case(2):
  displayChar(0x4673,0,2);
  displayChar(0x3D3D,1,2);
    break;
        case(3):
  displayChar(0x3B30,0,2);
  displayChar(0x3D3D,1,2);
    break;
        case(4):
  displayChar(0x3B4D,0,2);
    displayChar(0x3D3D,1,2);
    break;
        case(5):
  displayChar(0x385E,0,2);
    displayChar(0x3D3D,1,2);
    break;
  }


  
  displayChar(0x3B7E,3,2);


 // if(tmpday!=dayOfMonth){
  month2=month/10;
  month1=month-month2*10;
  
  switch(month1){
  case(0):
  displayChar(0x4E6D,2,1);
  break;
  case(1):
  displayChar(0x306C,2,1);
  break;
        case(2):
  displayChar(0x4673,2,1);
    break;
        case(3):
  displayChar(0x3B30,2,1);
    break;
        case(4):
  displayChar(0x3B4D,2,1);
    break;
        case(5):
  displayChar(0x385E,2,1);
    break;
        case(6):
  displayChar(0x4F3B,2,1);
    break;
        case(7):
  displayChar(0x3C37,2,1);
    break;
        case(8):
  displayChar(0x482C,2,1);
    break;
        case(9):
  displayChar(0x3665,2,1);
    break;
  }

  switch(month2){
  case(0):
  displayChar(0x2121,0,1);
  displayChar(0x2121,1,1);
  break;
  case(1):
  displayChar(0x2121,0,1);
  displayChar(0x3D3D,1,1);
  break;
        case(2):
  displayChar(0x4673,0,1);
  displayChar(0x3D3D,1,1);
    break;
        case(3):
  displayChar(0x3B30,0,1);
  displayChar(0x3D3D,1,1);
    break;
        case(4):
  displayChar(0x3B4D,0,1);
    displayChar(0x3D3D,1,1);
    break;
        case(5):
  displayChar(0x385E,0,1);
    displayChar(0x3D3D,1,1);
    break;
  }
  displayChar(0x376E,3,1);


  day2=dayOfMonth/10;
  day1=dayOfMonth-day2*10;
  
  switch(day1){
  case(0):
  displayChar(0x4E6D,6,1);
  break;
  case(1):
  displayChar(0x306C,6,1);
  break;
        case(2):
  displayChar(0x4673,6,1);
    break;
        case(3):
  displayChar(0x3B30,6,1);
    break;
        case(4):
  displayChar(0x3B4D,6,1);
    break;
        case(5):
  displayChar(0x385E,6,1);
    break;
        case(6):
  displayChar(0x4F3B,6,1);
    break;
        case(7):
  displayChar(0x3C37,6,1);
    break;
        case(8):
  displayChar(0x482C,6,1);
    break;
        case(9):
  displayChar(0x3665,6,1);
    break;
  }

  switch(day2){
  case(0):
  displayChar(0x2121,4,1);
  displayChar(0x2121,5,1);
  break;
  case(1):
  displayChar(0x2121,4,1);
  displayChar(0x3D3D,5,1);
  break;
        case(2):
  displayChar(0x4673,4,1);
  displayChar(0x3D3D,5,1);
    break;
        case(3):
  displayChar(0x3B30,4,1);
  displayChar(0x3D3D,5,1);
    break;
  }
  displayChar(0x467C,7,1);


  
  
  displayChar(0x4A3F,2,0);
  displayChar(0x402E,3,0); 


  heisei=int(year)+12;
  wareki2=heisei/10;
  wareki1=heisei-wareki2*10;
  
  switch(wareki1){
  case(0):
  displayChar(0x4E6D,6,0);
  break;
  case(1):
  displayChar(0x306C,6,0);
  break;
        case(2):
  displayChar(0x4673,6,0);
    break;
        case(3):
  displayChar(0x3B30,6,0);
    break;
        case(4):
  displayChar(0x3B4D,6,0);
    break;
        case(5):
  displayChar(0x385E,6,0);
    break;
        case(6):
  displayChar(0x4F3B,6,0);
    break;
        case(7):
  displayChar(0x3C37,6,0);
    break;
        case(8):
  displayChar(0x482C,6,0);
    break;
        case(9):
  displayChar(0x3665,6,0);
    break;

  }

  switch(wareki2){
  case(0):
  displayChar(0x2121,4,0);
  displayChar(0x2121,5,0);
  break;
  case(1):
  displayChar(0x2121,4,0);
  displayChar(0x3D3D,5,0);
  break;
        case(2):
  displayChar(0x4673,4,0);
  displayChar(0x3D3D,5,0);
    break;
        case(3):
  displayChar(0x3B30,4,0);
  displayChar(0x3D3D,5,0);
  break;
  }
  displayChar(0x472F,7,0);

  
   switch(dayOfWeek){
  case(0):
  displayChar(0x467C,0,3);
  displayChar(0x4D4B,1,3);
  break;
  case(1):
  displayChar(0x376E,0,3);
  displayChar(0x4D4B,1,3);
  break;
        case(2):
  displayChar(0x3250,0,3);
  displayChar(0x4D4B,1,3);
    break;
        case(3):
  displayChar(0x3F65,0,3);
  displayChar(0x4D4B,1,3);
    break;
        case(4):
  displayChar(0x4C5A,0,3);
  displayChar(0x4D4B,1,3);
    break;
        case(5):
  displayChar(0x3662,0,3);
  displayChar(0x4D4B,1,3);
    break;
        case(6):
  displayChar(0x467C,0,3);
  displayChar(0x455A,1,3);
    break;
  } 
 // }
  }
  tmpsec=second;
  tmpday=dayOfMonth;

  if(minute==59){
    toneflag=1;
  }
  
  if((toneflag==1)&&(minute==0)){
    tmphour=hour-12;
    switch(tmphour){
      case(0):
        play_rtttl(song0);
        toneflag=0;
        break;
      case(1):
        play_rtttl(song1);
        toneflag=0;
        break;
      case(2):
        play_rtttl(song2);
        toneflag=0;
        break;
      case(3):
        play_rtttl(song3);
        toneflag=0;
        break;
      case(4):
        play_rtttl(song4);
        toneflag=0;
        break;
      case(5):
        play_rtttl(song5);
        toneflag=0;
        break;
      case(6):
        play_rtttl(song6);
        toneflag=0;
        break;
      case(7):
        play_rtttl(song7);
        toneflag=0;
        break;
      case(8):
        play_rtttl(song8);
        toneflag=0;
        break;
      case(9):
        play_rtttl(song9);
        toneflag=0;
        break;
      case(10):
        play_rtttl(song10);
        toneflag=0;
        break;
      case(11):
        play_rtttl(song11);
        toneflag=0;
        break;
        
    }
    
    
    
  }





}



#define isdigit(n) (n >= '0' && n <= '9')
void play_rtttl(char *p)
{
  // Absolutely no error checking in here

  byte default_dur = 4;
  byte default_oct = 6;
  int bpm = 63;
  int num;
  long wholenote;
  long duration;
  byte note;
  byte scale;

  // format: d=N,o=N,b=NNN:
  // find the start (skip name, etc)

  while(*p != ':') p++;    // ignore name
  p++;                     // skip ':'

  // get default duration
  if(*p == 'd')
  {
    p++; p++;              // skip "d="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    if(num > 0) default_dur = num;
    p++;                   // skip comma
  }

  Serial.print("ddur: "); Serial.println(default_dur, 10);

  // get default octave
  if(*p == 'o')
  {
    p++; p++;              // skip "o="
    num = *p++ - '0';
    if(num >= 3 && num <=7) default_oct = num;
    p++;                   // skip comma
  }

  Serial.print("doct: "); Serial.println(default_oct, 10);

  // get BPM
  if(*p == 'b')
  {
    p++; p++;              // skip "b="
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    bpm = num;
    p++;                   // skip colon
  }

  Serial.print("bpm: "); Serial.println(bpm, 10);

  // BPM usually expresses the number of quarter notes per minute
  wholenote = (60 * 1000L / bpm) * 4;  // this is the time for whole note (in milliseconds)

  Serial.print("wn: "); Serial.println(wholenote, 10);


  // now begin note loop
  while(*p)
  {
    // first, get note duration, if available
    num = 0;
    while(isdigit(*p))
    {
      num = (num * 10) + (*p++ - '0');
    }
    
    if(num) duration = wholenote / num;
    else duration = wholenote / default_dur;  // we will need to check if we are a dotted note after

    // now get the note
    note = 0;

    switch(*p)
    {
      case 'c':
        note = 1;
        break;
      case 'd':
        note = 3;
        break;
      case 'e':
        note = 5;
        break;
      case 'f':
        note = 6;
        break;
      case 'g':
        note = 8;
        break;
      case 'a':
        note = 10;
        break;
      case 'b':
        note = 12;
        break;
      case 'p':
      default:
        note = 0;
    }
    p++;

    // now, get optional '#' sharp
    if(*p == '#')
    {
      note++;
      p++;
    }

    // now, get optional '.' dotted note
    if(*p == '.')
    {
      duration += duration/2;
      p++;
    }
  
    // now, get scale
    if(isdigit(*p))
    {
      scale = *p - '0';
      p++;
    }
    else
    {
      scale = default_oct;
    }

    scale += OCTAVE_OFFSET;

    if(*p == ',')
      p++;       // skip comma for next note (or we may be at the end)

    // now play the note

    if(note)
    {
      Serial.print("Playing: ");
      Serial.print(scale, 10); Serial.print(' ');
      Serial.print(note, 10); Serial.print(" (");
      Serial.print(notes[(scale - 4) * 12 + note], 10);
      Serial.print(") ");
      Serial.println(duration, 10);
      tone1.play(notes[(scale - 4) * 12 + note]);
      delay(duration);
      tone1.stop();
    }
    else
    {
      Serial.print("Pausing: ");
      Serial.println(duration, 10);
      delay(duration);
    }
  }
}
