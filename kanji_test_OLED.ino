#include <SPI.h> 
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <string.h> 
#define ASC8x16S 255968
#define ASC8x16N 257504
#define kanji_CS 4 
byte rawdata[32];
int i=0;

// SPI PIN definition. Change to fit your board....
#define OLED_MOSI   5
#define OLED_CLK   7
#define OLED_DC    19
#define OLED_CS    18
#define OLED_RESET 20
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
 pinMode(kanji_CS,OUTPUT);
 SPI.begin();
 Serial.begin(4800);
   display.begin();
  display.display();
  display.clearDisplay();
  display.fillScreen(0);
    display.clearDisplay();
}

void loop() {
int count;
int count1,count2,count3,count4;
    count=i%4;

   displayChar(0x467c,0,count);
   displayChar(0x4B5c,1,count);
   displayChar(0x386C,2,count);
   displayChar(0x242A,3,count);
   displayChar(0x2141,4,count);
   displayChar(0x2431,5,count);
   displayChar(0x2141,6,count);
   displayChar(0x212A,7,count);
   
    count1=count+1;
    if(count1>3){
      count1=count1-4;
    }
   displayChar(0x3441,0,count1);
   displayChar(0x3B7A,1,count1);
   displayChar(0x256D,2,count1);
   displayChar(0x2560,3,count1);
   displayChar(0x456B,4,count1);
   displayChar(0x3A5C,5,count1);
   displayChar(0x2443,6,count1);
   displayChar(0x2439,7,count1);

    count2=count+2;
        if(count2>3){
      count2=count2-4;
    }
   displayChar(0x2338,0,count2);
   displayChar(0x215F,1,count2);
   displayChar(0x2334,2,count2);
   displayChar(0x4A38,3,count2);
   displayChar(0x3B7A,4,count2);
   displayChar(0x493D,5,count2);
   displayChar(0x3C28,6,count2);
   displayChar(0x212A,7,count2);


    count3=count+3;
        if(count3>3){
      count3=count3-4;
    }
   displayChar(0x4954,0,count3);
   displayChar(0x597A,1,count3);
   displayChar(0x4954,2,count3);
   displayChar(0x367E,3,count3);
   displayChar(0x3140,4,count3);
   displayChar(0x3330,5,count3);
   displayChar(0x4173,6,count3);
   displayChar(0x4537,7,count3);

  i++;
  delay(2000);  
}
