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
    TRISCbits.TRISC6 = 1;
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

void uart_9bit(unsigned char rx9, unsigned char tx9){
    TXSTAbits.TX9 = tx9;
    RCSTAbits.RX9 = rx9;
}

void txrx_en(unsigned char tx, unsigned char rx){
    TXSTAbits.TXEN = tx;
    RCSTAbits.CREN = rx;
}

void baudRate(unsigned char rateSel, unsigned char rateGen, unsigned char baudN){
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
    while(!TXSTAbits.TRMT);
    TXREG = transmit;
}

void uart_sendString(char *text){
    int i = 0;
    while(text[i] != '\0')
        uart_transmit(text[i++]);
}

unsigned char uart_receive(void){
    if (RCSTAbits.FERR || RCSTAbits.OERR){
//        uint8_t receive = 0;
//        receive = RCREG;
        RCSTAbits.CREN = 0;
        RCSTAbits.CREN = 1;
    }
    while(!PIR1bits.RCIF);
    return RCREG;
}

//uint8_t uart_txCheck(void){
//    return 
//}