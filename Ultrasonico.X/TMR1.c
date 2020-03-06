/*
 * File:   TMR1.c
 * Author: Peter
 *
 * Created on March 4, 2020, 2:48 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "OSCCON.h"

#define _XTAL_FREQ 4000000

void tmr1_Init(void){
    T1CONbits.T1OSCEN = 0;
    T1CONbits.TMR1CS = 0;
    T1CONbits.TMR1ON = 1;
}

void tmr1_Prescaler(unsigned char prescaler){
    switch(prescaler){
        case 0:                     // Prescaler de 1:1
            T1CONbits.T1CKPS1 = 0;
            T1CONbits.T1CKPS0 = 0;
            break;
        case 1:                     // Prescaler de 1:2
            T1CONbits.T1CKPS1 = 0;
            T1CONbits.T1CKPS0 = 1;
            break;
        case 2:                     // Prescaler de 1:4
            T1CONbits.T1CKPS1 = 1;
            T1CONbits.T1CKPS0 = 0;
            break;
        case 3:                     // Prescaler de 1:8
            T1CONbits.T1CKPS1 = 1;
            T1CONbits.T1CKPS0 = 1;
            break;
    }
}

void tmr1_Interrupt(unsigned char tmr1_Int){
    PIE1bits.TMR1IE = tmr1_Int;
    PIR1bits.TMR1IF = 0;
}