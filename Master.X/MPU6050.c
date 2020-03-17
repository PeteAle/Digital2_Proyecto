/*
 * File:   I2C.c
 * Author: Daniel
 *
 * Created on February 24, 2020, 10:46 AM
 */

#include <xc.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "I2C.h"
#include "MPU6050.h"
//#include "LCD.h"
//#include "UART.h"  // for debugging serial terminal

//-------------[ MPU6050 Routines ]------------------
//---------------------------------------------------

void mpu6050_init(void){
  // Power-Up Delay & I2C_Init
  __delay_ms(100);
//  i2c_master_init(100000);
 
  //------------------- Setting The Sample (Data) Rate ------------------------
    i2c_addr_start(0xD0);
    i2c_masterWrite(SMPLRT_DIV);
    i2c_masterWrite(0x07);
    i2c_masterStop();
 
  //--------------------- Setting The Clock Source ----------------------------
    i2c_addr_start(0xD0);
    i2c_masterWrite(PWR_MGMT_1);
    i2c_masterWrite(0x00);
    i2c_masterStop();
  
  //------------------------ Configure The DLPF -------------------------------
    i2c_addr_start(0xD0);
    i2c_masterWrite(CONFIG);
    i2c_masterWrite(0x00);
    i2c_masterStop();
 
  //------------------- Configure The ACCEL (FSR= +-2g) -----------------------
    i2c_addr_start(0xD0);
    i2c_masterWrite(ACCEL_CONFIG);
    i2c_masterWrite(0x00);
    i2c_masterStop();
 
  //------------------ Configure The GYRO (FSR= +-2000d/s) --------------------
    i2c_addr_start(0xD0);
    i2c_masterWrite(GYRO_CONFIG);
    i2c_masterWrite(0x12);
    i2c_masterStop();
 
  //----------------------- Enable Data Ready Interrupts ----------------------
    i2c_addr_start(0xD0);
    i2c_masterWrite(INT_ENABLE);
    i2c_masterWrite(0x01);
    i2c_masterStop();
}

//void mpu6050_read(void){
//  char buffer[40];
//  uint16_t Ax = 0, Ay = 0, Az = 0, T = 0, Gx = 0, Gy = 0, Gz = 0;
//  // Prepare For Reading, Starting From ACCEL_XOUT_H
//  i2c_addr_start(0xD0);
//  i2c_masterWrite(ACCEL_XOUT_H);
//  i2c_masterStop();
//  i2c_addr_start(0xD1);
//  Ax = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
//  Ay = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
//  Az = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
//  T  = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
//  Gx = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
//  Gy = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(0);
//  Gz = ((int)i2c_masterRead(0)<<8) | (int)i2c_masterRead(1);
//  i2c_masterStop();
  
//  lcd8_clearDisplay();
//  delay_1ms();
//  lcd8_setCursor(2,0);
//  delay_1ms();
//  lcd8_dispString("Ax:");
//  delay_1ms();
//  lcd8_setCursor(2,3);
//  delay_1ms();
//  sprintf(buffer, "%3u", Ax);
//}
  
//  lcd8_cmd(0x01); 
//  lcd8_setCursor(1,1);
//  lcd8_write("Ax    Ay    Az");
// 
//  sprintf(buffer,"Ax = %d    ",Ax);
//  //UART_Write_String(buffer);
//  lcd8_setCursor(2,1);
//  lcd8_write(buffer);
//  
//  sprintf(buffer," Ay = %d    ",Ay);
//  //UART_Write_String(buffer);
// 
//  sprintf(buffer," Az = %d    ",Az);
//  //UART_Write_String(buffer);
// 
//  sprintf(buffer," T = %d  ",T);
//  //UART_Write_String(buffer);
// 
//  sprintf(buffer," Gx = %d    ",Gx);
//  //UART_Write_String(buffer);
// 
//  sprintf(buffer," Gy = %d    ",Gy);
//  //UART_Write_String(buffer);
// 
//  sprintf(buffer," Gz = %d\r\n",Gz);
//  //UART_Write_String(buffer);
//}