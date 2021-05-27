#define FCY 32000000UL
#define PWM_PERIOD 62500

#include <stdlib.h>
#include <stdio.h>
#include <libpic30.h>
#include <xc.h>
#include <math.h>

#define lcdport LATC        //defines latch C as the LCD port


#define rs LATCbits.LATC0  
#define rw LATCbits.LATC1
#define en LATCbits.LATC2



void lcd_ini();
void dis_cmd(unsigned char);
void dis_data(unsigned char);
void lcdcmd(unsigned char);
void lcddata(unsigned char);
void write_to_LCD(unsigned char[]);
void clear_LCD();
void next_line();
void shift_cursor_right(unsigned int);
void shift_cursor_left(unsigned int);
void cursor_on(void);



void write_to_LCD(unsigned char data0[]){       //writes a string to be displayed on the LCD at the cursor position
    unsigned int i = 0;                         //defaults to writing to the first position of the first LCD line
    //lcd_ini();
        
        while(data0[i]!= 0){
            dis_data(data0[i]);
            __delay_ms(1);
            i++;
        }
}


void shift_cursor_right(unsigned int spaces){   //shifts the cursor one position right
    unsigned int i = 0;
    for(i = 0; i < spaces; i++){
        dis_cmd(0x14);
        __delay_ms(1);
    }
}

void shift_cursor_left(unsigned int spaces){    //shifts the cursor one position left
    unsigned int i = 0;
    for(i = 0; i < spaces; i++){
        dis_cmd(0x10);
        __delay_ms(1);
    }
}

void cursor_on(void){       //turns on a blinking cursor in the first position of the first LCD line
    dis_cmd(0x0F);
    dis_cmd(0x80);
}

void second_line(){         //sends the cursor to the first position of the second LCD line
    dis_cmd(0xc0);
    __delay_ms(2);
}

void first_line(){          //sends the cursor to the first position of the first LCD line
    dis_cmd(0x80);
    __delay_ms(2);
}

void clear_LCD(){
    dis_cmd(0x01);            //Clear LCD Command         
    dis_cmd(0x80);
}

void dis_cmd(unsigned char cmd_value)
{   unsigned char cmd_value1;
	cmd_value1 = (cmd_value & 0xF0);	// Mask lower nibble because RB4-RB7 pins are being used
	lcdcmd(cmd_value1);			// Send to LCD
	cmd_value1 = ((cmd_value<<4) & 0xF0);	// Shift 4-bit and mask
	lcdcmd(cmd_value1);			// Send to LCD
}

void dis_data(unsigned char data_value)
{
	unsigned char data_value1;
	data_value1=(data_value&0xF0);
	lcddata(data_value1);
	data_value1=((data_value<<4)&0xF0);
	lcddata(data_value1);
}

void lcdcmd(unsigned char cmdout)
{
	lcdport=cmdout;		//Send command to lcdport
	rs=0;						
	rw=0;
	en=1;
	__delay_ms(1);
    en=0;
}

void lcddata(unsigned char dataout)
{
	lcdport=dataout;	//Send data to lcdport 
	rs=1;
	rw=0;
	en=1;
	__delay_ms(1);
	en=0;
}

void lcd_ini()	                    
{
	dis_cmd(0x02);		// To initialize LCD in 4-bit mode.
	dis_cmd(0x28);		// To initialize LCD in 2 lines, 5x7 dots and 4bit mode.
	dis_cmd(0x0C);
	dis_cmd(0x06);
	dis_cmd(0x80);
}