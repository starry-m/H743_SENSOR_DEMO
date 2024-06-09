/*
 * mems_control.c
 *
 *  Created on: Jun 9, 2024
 *      Author: pc
 */

#include "mems_control.h"
#include "iks4a1_motion_sensors.h"
#include "lsm6dsv16x_reg.h"
#include "lsm6dsv16x.h"


extern void* MotionCompObj[IKS4A1_MOTION_INSTANCES_NBR];

void BSP_SENSOR_QVAR_GetValue(int16_t* Axes)
{
  (void)LSM6DSV16X_QVAR_GetData(MotionCompObj[IKS4A1_LSM6DSV16X_0], Axes);
}


uint8_t QVAR_action_check_statemachine(const uint16_t qvar_value)
{
    static uint32_t tick_cur = HAL_GetTick();
    static uint32_t tick_prev = 0;




    tick_prev = tick_cur;

    return 0;

}


