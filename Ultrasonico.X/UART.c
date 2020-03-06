/*
 * File:   UART.c
 * Author: Peter
 *
 * Created on February 19, 2020, 11:43 PM
 */


#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include "UART.h"

void uart_init(void){
    TRISCbits.TRISC6 = 0;
    TRISCbits.TRISC7 = 1;
    //---------TXSTA-------------
    TXSTAbits.SYNC = 0;
    TXSTAbits.TRMT = 0;
    //---------RCSTA-------------
    RCSTAbits.SPEN = 1;
    RCSTAbits.FERR = 0;
    RCSTAbits.OERR = 0;
    //---------BAUDCTL-----------
    BAUDCTLbits.ABDOVF = 0;
    BAUDCTLbits.SCKP = 0;
    BAUDCTLbits.WUE = 0;
    BAUDCTLbits.ABDEN = 0;
}

void uart_bitNum(unsigned char rx9, unsigned char tx9){
    TXSTAbits.TX9 = tx9;
    RCSTAbits.RX9 = rx9;
}

void txrx_En(unsigned char tx, unsigned char rx){
    TXSTAbits.TXEN = tx;
    RCSTAbits.CREN = rx;
}

void baudRate(unsigned char rateSel, unsigned char baudN, unsigned char rateGen){
    TXSTAbits.BRGH = rateSel;
    BAUDCTLbits.BRG16 = rateGen;
    SPBRG = baudN;
}

void uart_interrupts(unsigned char rx_int, unsigned char tx_int){
    PIE1bits.RCIE = rx_int;
    PIR1bits.RCIF = 0;
    PIE1bits.TXIE = tx_int;
    PIR1bits.TXIF = 0;
}

void uart_transmit(uint8_t transmit){
    while(!PIR1bits.TXIF){
        TXREG = transmit;
    }
    PIR1bits.TXIF = 0;
}

unsigned char uart_receive(void){
    if (RCSTAbits.FERR || RCSTAbits.OERR){
        uint8_t x = 0;
        x = RCREG;
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    while(!PIR1bits.RCIF);
    return RCREG;
    PIR1bits.RCIF = 0;
}

//uint8_t uart_txCheck(void){
//    return 
//}