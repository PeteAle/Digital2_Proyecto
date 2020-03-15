/*
 * File:   master2.c
 * Author: Peter
 *
 * Created on March 14, 2020, 12:40 AM
 */

// PIC16F887 Configuration Bit Settings

// 'C' source line config statements

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
#include <pic16f887.h>
#include <stdint.h>
#include <stdio.h>
#include "I2C_2.h"
#include "OSCCON.h"
#include "TMR1.h"
#include "ADC.h"

#define _XTAL_FREQ 4000000

unsigned char motores = 0, motores2 = 0;

//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);

//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    oscInit(1);
    adcSetup();
    analogInSel(12);
    adcFoscSel(1);
    while(1){
//        struct{
//            unsigned bit0   :1; 
//            unsigned bit1   :1; 
//            unsigned bit2   :1; 
//            unsigned bit3   :1; 
//            unsigned bit4   :1; 
//            unsigned bit5   :1; 
//            unsigned bit6   :1; 
//            unsigned bit7   :1; 
//        }motores;
        
//        if (PORTDbits.RD0 == 1){
//            PORTAbits.RA7=1;
//            PORTAbits.RA6=0;
//        }
//        if (PORTDbits.RD1 == 1){
//            PORTAbits.RA7=0;
//            PORTAbits.RA6=1;
//        }
//        if (PORTDbits.RD2 == 1){
//            PORTAbits.RA5=1;
//            PORTAbits.RA4=0;
//        }
//        if (PORTDbits.RD3 == 1){
//            PORTAbits.RA5=0;
//            PORTAbits.RA4=1;
//        }
//        if (PORTDbits.RD4 == 1){
//            PORTAbits.RA3=1;
//            PORTAbits.RA2=0;
//        }
//        if (PORTDbits.RD5 == 1){
//            PORTAbits.RA3=0;
//            PORTAbits.RA2=1;
//        }
//        if (PORTDbits.RD6 == 1){
//            PORTAbits.RA1=1;
//            PORTAbits.RA0=0;
//        }
//        if (PORTDbits.RD7 == 1){
//            PORTAbits.RA1=0;
//            PORTAbits.RA0=1;
//        }
        motores = 0x1F;
        motores2 = 0xFF;
        i2c_masterStart();
        i2c_masterWrite(0x50);
        i2c_masterWrite(motores);
        i2c_masterStop();
        __delay_ms(200);
        i2c_masterStart();
        i2c_masterWrite(0x50);
        i2c_masterWrite(motores2);
        i2c_masterStop();
        __delay_ms(200);
       
//        i2c_masterStart();
//        i2c_masterWrite(0x51);
//        PORTD = i2c_masterRead(0);
//        i2c_masterStop();
//        __delay_ms(200);
//        PORTB++;   
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELHbits.ANS12 = 1;
    TRISBbits.TRISB0 = 1;
    TRISA = 0;
    TRISD = 0xFF;
    PORTA = 0x00;
    PORTD = 0;
    i2c_master_init(100000);        // Inicializar Comuncación I2C
}

//unsigned char i = 0;
//
//void setup(void);
//
//void main(void) {
//    setup();
//    oscInit(1);
//    i2c_master_init(100000);
//    while(1){
//        i2c_masterStart();
//        i2c_masterWrite(0x20);
//        i2c_masterWrite(i++);
//        i2c_masterStop();
//        __delay_ms(100);
//    }
//    return;
//}
//
//void setup(void){
//    TRISA = 0;
//    TRISBbits.TRISB0 = 0;
//    TRISBbits.TRISB1 = 1;
//    TRISBbits.TRISB2 = 0;
//    TRISD = 0xFF;
//}