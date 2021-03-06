/*
 * File:   ultrasonico.c
 * Author: Peter
 *
 * Created on March 4, 2020, 2:32 PM
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
#include <stdint.h>
#include <stdio.h>
#include "OSCCON.h"
#include "TMR1.h"
#include "LCD.h"

#define Trigger PORTBbits.RB0
#define Echo PORTBbits.RB1
#define _XTAL_FREQ 4000000

int distancia = 0;

void setup(void);

void __interrupt() ISR(){
    
}

void main(void) {
    setup();
    oscInit(1);
    lcd8_init();
    tmr1_Init();
    tmr1_Prescaler(1);
    tmr1_Interrupt(0);
    while(1){
        //------------------- Obtener valor de US -----------------------------
        TMR1H = 0;                //Sets the Initial Value of Timer
        TMR1L = 0;                //Sets the Initial Value of Timer
        Trigger = 1;                  //TRIGGER HIGH
        __delay_us(10);           //10uS Delay 
        Trigger = 0;                  //TRIGGER LOW
        while(!Echo);              //Waiting for Echo
        TMR1ON = 1;               //Timer Starts
        while(Echo);               //Waiting for Echo goes LOW
        TMR1ON = 0;               //Timer Stops
        //------------------- C�lculo de distancia ----------------------------
        distancia = (TMR1L | (TMR1H<<8));
        distancia = distancia/29.412;
        distancia = distancia + 1;
        //-------------------- Desplegar en LCD -------------------------------
        lcd8_setCursor(1,1);
        delay_1ms();
        lcd8_dispString("d:");
        delay_1ms();
        lcd8_setCursor(2,3);
        delay_1ms();
        lcd8_dispChar(distancia%10);
        delay_1ms();
        lcd8_setCursor(2,2);
        delay_1ms();
        distancia = distancia/10;
        lcd8_dispChar(distancia%10);
        delay_1ms();
        lcd8_setCursor(2,1);
        delay_1ms();
        distancia = distancia/10;
        lcd8_dispChar(distancia%10);
        lcd8_setCursor(2,4);
        delay_1ms();
        lcd8_dispString("cm");
    }
    return;
}

void setup(void){
    Trigger = 0;
    Echo = 0;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 1;
    TRISA = 0x00;
    PORTA = 0x00;
    ANSEL = 0;
    ANSELH = 0;
    TRISE = 0;
}

//void interruptEnable(void){
//    INTCONbits.GIE = 1;
//    INTCONbits.PEIE = 1;
//}
