
#include<reg52.h>
#include<stdio.h>
void UART_Init()
{
	TMOD = 0x20;
	TH1 = 0xFD;
	SCON = 0x50;
	TR1 = 1;
}
void UART_TxChar(char Data)
{
	SBUF = Data;
	while (TI==0);
	TI = 0;
}


char UART_RxChar()
{
	char dat;
	while (RI==0);
	RI = 0;
	dat = SBUF;
	return(dat);
}
	
void UART_SendString(char *str)
{
     int i;
     for(i=0;str[i]!=0;i++)
		{
         UART_TxChar(str[i]);
    }
}

sbit CLK=P0^1;
sbit DIN=P0^2;
sbit DC=P0^3;
sbit CE=P0^4;
sbit RST=P0^5;

void Send(unsigned char);
void Data(unsigned char);
void Cmd(unsigned char);
void setPixel(unsigned char, unsigned char);
void setCursor(unsigned char, unsigned char);
void allClear(void);
void Initialize_LCD(void);
void charDisp(unsigned char);
void stringDisp(unsigned char*);

const unsigned char code LookUpTable [][5] =
{
    { 0x00, 0x00, 0x00, 0x00, 0x00 },  // space
    { 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
    { 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
    { 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
    { 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
    { 0xc4, 0xc8, 0x10, 0x26, 0x46 },   // %
    { 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
    { 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
    { 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
    { 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
    { 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
    { 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
    { 0x00, 0x00, 0x50, 0x30, 0x00 },   // ,
    { 0x10, 0x10, 0x10, 0x10, 0x10 },   // -
    { 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
    { 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
    { 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
    { 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
    { 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
    { 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
    { 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
    { 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
    { 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
    { 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
    { 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
    { 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
    { 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
    { 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
    { 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
    { 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
    { 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
    { 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
    { 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
    { 0x7E, 0x11, 0x11, 0x11, 0x7E },   // A
    { 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
    { 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
    { 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
    { 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
    { 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
    { 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
    { 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
    { 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
    { 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
    { 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
    { 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
    { 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
    { 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
    { 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
    { 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
    { 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
    { 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
    { 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
    { 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
    { 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
    { 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
    { 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
    { 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
    { 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
    { 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
    { 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
    { 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
    { 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
    { 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
    { 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
    { 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
    { 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
    { 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
    { 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
    { 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
    { 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
    { 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
    { 0x0C, 0x52, 0x52, 0x52, 0x3E },   // g
    { 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
    { 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
    { 0x20, 0x40, 0x44, 0x3D, 0x00 },   // j
    { 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
    { 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
    { 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
    { 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
    { 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
    { 0x7C, 0x14, 0x14, 0x14, 0x08 },   // p
    { 0x08, 0x14, 0x14, 0x18, 0x7C },   // q
    { 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
    { 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
    { 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
    { 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
    { 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
    { 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
    { 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
    { 0x0C, 0x50, 0x50, 0x50, 0x3C },   // y
    { 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
    { 0x00, 0x08, 0x3e, 0x41, 0x00 }    // {
};


void Send(unsigned char a){
	unsigned char i;
	for(i=0;i<8;i++){
		CLK = 0;
		if((a&0x80)){
			DIN = 1;
		} else {
			DIN = 0;
		}
		CLK = 1;
		a <<= 1;
	}
}


void Data(unsigned char x){
	DC = 1;
	CE = 0;
	Send(x);
	CE = 1;
}

void Cmd(unsigned char x){
	DC = 0;
	CE = 0;
	Send(x);
	CE = 1;
}
void setPixel(unsigned char x, unsigned char y)
{
	Cmd(0x40|(y&0x07));	// Y axis
	Cmd(0x80|(x&0x7f));	// X axis
}

void allClear(void)
{
	int pixel;
	setPixel(0,0);										// Cursor Home.
	for (pixel=504;pixel>0;pixel--)	{Data(0x00);}		// 6*84 = 504 DDRAM addresses.
}


void Initialize_LCD(void)
{
	RST = 1;	// Set _RES HIGH.
	CE = 1;		// Disable Chip.


	Cmd(0x21);		// Activate Chip and H=1.
	Cmd(0xb2);		// Set LCD Voltage to about 7V.
	Cmd(0x13);		// Adjust voltage bias.
	Cmd(0x20);		// Horizontal addressing and H=0.
	Cmd(0x09);		// Activate all segments.
	allClear();			// Erase all pixel on the DDRAM.
	Cmd(0x08);		// Blank the Display.
	Cmd(0x0C);		// Display Normal.
	setPixel(0,0);		// Cursor Home.
}

void setCursor(unsigned char row, unsigned char col){
	if((row>6) || (row<1) || (col<1) || (col>14))
		return;
	setPixel(((col-1)*6),(row-1));
}

/* Writes single character on LCD                   */
/* Input Arguments: ch -> Character to be displayed */
void charDisp(unsigned char a){
	unsigned char i, b;
	if ( (a < 0x20) || (a > 0x7c) ){
		a = 92;
	}
	for(i=0;i<5;i++){
		b = LookUpTable[a - 32][i] << 1;
		Data(b);
	}
	Data(0x00);
}

void stringDisp(unsigned char *p){
	while(*p)
		charDisp(*p++);
}

void Delay(int k)
{
	int i,j;
	for(i=0;i<k;i++)
	for(j=0;j<1000;j++);
}
		
void main(){
	int l;
	char RFID[15];
	UART_Init();	/* Initialize UART communication  */
	Initialize_LCD();
	//Bring Cursor to Line 1, character 1
	setCursor(1,1);
	setPixel(0,0);
	//Write gfx data on LCD
		while(1)
	{
		for(l=0;l<12;l++)
		{ 
			RFID[l]=UART_RxChar();
		}
		
		stringDisp(RFID);  
	}	
}
