/*
 * GyroDriver.h
 *
 *  Created on: Nov 1, 2024
 *      Author: 17202
 */

#ifndef INC_GYRODRIVER_H_
#define INC_GYRODRIVER_H_

#include "ErrorHandling.h"
#include "stm32f4xx_hal.h"

#define GPIO_MISO_PIN GPIO_PIN_8
#define GPIO_MOSI_PIN GPIO_PIN_9
#define GPIO_SCK_PIN GPIO_PIN_7
#define GPIO_CS_PIN GPIO_PIN_1

#define WHO_AM_I_ADDR 0x0F
#define CTRL_REG1_ADDR 0x20
#define CTRL_REG2_ADDR 0x21
#define CTRL_REG3_ADDR 0x22
#define CTRL_REG4_ADDR 0x23
#define CTRL_REG5_ADDR 0x24
#define REF_DATACAP_ADDR 0x25
#define OUT_TEMP_ADDR 0x26
#define STATUS_ADDR 0x27
#define OUT_XL_ADDR 0x28
#define OUT_XH_ADDR 0x29
#define OUT_YL_ADDR 0x2A
#define OUT_YH_ADDR 0x2B
#define OUT_ZL_ADDR 0x2C
#define OUT_ZH_ADDR 0x2D
#define FIFO_CTRL_ADDR 0x2E
#define FIFO_SRC_ADDR 0x2F
#define INT1_CFG_ADDR 0x30
#define INT1_SRC_ADDR 0x31
#define INT1_THS_XH_ADDR 0x32
#define INT1_THS_XL_ADDR 0x33
#define INT1_THS_YH_ADDR 0x34
#define INT1_THS_YL_ADDR 0x35
#define INT1_THS_ZH_ADDR 0x36
#define INT1_THS_ZL_ADDR 0x37
#define INT1_DURATION_ADDR 0x38

#define VDD_IO_PIN 1
#define SPC_PIN 2
#define SDI_PIN 3
#define SDO_PIN 4
#define CS_PIN 5
#define DRDY_PIN 6
#define INT1_PIN 7
#define GND_PIN 13
#define PLLFFILT_PIN 14
#define VDD_PIN 16

#define DATA_SIZE  2
#define BLOCK_SIZE 8
#define TIMEOUT 2000
#define READ  (0x1 << 7)
#define WRITE 0x0000
#define GARBAGE_VAL (0x1 << 15)
#define EXTRACT_MASK 0xFF00
#define GYRO_POWER_BITSHIFT 3
#define GYRO_BOOT_BITSHIFT 7

void Gyro_Init();
uint8_t Gyro_getID();
void Gyro_powerOn();
uint8_t Gyro_getTemp();
void Gyro_configReg(uint32_t reg, uint32_t value);
uint32_t Gyro_readReg(uint32_t reg);
void Gyro_enableSlaveComm();
void Gyro_disableSlaveComm();
void Gyro_HAL_Status();



#endif /* INC_GYRODRIVER_H_ */
