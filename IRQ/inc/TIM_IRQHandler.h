#ifndef TIM_IRQHANDLER_H
#define TIM_IRQHANDLER_H

#include "main.h"
#include "POSI_task.h"

#define PERIOD 10

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim);

#endif