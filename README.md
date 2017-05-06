# TinyBasicPlus_OLED_support
	! PLEASE select LCD version if you are searching character LCD support for TinyBasicPlus!!, If you need graphic LCD support for TinyBasicPluse, please select OLED version. !
	SSD1306 compatible OLED/LCD (128x64 dots) support is added to TinyBasicPlus, using Adafruit_GFX lib. 
	Frame buffer for 21x8 characters is added for text display. Along this support, TVout and PS2 keyboard support is subtracted, assuming re-programmed Xbox360 or serial console usage for input device. (you can add PS/2, if you need) I refered the source code by Ben Heck's Retro Basic Computer. GRAPHIC related BASIC commands (currently supporting only direct command):  CLS (clear screen), LINE x0,y0,x1,y1 CIRCLE x,y,radius FILLCIRCLE x,y,radius ROUNDRECT x,y,w,h,r FILLROUNDRECT x,y,w,h,r are added just for graphic test..
	Not only Arduino sketch, also circuit schematic and circuit pattern (.brd) is uploaded for your quick test of OLED version.
