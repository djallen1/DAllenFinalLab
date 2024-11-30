/*
 * Scheduler.h
 *
 *  Created on: Sep 10, 2024
 *      Author: 17202
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include <stdint.h>
#define APP_DELAY_FLAG_EVENT (1 << 0)
#define DEVICE_ID_AND_TEMP_EVENT (1 << 1)

uint32_t getScheduledEvents();
void addSchedulerEvents(uint32_t);
void removeSchedulerEvents(uint32_t);

#endif /* SCHEDULER_H_ */
