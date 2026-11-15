#ifndef POSI_TASK_H
#define POSI_TASK_H

#include "tim.h"
#include "can.h"
#include <math.h>

#define CM_PER_CNT_1 0.003997037  // 轮径系数。前人祖传的数据，待标定
#define CM_PER_CNT_2 0.003987676  

typedef struct{
    TIM_HandleTypeDef *htim;
    int16_t last_cnt;
    int16_t cur_cnt;

    int32_t sum_cnt;
} Encoder;

typedef struct{
    int16_t raw_vx_car;
    int16_t raw_vy_car;
    int16_t vx_car;
    int16_t vy_car;
} Odom;

void Encoder_init(Encoder *enc, TIM_HandleTypeDef *htim);
int16_t get_delta_cnt(Encoder *enc);

#endif