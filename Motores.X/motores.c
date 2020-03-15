/*
 * File:   motores.c
 * Author: Peter
 *
 * Created on March 13, 2020, 6:57 PM
 */

// CONFIG1
#pragma config FOSC = INTRC_NOCLKOUT// Oscillator Selection bits (INTOSCIO oscillator: I/O function on RA6/OSC2/CLKOUT pin, I/O function on RA7/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled and can be enabled by SWDTEN bit of the WDTCON register)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config MCLRE = OFF      // RE3/MCLR pin function select bit (RE3/MCLR pin function is digital input, MCLR internally tied to VDD)
#pragma config CP = OFF         // Code Protection bit (Program memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)
#pragma config BOREN = OFF      // Brown Out Reset Selection bits (BOR disabled)
#pragma config IESO = OFF       // Internal External Switchover bit (Internal/External Switchover mode is disabled)
#pragma config FCMEN = OFF      // Fail-Safe Clock Monitor Enabled bit (Fail-Safe Clock Monitor is disabled)
#pragma config LVP = OFF        // Low Voltage Programming Enable bit (RB3 pin has digital I/O, HV on MCLR must be used for programming)

// CONFIG2
#pragma config BOR4V = BOR40V   // Brown-out Reset Selection bit (Brown-out Reset set to 4.0V)
#pragma config WRT = OFF        // Flash Program Memory Self Write Enable bits (Write protection off)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "OSCCON.h"
#include "I2C.h"

#define _XTAL_FREQ 4000000

unsigned char motores = 0;
uint8_t bit0 = 0, bit1 = 0, bit2 = 0, bit3 = 0, bit4 = 0, bit5 = 0, bit6 = 0, bit7 = 0;
uint8_t x = 0, z = 0;

void setup(void);

void __interrupt() isr(){
    di();
    //-------------------- Enviar por I2C -------------------------------------
    if (PIR1bits.SSPIF == 1){
        SSPCONbits.CKP = 0;
        if (SSPCONbits.WCOL == 1 || SSPCONbits.SSPOV == 1){
            x = SSPBUF;
            SSPCONbits.WCOL = 0;
            SSPCONbits.SSPOV = 0;
            SSPCONbits.CKP = 1;
        }
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW){ // Si se quiere escribir en el Slave
            x = SSPBUF;
            while(!SSPSTATbits.BF);
            motores = SSPBUF;
            PORTB = motores;
            SSPCONbits.CKP = 1;
        }
        else if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW){    // Si se quiere leer del Slave
            x = SSPBUF;
            SSPSTATbits.BF = 0;
            SSPBUF = x;
            SSPCONbits.CKP = 1;
        }
        __delay_us(300);
        while(SSPSTATbits.BF);
    }
    PIR1bits.SSPIF = 0;
    ei();
}

void main(void) {
    setup();
    //oscInit(1);
    i2c_slave_init(0x50);
    while(1){
//        bit1 = (motores & 0b00000001);
//        if (bit1 == 1){
//            PORTBbits.RB7=1;
//            PORTBbits.RB6=0;
//        }
//        else{
//            PORTBbits.RB7=1;
//            PORTBbits.RB6=0;
//        }
//        struct{
//            unsigned bit0    :1;
//            unsigned bit1    :1;
//            unsigned bit2    :1;
//            unsigned bit3    :1;
//            unsigned bit4    :1;
//            unsigned bit5    :1;
//            unsigned bit6    :1;
//            unsigned bit7    :1;
//        }motores;
//        
//        if (motores.bit0 == 1){
//            PORTBbits.RB7=1;
//            PORTBbits.RB6=0;
//        }
//        else{
//            PORTB = 0;
//        }
//        if (motores.bit1 == 1){
//            PORTBbits.RB7=0;
//            PORTBbits.RB6=1;
//        }
//        else{
//            PORTB = 0;
//        }
//        if (motores.bit2 == 1){
//            PORTBbits.RB5=1;
//            PORTBbits.RB4=0;
//        }
//        else{
//            PORTB = 0;
//        }
//        if (motores.bit3 == 1){
//            PORTBbits.RB5=0;
//            PORTBbits.RB4=1;
//        }
//        else{
//            PORTB = 0;
//        }
//        if (motores.bit4 == 1){
//            PORTAbits.RA0=1;
//            PORTAbits.RA1=0;
//        }
//        else{
//            PORTA = 0;
//        }
//        if (motores.bit5 == 1){
//            PORTAbits.RA0=0;
//            PORTAbits.RA1=1;
//        }
//        else{
//            PORTA = 0;
//        }
//        if (motores.bit6 == 1){
//            PORTAbits.RA2=1;
//            PORTAbits.RA3=0;
//        }
//        else{
//            PORTA = 0;
//        }
//        if (motores.bit7 == 1){
//            PORTAbits.RA2=0;
//            PORTAbits.RA3=1;
//        }
//        else{
//            PORTA = 0;
//        }
    }
    return;
}
    
void setup(void){
    TRISB = 0x00;
    PORTB = 0x00;
    TRISA = 0x00;
    PORTA = 0x00;
    ANSEL = 0x00;
    ANSELH = 0x00;
}