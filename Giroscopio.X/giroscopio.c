/*
 * File:   giroscopio.c
 * Author: Peter
 *
 * Created on March 11, 2020, 12:30 AM
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
#include <math.h>
#include "OSCCON.h"
#include "MPU6050.h"
#include "I2C.h"
#include "UART.h"

#define _XTAL_FREQ 4000000

uint8_t receive = 0, adc = 0;
uint16_t Ax = 0, Ay = 0, Az = 0, Gx = 0, Gy = 0, Gz = 0, T = 0;
uint16_t AX = 0, AY = 0, AZ = 0, GX = 0, GY = 0, GZ = 0, t = 0;
char buffer[12];
char buffer2[12];

void setup(void);
void globalInt(void);

void __interrupt() isr(){
    if (PIR1bits.RCIF == 1){
        receive = RCREG;
        PIR1bits.RCIF = 0;
    }
}

void main(void) {
    setup();
    globalInt();
    oscInit(1);
    uart_init();
    uart_9bit(0,0);
    txrx_en(1,1);
    baudRate(1,0,25);
    uart_interrupts(1,0);
    i2c_master_init(100000);
    mpu6050_init();
    while(1){
        i2c_addr_start(0xD0);
        i2c_masterWrite(ACCEL_XOUT_H);
        i2c_masterStop();
        i2c_addr_start(0xD1);
        Ax = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
//        i2c_masterStop();
//        i2c_addr_start(0xD0);
//        i2c_masterWrite(ACCEL_YOUT_H);
//        i2c_masterStop();
//        i2c_addr_start(0xD1);
        Ay = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
        Az = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
        T  = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
        Gx = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
        Gy = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
        Gz = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(1);
        i2c_masterStop();
        
        AX = (int)Ax/16384.0;
        AY = (int)Ay/16384.0;
        AZ = (int)Az/16384.0;
        GX = (int)Gx/131.0;
        GY = (int)Gy/131.0;
        GZ = (int)Gz/131.0;
        t = ((int)T/340.00)+36.53;
        
        sprintf(buffer, "Ax = %d    ",AX);
        uart_sendString(buffer);
      
        sprintf(buffer, "Ay = %d    ",AY);
        uart_sendString(buffer);
        
        sprintf(buffer," Az = %d    ",AZ);
        uart_sendString(buffer);
 
        sprintf(buffer," T = %d      ",t);
        uart_sendString(buffer);
 
        sprintf(buffer," Gx = %d    ",GX);
        uart_sendString(buffer);
 
        sprintf(buffer," Gy = %d    ",GY);
        uart_sendString(buffer);
 
        sprintf(buffer," Gz = %d\r\n",GZ);
        uart_sendString(buffer);
        
//        uart_sendString("A");
        
        __delay_ms(500);
    }
    return;
}

void setup(void){
    TRISC = 0x00;
    TRISA = 0x00;
    PORTA = 0x00;
    TRISB = 0xFF;
    ANSEL = 0x00;
    ANSELHbits.ANS12 = 1;
}

void globalInt(void){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
}
