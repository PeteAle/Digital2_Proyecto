/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef __I2C_2_H_
#define	__I2C_2_H_

#include <xc.h> // include processor files - each processor file is guarded.  

void i2c_master_init(const unsigned long c);
void i2c_wait(void);
void i2c_masterStart(void);
void i2c_masterRestart(void);
void i2c_masterStop(void);
void i2c_masterWrite(unsigned d);
unsigned short i2c_masterRead(unsigned short ack);
void i2c_slave_init(short address);

#endif	/* XC_HEADER_TEMPLATE_H */

