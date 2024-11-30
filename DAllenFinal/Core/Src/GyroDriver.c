/*
 * GyroDriver.c
 *
 *  Created on: Nov 1, 2024
 *      Author: 17202
 */
#include "GyroDriver.h"

static HAL_StatusTypeDef HAL_STATUS;
static SPI_HandleTypeDef hspi5 = {0};
static uint16_t DataIn;
static uint16_t DataOut;

void Gyro_Init()
{
	hspi5.Instance = SPI5;
	hspi5.Init.Mode = SPI_MODE_MASTER;
	hspi5.Init.Direction = SPI_DIRECTION_2LINES;
	hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
	hspi5.Init.CLKPolarity = SPI_POLARITY_HIGH;
	hspi5.Init.CLKPhase = SPI_PHASE_2EDGE;
	hspi5.Init.NSS = SPI_NSS_SOFT;
	hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
	hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
	hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
	hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
	hspi5.Init.CRCPolynomial = 0;

	Gyro_HAL_Status();

	GPIO_InitTypeDef Gyro = {0};
	__HAL_RCC_GPIOF_CLK_ENABLE();

	Gyro.Pin = GPIO_MISO_PIN | GPIO_MOSI_PIN | GPIO_SCK_PIN;
	Gyro.Mode = GPIO_MODE_AF_PP;
	Gyro.Pull = GPIO_NOPULL;
	Gyro.Speed = GPIO_SPEED_FREQ_LOW;
	Gyro.Alternate = GPIO_AF5_SPI5;
	HAL_GPIO_Init(GPIOF, &Gyro);

	GPIO_InitTypeDef CS = {0};
	__HAL_RCC_GPIOC_CLK_ENABLE();

	CS.Pin = GPIO_CS_PIN;
	CS.Mode = GPIO_MODE_OUTPUT_PP;
	CS.Pull = GPIO_NOPULL;
	CS.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC, &CS);

	Gyro_disableSlaveComm();
	Gyro_powerOn();
	Gyro_configReg(CTRL_REG5_ADDR, (SET << GYRO_BOOT_BITSHIFT));
}

uint8_t Gyro_getID()
{
	return Gyro_readReg(WHO_AM_I_ADDR);
}

void Gyro_powerOn()
{
	Gyro_configReg(CTRL_REG1_ADDR, (SET << GYRO_POWER_BITSHIFT));
}

uint8_t Gyro_getTemp()
{
	return Gyro_readReg(OUT_TEMP_ADDR);
}

void Gyro_configReg(uint32_t reg, uint32_t value)
{
	//power down mode in normal mode  ->  CR1 |= (0x1 << 3)
	//bandwidth field in default of 0x01  ->  CR1 |= (0x00 << 4) || CR1 |= (0x01 << 4)

	//Control register 5 reboot content enabled CR5 |= (0x## << #)
	DataOut = (value << BLOCK_SIZE) | reg;
	DataIn = RESET;

	Gyro_enableSlaveComm();

	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_RESET )
	{
		HAL_STATUS = HAL_SPI_Transmit(&hspi5,(uint8_t*) &DataOut, DATA_SIZE, TIMEOUT);
	}

	Gyro_disableSlaveComm();
}

uint32_t Gyro_readReg(uint32_t reg)
{
	DataOut = (READ | reg) | GARBAGE_VAL;
	DataIn = RESET;

	Gyro_enableSlaveComm();

	if(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_1) == GPIO_PIN_RESET )
	{
		HAL_STATUS = HAL_SPI_TransmitReceive(&hspi5,(uint8_t*) &DataOut,(uint8_t*) &DataIn, DATA_SIZE, TIMEOUT);
	}

	Gyro_disableSlaveComm();
	Gyro_HAL_Status();

	return ((EXTRACT_MASK & DataIn) >> BLOCK_SIZE);
}

void Gyro_enableSlaveComm()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, RESET);
}

void Gyro_disableSlaveComm()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, SET);
}

void Gyro_HAL_Status()
{
	if (HAL_SPI_Init(&hspi5) != HAL_OK)
	{
		bool tf = false;
		APPLICATION_ASSERT(tf);
	}
}
