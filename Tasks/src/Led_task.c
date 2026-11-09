#include "Led_task.h"

void StartLEDTask(void *argument)
{
    while (1)
    {
        LED_ON(1);
        osDelay(1);
        LED_OFF(1);
        osDelay(1);

        LED_ON(2);
        osDelay(1);
        LED_OFF(2);
        osDelay(1);   
    }
}