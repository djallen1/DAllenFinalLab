/*
 * ErrorHandling.c
 *
 *  Created on: Nov 4, 2024
 *      Author: 17202
 */
#include "ErrorHandling.h"


void APPLICATION_ASSERT(bool tf)
{
	__disable_irq();
	while(tf!=HAL_OK)
	{}		//loop forever
}
