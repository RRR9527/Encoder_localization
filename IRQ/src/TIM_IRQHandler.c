#include "TIM_IRQHandler.h"

static uint8_t read_times = 0;

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        if (++read_times >= PERIOD)  // 每隔10ms测量一次速度
        {
            odom.raw_vx_car = get_delta_cnt(&encoder[0]);  // 原始脉冲
            odom.raw_vy_car = get_delta_cnt(&encoder[1]);
            read_times %= PERIOD;
        } 
    }
}
