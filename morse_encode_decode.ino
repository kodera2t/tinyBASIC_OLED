#include <avr/pgmspace.h>
#include <MorseEnDecoder.h>


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
int wpm = 13;

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

static const unsigned char initmsg[]          PROGMEM = "Morse EnDecoder";
static const unsigned char wpmsg[]              PROGMEM = "Current wpm is ";
static const unsigned char errormsg[]         PROGMEM = "< ERROR:too many morse signals! >";

// Pin mapping
const byte morseInPin = 10;      
const byte morseOutPin = 11;

// Instantiate Morse objects
morseDecoder morseInput(morseInPin, MORSE_KEYER, MORSE_ACTIVE_LOW);
morseEncoder morseOutput(morseOutPin);

void setup()
{
  Serial.begin(4800);
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
 // setOutputHandler(&lcdChar);
  printmsg(initmsg);
  doFrame(147);
    printmsg(initmsg);
 //   printmsg(wpmsg);
 //   printmsg(char(wpm));
  
  morseInput.setspeed(wpm);
  morseOutput.setspeed(wpm);
}


void loop()
{
  // Need to call these once per loop


  morseOutput.encode();
  // SEND MORSE (OUTPUT)
  // Encode and send text received from the serial port (serial monitor)
  if (morseOutput.available())
  {
    // Get character from serial and send as Morse code
    char sendMorse = Serial.read();
    morseOutput.write(sendMorse);
    //Serial.print(sendMorse);
    lcdChar(sendMorse);
  }

 morseInput.decode();
  // RECEIVE MORSE (INPUT)
  // If a character is decoded from the input, write it to serial port
  if (morseInput.available())
  {
    char receivedMorse = morseInput.read();
    lcdChar(receivedMorse);
   if(receivedMorse != 0xFF){
       Serial.print(receivedMorse);
    // A little error checking    
    if (receivedMorse == '#') printmsg(errormsg);
  }
  }
}


static void outchar(unsigned char c)
{
    lcdChar(c);
}

static void lcdChar(unsigned int c) {
if(c <= 128){
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
	
	if (cursorX == 21 or c == 10) {			
	
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
