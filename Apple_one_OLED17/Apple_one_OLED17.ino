// 12.01.2015 (c) P.Sieg simple Apple 1 emulator based on arduino_6502 
// project from miker00lz (Mike Chambers)
// Link: http://forum.arduino.cc/index.php?topic=193216.0
// apple 1 roms are (c) Steve Wozniak / Apple Inc. Status is Freeware
// according to: http://www.callapple.org/soft/ap1/emul.html
// a1 assembler (c) 9/2006 San Bergmans released as freeware
// Link: http://www.sbprojects.com
// My code released under GNU GPL V2

// HIGH = apple integer basic at 0xE000
// LOW  = a1 assembler at 0xE000

// OLED flame buffer added Mar. 29, 2015 kodera2t
// cpu.c is un-touched. 
// April 5, 2015, cpu.c is modified for Arduino IDE 1.7.0
//////////////// Newly added for graphic OLED(LCD) 128x64 support/////////////
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// SPI PIN definition. Change to fit your board....
#define OLED_MOSI   5
#define OLED_CLK   7
#define OLED_DC    19
#define OLED_CS    18
#define OLED_RESET 20
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);


int screenMem[168]; //the implementation of frame buffer is referenced from Ben Heck's
int cursorX = 0;    //Retro BASIC computer's source
int checkChar = 0;
static const unsigned char initmsg[]          PROGMEM = "tiny apple 1";
//////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// ASCII Characters
#define CR	'\r'
#define NL	'\n'
#define LF      0x0a
#define TAB	'\t'
#define BELL	'\b'
#define SPACE   ' '
#define SQUOTE  '\''
#define DQUOTE  '\"'
#define CTRLC	0x1B  // Changed to ESC key (27 - 0x1B)
#define CTRLH	0x08
#define CTRLS	0x13
#define CTRLX	0x18

//////////////////////////////////////////////////////////////////////

//
#define e0_pin 2 

uint8_t curkey = 0;
uint8_t iskey  = 0;
uint8_t e0_bas = 1;

extern "C" {
  uint16_t getpc();
  uint8_t getop();
  void exec6502(int32_t tickcount);
  void reset6502();
  void serout(uint8_t val) {
    //Serial.print(val, HEX);
    //Serial.println();
//    Serial.write(val);
  outchar(val);
  }
  uint8_t isakey() {
    if (Serial.available()) iskey  = 0x80;
    else iskey = 0;
    return(iskey);
  }
  uint8_t getkey() {
    //curkey = Serial.read() & 0x7F;
        curkey = Serial.read();
    // make a-z => A-Z
    if(curkey !=0x00){
    if ((curkey >= 97) && (curkey <= 122)) curkey = curkey - 0x20;
    return(curkey);
    }
  }
  void clearkey() {
    curkey = 0;
  }
  void printhex(uint16_t val) {
//    Serial.print(val, HEX);
//    Serial.println();
    outchar(val);
  }
  uint8_t get_e0_bas() {
    return(e0_bas);
  }
}

void setup () {
  pinMode(e0_pin,INPUT);
  digitalWrite(e0_pin,HIGH); //pullup on
  delay(100);
  e0_bas = digitalRead(e0_pin);
  Serial.begin (4800);

  //Serial.println("apple 1 emulator");
  //Serial.println ();

  reset6502();
  ////////added OLED part/////
      display.begin();
    //   init done
    display.display();
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
	for (int xg = 0 ; xg < 169 ; xg++) {
		screenMem[xg] = 32;
	}
//  setOutputHandler(&lcdChar);
  printmsg(initmsg);
  doFrame(147);
    printmsg(initmsg);
  
  
}

void loop () {
  exec6502(100); //if timing is enabled, this value is in 6502 clock ticks. otherwise, simply instruction count.
}


static void outchar(unsigned char c)
{
    lcdChar(c);
}

static void lcdChar(byte c) {

	if (c == 8) {	//Backspace?
	
		if (cursorX > 0) {
	
			cursorX -= 1;	
			screenMem[147 + cursorX] = 32;	
			doFrame(147 + cursorX); 
			
		}
	
	}

	if (c != 13 and c != 10 and c != 8) {	
	
		screenMem[147 + cursorX] = c;
		cursorX += 1;
		if (cursorX < 21) {
	        display.setCursor(cursorX*6,8*7);		
		display.write(c);
		display.display();
		}
		
	}
	
	if (cursorX == 21 or c == 10 or c == 13) {			
	
		for (int xg = 21 ; xg > 0 ; xg--) {

			screenMem[0 + xg] = screenMem[21 + xg];
			screenMem[21 + xg] = screenMem[42 + xg];		
			screenMem[42 + xg] = screenMem[63 + xg];
			screenMem[63 + xg] = screenMem[84 + xg];
			screenMem[84 + xg] = screenMem[105 + xg];
			screenMem[105 + xg] = screenMem[126 + xg];		
			screenMem[126 + xg] = screenMem[147 + xg];



			screenMem[147 + xg] = 32;
		
		
		}
	
		cursorX = 0;
		
		doFrame(147);	

	}


}

static void doFrame(byte amount) {
        int xposi,yposi,yshift;
        display.clearDisplay();
	for (int xg = 0 ; xg < amount ; xg++) {
                yshift=int(xg/21.0);
                yposi=yshift*8;
                xposi=(xg-yshift*21)*6;
	        display.setCursor(xposi,yposi);
		display.write(screenMem[xg]);
	}
        display.display();
}  

/***************************************************************************/
void printmsgNoNL(const unsigned char *msg)
{
  while( pgm_read_byte( msg ) != 0 ) {
    outchar( pgm_read_byte( msg++ ) );
  };
}

/***************************************************************************/
void printmsg(const unsigned char *msg)
{
  printmsgNoNL(msg);
  line_terminator();
}

static void line_terminator(void)
{
  outchar(NL);
  outchar(CR);
}
