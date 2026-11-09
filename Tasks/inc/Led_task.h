#ifndef LED_TASK_H
#define LED_TASK_H

#include "main.h"
#include "gpio.h"

#define LED_ON(x) HAL_GPIO_WritePin(LED_##x##_GPIO_Port, LED_##x##_Pin, GPIO_PIN_SET)
#define LED_OFF(x) HAL_GPIO_WritePin(LED_##x##_GPIO_Port, LED_##x##_Pin, GPIO_PIN_RESET)

#endif