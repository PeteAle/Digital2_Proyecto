/*
 * File:   master.c
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
#include "TMR1.h"

#define Trigger PORTBbits.RB0
#define Echo PORTBbits.RB1

#define _XTAL_FREQ 4000000

uint16_t distancia = 0, fuerza = 0;

void setup(void);

void main(void){
    setup();
    oscInit(1);
    tmr1_Init();
    tmr1_Prescaler(1);
    tmr1_Interrupt(0);
    i2c_master_init(100000);
    lcd8_init();                // Iniciar el LCD (DESPUÉS DE INICIAR I2C!!!)
    while(1){
        //----------------------- Recibir datos en I2C -----------------------
        i2c_masterStart();
        i2c_masterWrite(0x11);
        fuerza = i2c_masterRead(0);
        i2c_masterStop();
        __delay_ms(1);
        //----------------------- Ultrasónico ---------------------------------
        TMR1H = 0;                // Establece TMR1H como 0.
        TMR1L = 0;                // Establece TMR1L como 0.
        Trigger = 1;              // Habilitar el trigger para enciar señal
        __delay_us(10);           // 10uS Delay 
        Trigger = 0;              // TRIGGER LOW para parar señal
        while(!Echo);             // Esperando recibir el ECHO (señal rebotada)
        T1CONbits.TMR1ON = 1;               // Inicia timer
        while(Echo);              // Esperar que ya no se reciba un ECHO.
        T1CONbits.TMR1ON = 0;               // Para el timer
        //------------------- Cálculo de distancia ----------------------------
        distancia = (TMR1L | (TMR1H<<8));
        distancia = distancia/29;
        distancia = distancia + 1;
        //------------------- Alarma de distancia -----------------------------
        if (distancia <= 10 && distancia >= 9){
            PORTBbits.RB2 = 1;
            __delay_ms(300);
            PORTBbits.RB2 = 0;
            __delay_ms(600);
        }
        if (distancia <= 8 && distancia >= 6){
            PORTBbits.RB2 = 1;
            __delay_ms(300);
            PORTBbits.RB2 = 0;
            __delay_ms(300);
        }
        if (distancia < 6){
            PORTBbits.RB2 = 1;            
        }
        //------------------------ Desplegar en LCD ---------------------------
        char D[5];
        sprintf(D ,"%3u", distancia);
        lcd8_setCursor(1,0);
        delay_1ms();
        lcd8_dispString("d:");
        delay_1ms();
        lcd8_setCursor(1,2);
        delay_1ms();
        lcd8_dispString(D);
        delay_1ms();
        lcd8_setCursor(1,5);
        delay_1ms();
        lcd8_dispString("cm");
        //------------------------ Fuerza en I2C ------------------------------
        char F[5];
        sprintf(F, "%3u", fuerza);
        lcd8_setCursor(1,8);
        delay_1ms();
        lcd8_dispString("f:");
        delay_1ms();
        lcd8_setCursor(1,10);
        delay_1ms();
        lcd8_dispString(F);
        delay_1ms();
    }
    return;
}

void setup(void){
    TRISA = 0x00;
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 1;
    TRISBbits.TRISB2 = 0;
    TRISE = 0;
    ANSEL = 0;
    ANSELH = 0;
}