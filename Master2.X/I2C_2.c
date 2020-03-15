/*
 * File:   I2C_2.c
 * Author: Peter
 *
 * Created on March 14, 2020, 12:13 AM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "I2C_2.h"

#define _XTAL_FREQ 4000000

void i2c_master_init(const unsigned long c){
    SSPCON = 0b00101000;            //SSP Module as Master
    SSPCON2 = 0;
    SSPADD = (_XTAL_FREQ/(4*c))-1; //Setting Clock Speed
    SSPSTAT = 0;
    TRISCbits.TRISC3 = 1;          //Setting as input as given in datasheet
    TRISCbits.TRISC4 = 1;          //Setting as input as given in datasheet
}

void i2c_wait(void){
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F)); //Transmit is in progress
}

void i2c_masterStart(void){
    i2c_wait();    
    SSPCON2bits.SEN = 1;             //Initiate start condition
}

void i2c_masterRestart(void){
    i2c_wait();
    SSPCON2bits.RSEN = 1;           //Initiate repeated start condition
}

void i2c_masterStop(void){
    i2c_wait();
    SSPCON2bits.PEN = 1;           //Initiate stop condition
}

void i2c_masterWrite(unsigned d){
    i2c_wait();
    SSPBUF = d;         //Write data to SSPBUF
    while(SSPSTATbits.BF);
}

unsigned short i2c_masterRead(unsigned short ack){
    unsigned short temp;
    i2c_wait();
    SSPCON2bits.RCEN = 1;
    i2c_wait();
    temp = SSPBUF;      //Read data from SSPBUF
    i2c_wait();
    if (ack == 0){
        SSPCON2bits.ACKDT = 1;
    }
    else if (ack == 1){
        SSPCON2bits.ACKDT = 0;
    }
    SSPCON2bits.ACKEN = 1;          //Acknowledge sequence
    return temp;
}

void i2c_slave_init(short address){
    SSPSTAT = 0x80;    
    SSPADD = address; //Setting address
    SSPCON = 0x36;    //As a slave device
    SSPCON2 = 0x01;
    TRISCbits.TRISC3 = 1;       //Setting as input as given in datasheet
    TRISCbits.TRISC4 = 1;       //Setting as input as given in datasheet
    INTCONbits.GIE = 1;          //Global interrupt enable
    INTCONbits.PEIE = 1;         //Peripheral interrupt enable
    PIR1bits.SSPIF = 0;        //Clear interrupt flag
    PIE1bits.SSPIE = 1;        //Synchronous serial port interrupt enable
}