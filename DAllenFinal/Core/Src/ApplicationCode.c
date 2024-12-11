/*
 * ApplicationCode.c
 *
 *  Created on: Dec 30, 2023 (updated 11/12/2024) Thanks Donavon! 
 *      Author: Xavion
 */

#include "ApplicationCode.h"

TIM_HandleTypeDef htim7;

static void MX_TIM7_Init(void);

/* Static variables */
extern void initialise_monitor_handles(void); 

#if COMPILE_TOUCH_FUNCTIONS == 1
static STMPE811_TouchData StaticTouchData;
#if TOUCH_INTERRUPT_ENABLED == 1
static EXTI_HandleTypeDef LCDTouchIRQ;
void LCDTouchScreenInterruptGPIOInit(void);
#endif // TOUCH_INTERRUPT_ENABLED
#endif // COMPILE_TOUCH_FUNCTIONS

void ApplicationInit(void)
{
	initialise_monitor_handles(); // Allows printf functionality
    LTCD__Init();
    LTCD_Layer_Init(0);
    LCD_Clear(0,LCD_COLOR_BLACK);

    #if COMPILE_TOUCH_FUNCTIONS == 1
	InitializeLCDTouch();

	// This is the orientation for the board to be direclty up where the buttons are vertically above the screen
	// Top left would be low x value, high y value. Bottom right would be low x value, low y value.
	StaticTouchData.orientation = STMPE811_Orientation_Portrait_2;

	#if TOUCH_INTERRUPT_ENABLED == 1
	LCDTouchScreenInterruptGPIOInit();
	#endif // TOUCH_INTERRUPT_ENABLED
	#endif // COMPILE_TOUCH_FUNCTIONS
	applicationButtonInit();
	game_start_screen();
}

void Application_game_inst(void)
{
	game_init();
	start_timer();
}

// TouchScreen Interrupt
#if TOUCH_INTERRUPT_ENABLED == 1

void LCDTouchScreenInterruptGPIOInit(void)
{
	GPIO_InitTypeDef LCDConfig = {0};
    LCDConfig.Pin = GPIO_PIN_15;
    LCDConfig.Mode = GPIO_MODE_IT_RISING_FALLING;
    LCDConfig.Pull = GPIO_NOPULL;
    LCDConfig.Speed = GPIO_SPEED_FREQ_HIGH;
    
    // Clock enable
    __HAL_RCC_GPIOA_CLK_ENABLE();

    // GPIO Init      
    HAL_GPIO_Init(GPIOA, &LCDConfig);

    // Interrupt Configuration
    HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	LCDTouchIRQ.Line = EXTI_LINE_15;

}

#define TOUCH_DETECTED_IRQ_STATUS_BIT   (1 << 0)  // Touchscreen detected bitmask

static uint8_t statusFlag;

void EXTI15_10_IRQHandler()
{
	HAL_NVIC_DisableIRQ(EXTI15_10_IRQn); // May consider making this a universial interrupt guard
	bool isTouchDetected = false;

	static uint32_t count;
	count = 0;
	while(count == 0){
		count = STMPE811_Read(STMPE811_FIFO_SIZE);
	}

	// Disable touch interrupt bit on the STMPE811
	uint8_t currentIRQEnables = ReadRegisterFromTouchModule(STMPE811_INT_EN);
	WriteDataToTouchModule(STMPE811_INT_EN, 0x00);

	// Clear the interrupt bit in the STMPE811
	statusFlag = ReadRegisterFromTouchModule(STMPE811_INT_STA);
	uint8_t clearIRQData = (statusFlag | TOUCH_DETECTED_IRQ_STATUS_BIT); // Write one to clear bit
	WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);
	
	uint8_t ctrlReg = ReadRegisterFromTouchModule(STMPE811_TSC_CTRL);
	if (ctrlReg & 0x80)
	{
		isTouchDetected = true;
	}

	// Determine if it is pressed or unpressed
	if(isTouchDetected) // Touch has been detected
	{
		if(is_start_screen())
		{
			Application_game_inst();
		}
		else
		{
			DetermineTouchPosition(&StaticTouchData);
			if(StaticTouchData.x > 120)
			{
				game_move(RIGHT);
			}
			else
			{
				game_move(LEFT);
			}
		}
	}

	STMPE811_Write(STMPE811_FIFO_STA, 0x01);
	STMPE811_Write(STMPE811_FIFO_STA, 0x00);

	// Re-enable IRQs
    WriteDataToTouchModule(STMPE811_INT_EN, currentIRQEnables);
	HAL_EXTI_ClearPending(&LCDTouchIRQ, EXTI_TRIGGER_RISING_FALLING);

	HAL_NVIC_ClearPendingIRQ(EXTI15_10_IRQn);
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

	//Potential ERRATA? Clearing IRQ bit again due to an IRQ being triggered DURING the handling of this IRQ..
	WriteDataToTouchModule(STMPE811_INT_STA, clearIRQData);

}
#endif // TOUCH_INTERRUPT_ENABLED

void applicationButtonInit()
{
	ButtonInit();
}

void EXTI0_IRQHandler()
{
	HAL_NVIC_DisableIRQ(EXTI0_IRQn);
	//EXTI_HandleTypeDef* line = {0};
	HAL_EXTI_ClearPending(EXTI_GPIOA, EXTI_TRIGGER_RISING);
	//IRQ_enable(EXTI0_IRQ_NUMBER);
	game_rotate();
	HAL_NVIC_EnableIRQ(EXTI0_IRQn);
}

static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = PRESCALAR_VAL;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = ARR_VAL;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    APPLICATION_ASSERT(1);
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
	APPLICATION_ASSERT(1);
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

void TIM7_IRQHandler(void)
{
	HAL_NVIC_DisableIRQ(TIM7_IRQn);
	__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
	if(!game_finished())
	{
		game_drop();
	}
	else
	{
		game_over();
	}
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
}

void start_timer()
{
	MX_TIM7_Init();
	__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
	HAL_NVIC_EnableIRQ(TIM7_IRQn);
	HAL_TIM_Base_Start_IT(&htim7);
	//((&htim7)->Instance->SR) |= 0;
	//__HAL_TIM_CLEAR_FLAG(&htim7, TIM_FLAG_UPDATE);
}

