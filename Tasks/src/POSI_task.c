#include "POSI_task.h"

volatile Encoder encoder[2];  // 0号代表x轴的码盘，1号代表y轴的码盘。
volatile Odom odom = {
    .raw_vx_car = 0,
    .raw_vy_car = 0,
    .vx_car = 0,
    .vy_car = 0,
};

void Encoder_init(Encoder *enc, TIM_HandleTypeDef *htim)
{
    enc->htim = htim;
    enc->last_cnt = 0x7FFF;
    enc->cur_cnt  = 0x7FFF;
    enc->sum_cnt  = 0; 

    __HAL_TIM_SET_COUNTER(htim, 0x7fff);
}

int16_t get_delta_cnt(Encoder *enc)
{
    enc->cur_cnt = (int16_t) __HAL_TIM_GET_COUNTER(enc->htim);

    int16_t delta_cnt = enc->cur_cnt - enc->last_cnt;
    enc->last_cnt = enc->cur_cnt;
    enc->sum_cnt += delta_cnt;

    return delta_cnt;
}

void StartPOSI_Task(void *argument)
{
    while (1)
    {
        CAN_TxHeaderTypeDef txheader = {0};
        uint8_t txdata[8] = {0};
        uint32_t txmailbox;

        float v_x = odom.raw_vx_car * CM_PER_CNT_1 * 1000.0f;  // 单位：mm/s
        float v_y = odom.raw_vy_car * CM_PER_CNT_2 * 1000.0f;
        odom.vx_car = (int16_t)lroundf(0.1f * odom.vx_car + 0.9f * v_x);  // 加了一点低通滤波
        odom.vy_car = (int16_t)lroundf(0.1f * odom.vy_car + 0.9f* v_y);
        // 处理主板信息，并通过CAN发送速度信息

        txheader.DLC = 8U;
        txheader.IDE = CAN_ID_STD;
        txheader.RTR = CAN_RTR_DATA;
        txheader.StdId = 0x210;

        txdata[0] = (uint8_t)(odom.vx_car >> 8);
        txdata[1] = (uint8_t)(odom.vx_car & 0x00FF);
        txdata[2] = (uint8_t)(odom.vy_car >> 8);
        txdata[3] = (uint8_t)(odom.vy_car & 0x00FF);

        HAL_CAN_AddTxMessage(&hcan1, &txheader, txdata, &txmailbox);
        osDelay(10);
    }
}
