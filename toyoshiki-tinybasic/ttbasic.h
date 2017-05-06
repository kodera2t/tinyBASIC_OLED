/*
 TOYOSHIKI Tiny BASIC for Arduino
 (C)2012 Tetsuya Suzuki
 GNU General Public License
 */

#include <Arduino.h>

//Prototypes
void newline(void);
char* c_strchr(char *, char);
void c_puts(const char *);
void c_gets(void);
void gpush(unsigned char*);
unsigned char* gpop(void);
void lpush(unsigned char*);
unsigned char* lpop(void);
void error(void);
void putnum(short, short);
short getnum(void);
short getvalue(unsigned char*);
short getparam(void);
unsigned char* getlp(short);
unsigned char toktoi(void);
void putlist(unsigned char*);
void inslist(void);
short getsize(void);
short getabs(void);
short getrnd(void);
short getarray(void);
short ivalue(void);
short icalc(void);
short iexp(void);
void iprint(void);
void iinput(void);
char iif(void);
void ivar(void);
void iarray(void);
void ilet(void);
void ilist(void);
void inew(void);
unsigned char* iexe(void);
void irun(void);
void icom(void);
void basic(void);

