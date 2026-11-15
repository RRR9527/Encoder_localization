#ifndef TIM_IRQHANDLER_H
#define TIM_IRQHANDLER_H

#include "main.h"
#include "POSI_task.h"

#define PERIOD 100

volatile extern Encoder encoder[2];
volatile extern Odom odom;

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif