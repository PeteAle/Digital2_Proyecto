/*
 * File:   control_master.c
 * Author: Peter
 *
 * Created on March 8, 2020, 2:21 PM
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
#include <stdio.h>
#include <stdint.h>
#include "I2C.h"
#include "LCD.h"
#include "OSCCON.h"

#define _XTAL_FREQ 4000000

uint16_t ultrasonico = 0;
uint16_t usCentenas = 0, usDecenas = 0, usUnidades = 0;

void setup(void);

void main(void) {
    setup();
    oscInit(1);
    i2c_master_init(100000);    // Iniciar el I2C como master a 100 KHz.
    lcd8_init();                // Iniciar el LCD (DESPUÉS DE INICIAR I2C!!!)
    while(1){
        //--------------------- Lectura del ultrasónico -----------------------
        i2c_masterStart();
        i2c_masterWrite(0x11);
        ultrasonico = i2c_masterRead(0);
        i2c_masterStop();
        __delay_ms(1);
        //--------------------- Conversión del ultrasónico --------------------
        ultrasonico = ultrasonico/29.412;
        ultrasonico = ultrasonico + 1;
//        ultrasonico = ultrasonico*100;
//        usCentenas = ultrasonico/100;
//        ultrasonico = ultrasonico - usCentenas*100;
//        usDecenas = ultrasonico/10;
//        usUnidades = ultrasonico - usDecenas*10;
        //--------------------- Desplegar ultrasónico en LCD ------------------
        lcd8_setCursor(1,0);
        delay_1ms();
        lcd8_dispString("d:");
        delay_1ms();
        lcd8_setCursor(1,3);
        delay_1ms();
        lcd8_dispChar(ultrasonico%10);
        delay_1ms();
        lcd8_setCursor(1,2);
        delay_1ms();
        ultrasonico = ultrasonico/10;
        lcd8_dispChar(ultrasonico%10);
        delay_1ms();
        lcd8_setCursor(1,4);
        delay_1ms();
        lcd8_dispString("cm");
    }
    return;
}

void setup(void){
    TRISA = 0x00;
    TRISE = 0;
    ANSEL = 0;
    ANSELH = 0;
}