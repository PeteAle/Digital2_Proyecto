/*
 * File:   Sensores.c
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
#include "ADC.h"
#include "I2C.h"

#define _XTAL_FREQ 4000000

int luz = 0;
int fuerza = 0;

uint8_t x = 0, z = 0;

void setup(void);

void __interrupt() ISR(){
    di();
    //-------------------- ADC del sensor de fuerza ---------------------------
    if (PIR1bits.ADIF == 1 && ADCON0bits.CHS3 == 1 && ADCON0bits.CHS2 == 0 && ADCON0bits.CHS1 == 0 && ADCON0bits.CHS0 == 1){
        fuerza = ADRESH;
        ADCON0bits.CHS3 = 1;
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS0 = 0;
        PIR1bits.ADIF = 0;
    }
    //-------------------- ADC de la fotoresistencia --------------------------
    else if (PIR1bits.ADIF == 1 && ADCON0bits.CHS3 == 1 && ADCON0bits.CHS2 == 0 && ADCON0bits.CHS1 == 0 && ADCON0bits.CHS0 == 0){
        luz = ADRESH;
        ADCON0bits.CHS3 = 1;
        ADCON0bits.CHS2 = 0;
        ADCON0bits.CHS1 = 0;
        ADCON0bits.CHS0 = 1;
        PIR1bits.ADIF = 0;
    }
    //-------------------- Enviar por I2C -------------------------------------
    if (PIR1bits.SSPIF == 1){
        SSPCONbits.CKP = 0;
        if (SSPCONbits.WCOL == 1 || SSPCONbits.SSPOV == 1){
            x = SSPBUF;
            SSPCONbits.WCOL = 0;
            SSPCONbits.SSPOV = 0;
            SSPCONbits.CKP = 1;
        }
        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW){     // Escribir al slave
            x = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            z = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            PORTA = z;
            __delay_us(250);
        }
        else if (!SSPSTATbits.D_nA && SSPSTATbits.R_nW){    // Leer al slave
            x = SSPBUF;
            SSPSTATbits.BF = 0;
            SSPBUF = fuerza;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
        PIR1bits.SSPIF = 0;
    }
    ei();
}

void main(void) {
    setup();
    oscInit(1);
    adcSetup();
    adcFoscSel(1);
    analogInSel(9);
    adcInterrupt(1);
    di();
    i2c_slave_init(0x10);
    ei();
    while(1){
        //------------------- Iniciar ADC -------------------------------------
        if (ADCON0bits.GO_DONE == 0){
            ADCON0bits.GO_DONE = 1;
        }        
    }
    return;
}

void setup(void){
    TRISA = 0x00;
    PORTA = 0x00;
    TRISBbits.TRISB3 = 1;   // Entrada analógica para el sensor de fuerza.
    TRISBbits.TRISB2 = 1;   // Entrada analógica para la fotoresistencia.
    ANSEL = 0;
    ANSELHbits.ANS9 = 1;
    ANSELHbits.ANS11 = 1;
}
