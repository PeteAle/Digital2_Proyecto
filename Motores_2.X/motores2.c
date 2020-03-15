/*
 * File:   motores2.c
 * Author: Peter
 *
 * Created on March 14, 2020, 12:26 AM
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

#define _XTAL_FREQ 4000000

uint8_t motores = 0, bit0 = 0;
uint8_t z;
uint8_t dato;
//*****************************************************************************
// Definición de funciones para que se puedan colocar después del main de lo 
// contrario hay que colocarlos todas las funciones antes del main
//*****************************************************************************
void setup(void);
//*****************************************************************************
// Código de Interrupción 
//*****************************************************************************
void __interrupt() isr(void){
   if(PIR1bits.SSPIF == 1){ 

        SSPCONbits.CKP = 0;
       
        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){
            z = SSPBUF;                 // Read the previous value to clear the buffer
            SSPCONbits.SSPOV = 0;       // Clear the overflow flag
            SSPCONbits.WCOL = 0;        // Clear the collision bit
            SSPCONbits.CKP = 1;         // Enables SCL (Clock)
        }

        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW) {
            //__delay_us(7);
            z = SSPBUF;                 // Lectura del SSBUF para limpiar el buffer y la bandera BF
            //__delay_us(2);
            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
            motores = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
            PORTB = motores;
            __delay_us(250);
            
        }else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){
            z = SSPBUF;
            BF = 0;
            SSPBUF = PORTB;
            SSPCONbits.CKP = 1;
            __delay_us(250);
            while(SSPSTATbits.BF);
        }
       
        PIR1bits.SSPIF = 0;    
    }
}
//*****************************************************************************
// Main
//*****************************************************************************
void main(void) {
    setup();
    oscInit(1);
    //*************************************************************************
    // Loop infinito
    //*************************************************************************
    while(1){
        PORTB = motores;
//        bit0 = (motores & 0b00000001);
//        if (bit0 == 1){
//            PORTBbits.RB7=1;
//            PORTBbits.RB6=0;
//        }
//        else{
//            PORTBbits.RB7=0;
//            PORTBbits.RB6=1;
//        }
//        PORTB = PORTB;
       __delay_ms(500);
    }
    return;
}
//*****************************************************************************
// Función de Inicialización
//*****************************************************************************
void setup(void){
    ANSEL = 0;
    ANSELH = 0;
    
    TRISB = 0;
    TRISD = 0;
    
    PORTB = 0;
    PORTD = 0;
    i2c_slave_init(0x50);   
}


//char z = 0, x = 0;
//uint8_t motores = 0;
//
//void setup(void);
//
//void __interrupt() isr(){
//    di();
//    if(SSPIF == 1){
//        SSPCONbits.CKP = 0;
//        if ((SSPCONbits.SSPOV) || (SSPCONbits.WCOL)){ //If overflow or collision
//            z = SSPBUF;             // Read the previous value to clear the buffer
//            SSPCONbits.SSPOV = 0;   // Clear the overflow flag
//            SSPCONbits.WCOL = 0;    // Clear the collision bit
//            SSPCONbits.CKP = 1;
//            }
//        if(!SSPSTATbits.D_nA && !SSPSTATbits.R_nW){ //If last byte was Address + Write
//            z = SSPBUF;
//            PIR1bits.SSPIF = 0;         // Limpia bandera de interrupción recepción/transmisión SSP
//            SSPCONbits.CKP = 1;         // Habilita entrada de pulsos de reloj SCL
//            while(!SSPSTATbits.BF);     // Esperar a que la recepción se complete
//            PORTB = SSPBUF;             // Guardar en el PORTD el valor del buffer de recepción
//            __delay_us(250);
//        }
//        else if(!SSPSTATbits.D_nA && SSPSTATbits.R_nW){ //If last byte was Address + Read
//            z = SSPBUF;
//            SSPSTATbits.BF = 0;
//            SSPBUF = PORTB ;
//            SSPCONbits.CKP = 1;
//            __delay_us(250);
//            while(SSPSTATbits.BF);
//        }
//        PIR1bits.SSPIF = 0;
//    }
//    ei();
//}
//
//void main(void) {
//    setup();
//    oscInit(1);
//    i2c_slave_init(0x20);
//    while(1){
//        
//    }
//    return;
//}
//
//void setup(void){
//    TRISA = 0;
//    PORTA = 0;
//    TRISB = 0;
//    PORTB = 0;
//    ANSEL = 0;
//    ANSELH = 0;
//    TRISAbits.TRISA0 = 1;
//    TRISAbits.TRISA1 = 1;
//    TRISAbits.TRISA2 = 1;
//    TRISAbits.TRISA3 = 1;
//    TRISBbits.TRISB0 = 1;
//    TRISBbits.TRISB1 = 1;
//    TRISBbits.TRISB2 = 1;
//    TRISBbits.TRISB3 = 1;
//}