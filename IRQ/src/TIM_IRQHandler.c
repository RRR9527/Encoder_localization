#include "TIM_IRQHandler.h"

extern Encoder encoder[2];
extern Odom odom;
static uint8_t read_times = 0;

void TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        if (read_times++ == 10)
        {
            odom.v_car_x = get_delta_cnt(&encoder[0]) * CM_PER_CNT_1 * 100.0;  // 单位：cm/s
            odom.v_car_y = get_delta_cnt(&encoder[1]) * CM_PER_CNT_2 * 100.0;
        } 
    }
}
