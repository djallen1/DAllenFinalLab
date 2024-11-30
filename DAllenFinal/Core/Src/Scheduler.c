/*
 * Scheduler.c
 *
 *  Created on: Sep 10, 2024
 *      Author: 17202
 */

#ifndef SCHEDULER_C_
#define SCHEDULER_C_
#include "Scheduler.h"

static uint32_t scheduledEvents;

uint32_t getScheduledEvents()
{
	return scheduledEvents;
}

void addSchedulerEvents(uint32_t event)
{
	scheduledEvents |= event;
}

void removeSchedulerEvents(uint32_t event)
{
	scheduledEvents &= ~event;
}

#endif /* SCHEDULER_C_ */
