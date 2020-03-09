/*
 * File:   .c
 * Author: Peter
 *
 * Created on February 9, 2020, 5:31 PM
 */

#define rs PORTEbits.RE0
#define rw PORTEbits.RE1
#define en PORTEbits.RE2
#define data PORTA

#define _XTAL_FREQ 4000000

#include <xc.h>
#include "LCD.h"

void lcd8_init(){
    __delay_ms(20);
    lcd8_cmd(0x30);
    __delay_ms(5);
    lcd8_cmd(0x30);
    __delay_ms(1);
    lcd8_cmd(0x30);
    __delay_ms(1);
    lcd8_cmd(0x38);
    lcd8_cmd(0x10);
    lcd8_cmd(0x01);
    lcd8_cmd(0x06);
    lcd8_cmd(0x0C);
    lcd8_cmd(0x80);
}

void lcd8_cmd(unsigned char cmd){
    data = cmd;
    rs = 0;
    rw = 0;
    en = 1;
    delay_1ms();
    en = 0;
}

void lcd8_write(unsigned int dat){
    data = dat;
    rs = 1;
    rw = 0;
    en = 1;
    delay_1ms();
    en = 0;
}

void lcd8_dispString(char *value){
    while(*value){
        lcd8_write(*value++);
    }
}

void lcd8_dispChar(char val_num){
    lcd8_write(val_num + 0x30);
}

void lcd8_setCursor(unsigned char fila, unsigned char columna){
    if (fila == 1){
        lcd8_cmd(0x80 + columna);
    }
    else if (fila == 2){
        lcd8_cmd(0xC0 + columna);
    }
}

void delay_1ms(void){
    for (int i = 0; i < 50; i++);
    // _XTAL_FREQ 8000000
    // _XTAL_FREQ/80000
    // Con esto, resulta en cambiar i < 50 a i < 100 para 1 ms.
}