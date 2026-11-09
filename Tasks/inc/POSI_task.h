#ifndef POSI_TASK_H
#define POSI_TASK_H

#include "main.h"
#include "tim.h"

#define CM_PER_CNT_1 0.003997037
#define CM_PER_CNT_2 0.003987676  // 轮径系数。前人的数据，待标定

typedef struct{
    TIM_HandleTypeDef *htim;
    int16_t last_cnt;
    int16_t cur_cnt;
} Encoder;

typedef struct{
    float pos_x;
    float pos_y;
    float v_car_x;
    float v_car_y;
} Odom;

void Encoder_init(Encoder *enc, TIM_HandleTypeDef *htim);
int16_t get_delta_cnt(Encoder *enc);

#endif