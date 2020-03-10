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

#define Trigger PORTBbits.RB0
#define Echo PORTBbits.RB1

#define _XTAL_FREQ 4000000

int distancia = 0;
uint16_t fuerza = 0, velocidad = 0, angulo = 0;
uint16_t usCentenas = 0, usDecenas = 0, usUnidades = 0;
uint8_t fCentenas = 0, fDecenas = 0, fUnidades = 0;

void setup(void);

void main(void) {
    setup();
    oscInit(1);
    i2c_master_init(100000);    // Iniciar el I2C como master a 100 KHz.
    lcd8_init();                // Iniciar el LCD (DESPU�S DE INICIAR I2C!!!)
    T1CON= 0x10;
    while(1){
        //--------------------- Lectura del sensor de fuerza ------------------
//        i2c_masterStart();
//        i2c_masterWrite(0x10);
//        i2c_masterWrite(0x01);
//        i2c_masterStop();
//        __delay_ms(1);
        i2c_masterStart();
        i2c_masterWrite(0x11);
        fuerza = i2c_masterRead(0);
        i2c_masterStop();
        __delay_ms(1);
        //--------------------- Lectura de aceler�metro------------------------
//        i2c_masterStart();
//        i2c_masterWrite(0x10);
//        i2c_masterWrite(0x02);
//        i2c_masterStop();
//        __delay_ms(1);
//        i2c_masterStart();
//        i2c_masterWrite(0x11);
//        velocidad = i2c_masterRead(0);
//        i2c_masterStop();
//        __delay_ms(1);
        //--------------------- Lectura de giroscopio -------------------------
        //--------------------- Lectura de 
        //--------------------- Conversi�n del ultras�nico --------------------
        TMR1H = 0;                // Establece TMR1H como 0.
        TMR1L = 0;                // Establece TMR1L como 0.
        Trigger = 1;              // Habilitar el trigger para enciar se�al
        __delay_us(10);           // 10uS Delay 
        Trigger = 0;              // TRIGGER LOW para parar se�al
        while(!Echo);             // Esperando recibir el ECHO (se�al rebotada)
        T1CONbits.TMR1ON = 1;               // Inicia timer
        while(Echo);              // Esperar que ya no se reciba un ECHO.
        T1CONbits.TMR1ON = 0;               // Para el timer
        //------------------- C�lculo de distancia ----------------------------
        distancia = (TMR1L | (TMR1H<<8));
        distancia = distancia/29.412;
        distancia = distancia + 1;
        //-------------------- Funcionamiento de alarma ---------------------
        if (distancia <= 100 && distancia >= 50){
            PORTBbits.RB3 = 1;
            __delay_ms(500);
            PORTBbits.RB3 = 0;
            __delay_ms(1000);
        }
        if (distancia <= 49 && distancia >= 21){
            PORTBbits.RB3 = 1;
            __delay_ms(500);
            PORTBbits.RB3 = 0;
            __delay_ms(500);
        }
        if (distancia < 20){
            PORTBbits.RB3 = 1;            
        }
//        ultrasonico = ultrasonico/29.412;
//        ultrasonico = ultrasonico + 1;
//        ultrasonico = ultrasonico*100;
//        usCentenas = ultrasonico/100;
//        ultrasonico = ultrasonico - usCentenas*100;
//        usDecenas = ultrasonico/10;
//        usUnidades = ultrasonico - usDecenas*10;
        //--------------------- Desplegar ultras�nico en LCD ------------------
        lcd8_setCursor(1,0);
        delay_1ms();
        lcd8_dispString("f:");
        delay_1ms();
        lcd8_setCursor(1,4);
        delay_1ms();
        fCentenas = fuerza/100;
        fuerza = fuerza - fCentenas*100;
        fDecenas = fuerza/10;
        fUnidades = fuerza - fDecenas*10;
        lcd8_dispChar(fUnidades);
        delay_1ms();
        lcd8_setCursor(1,3);
        delay_1ms();
        lcd8_dispChar(fDecenas);
        delay_1ms();
        lcd8_setCursor(1,2);
        delay_1ms();
        lcd8_dispChar(fCentenas);
        delay_1ms();
//        lcd8_dispChar(fuerza%10);
//        delay_1ms();
//        lcd8_setCursor(1,2);
//        delay_1ms();
//        fuerza = fuerza/10;
//        lcd8_dispChar(fuerza%10);
//        delay_1ms();
//        lcd8_setCursor(1,4);
//        delay_1ms();
//        lcd8_dispString("kg");
    }
    return;
}

void setup(void){
    TRISA = 0x00;
    TRISE = 0;
    ANSEL = 0;
    ANSELH = 0;
}