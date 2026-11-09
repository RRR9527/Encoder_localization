#include "POSI_task.h"

volatile Encoder encoder[2];  // 0号代表x轴的码盘，1号代表y轴的码盘。
volatile Odom odom = {
    .pos_x = 0, 
    .pos_y = 0,
    .v_car_x = 0,
    .v_car_y = 0
};

void Encoder_init(Encoder *enc, TIM_HandleTypeDef *htim)
{
    enc->htim = htim;
    enc->last_cnt = 0x7FFF;
    enc->cur_cnt  = 0x7FFF;

    __HAL_TIM_SET_COUNTER(htim, 0x7fff);
}

int16_t get_delta_cnt(Encoder *enc)
{
    enc->cur_cnt = (int16_t) __HAL_TIM_GET_COUNTER(enc->htim);

    int16_t delta_cnt = enc->cur_cnt - enc->last_cnt;
    enc->last_cnt = enc->cur_cnt;

    return delta_cnt;
}

void StartPOSI_Task(void *argument)
{

    Encoder_init(&encoder[0], &htim3);
    Encoder_init(&encoder[1], &htim5);

    while (1)
    {
        // 处理主板信息，并通过CAN发送速度信息
    }
}