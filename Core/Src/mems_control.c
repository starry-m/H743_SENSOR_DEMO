/*
 * mems_control.c
 *
 *  Created on: Jun 9, 2024
 *      Author: pc
 */

#include "mems_control.h"
#include "iks4a1_motion_sensors.h"
#include "lsm6dsv16x.h"
#include "lsm6dsv16x_reg.h"

#include "main.h"


extern void *MotionCompObj[IKS4A1_MOTION_INSTANCES_NBR];

void BSP_SENSOR_QVAR_GetValue(int16_t *Axes)
{
    (void)LSM6DSV16X_QVAR_GetData(MotionCompObj[IKS4A1_LSM6DSV16X_0], Axes);
}
#define ACTION_IDLE 0
#define ACTION_FIRST_UP 1
#define ACTION_FIRST_DOWN 2
#define ACTION_FIRST_MIDDLE 3
#define ACTION_SECOND_UP 4
#define ACTION_SECOND_DOWN 5
#define ACTION_END_MIDDLE 6

#define JUDGE_UP_VALUE 400
#define JUDGE_DOWN_VALUE -400
#define JUDGE_MIDDLE_UP 100
#define JUDGE_MIDDLE_DOWN -100

#define RESULT_LEFT_SINGEL_CLICK 1
#define RESULT_RIGHT_SINGEL_CLICK 2
#define RESULT_LEFT_DOUBLE_CLICK 3
#define RESULT_RIGHT_DOUBLE_CLICK 4
#define RESULT_LEFT_SLIP 5
#define RESULT_RIGHT_SLIP 6
#define RESULT_LEFT_LONG_PRESSED 7
#define RESULT_RIGHT_LONG_PRESSED 8

uint8_t QVAR_action_check_statemachine(const int16_t qvar_value)
{
    static uint32_t tick_cur =0;
    static uint32_t tick_prev = 0;
    static uint8_t cur_state = ACTION_IDLE;
    static uint8_t prev_state = ACTION_IDLE;

    tick_cur = HAL_GetTick();
//    printf("--qvr=%d\n", qvar_value);
    switch (cur_state)
    {
    case ACTION_IDLE:
        if (qvar_value > JUDGE_UP_VALUE)
        {
            cur_state = ACTION_FIRST_UP;
            printf("\r\n----ACTION_IDLE->ACTION_FIRST_UP---\r\n");
        }
        else if (qvar_value < JUDGE_DOWN_VALUE)
        {
            cur_state = ACTION_FIRST_DOWN;
            printf("\r\n----ACTION_IDLE->ACTION_FIRST_DOWN---\r\n");
        }
        else
        {
            cur_state = ACTION_IDLE;
            prev_state = ACTION_IDLE;
        }
        tick_prev = tick_cur;
        break;
    case ACTION_FIRST_UP:
        if ((tick_cur - tick_prev) < 15)
            break;
        if ((tick_cur - tick_prev) < 25)
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                cur_state = ACTION_FIRST_MIDDLE;
                tick_prev = tick_cur;
                prev_state = ACTION_FIRST_UP;
                printf("\r\n----ACTION_FIRST_UP->ACTION_FIRST_MIDDLE---\r\n");
            }
            else if (qvar_value < JUDGE_DOWN_VALUE)
            {
                cur_state = ACTION_SECOND_DOWN;
                tick_prev = tick_cur;
                prev_state = ACTION_FIRST_UP;
                printf("\r\n----ACTION_FIRST_UP->ACTION_SECOND_DOWN---\r\n");
            }
        }
        else if ((tick_cur - tick_prev) < 35)
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                prev_state = ACTION_FIRST_UP;
                cur_state = ACTION_END_MIDDLE;
                printf("\r\n----ACTION_FIRST_UP->ACTION_END_MIDDLE---\r\n");
            }
        }
        else
        {

            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                /*左长按*/
                cur_state = ACTION_IDLE;
                prev_state = ACTION_IDLE;
                printf("\r\n----ACTION_FIRST_UP->ACTION_IDLE---\r\n");
                return RESULT_LEFT_LONG_PRESSED;
            }
            else
            {
            }
        }

        break;

    case ACTION_FIRST_DOWN:
        if ((tick_cur - tick_prev) < 15)
            break;
        if ((tick_cur - tick_prev) < 25)
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                cur_state = ACTION_FIRST_MIDDLE;
                tick_prev = tick_cur;
                prev_state = ACTION_FIRST_DOWN;
                printf("\r\n----ACTION_FIRST_DOWN->ACTION_FIRST_MIDDLE---\r\n");
            }
            else if (qvar_value > JUDGE_UP_VALUE)
            {
                cur_state = ACTION_SECOND_UP;
                tick_prev = tick_cur;
                prev_state = ACTION_FIRST_DOWN;
                printf("\r\n----ACTION_FIRST_DOWN->ACTION_SECOND_UP---\r\n");
            }
        }
        else if ((tick_cur - tick_prev) < 35)
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                prev_state = ACTION_FIRST_DOWN;
                cur_state = ACTION_END_MIDDLE;
                printf("\r\n----ACTION_FIRST_DOWN->ACTION_END_MIDDLE---\r\n");
            }
        }
        else
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                /*右长按*/
                cur_state = ACTION_IDLE;
                prev_state = ACTION_IDLE;
                printf("\r\n----ACTION_FIRST_DOWN->ACTION_IDLE---\r\n");
                return RESULT_RIGHT_LONG_PRESSED;

            }
            else
            {
            }
        }
        break;
    case ACTION_FIRST_MIDDLE:
        if ((tick_cur - tick_prev) < 15)
            break;
        if ((tick_cur - tick_prev) < 25)
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                cur_state = ACTION_IDLE;
                prev_state = ACTION_IDLE;
                printf("\r\n----ACTION_FIRST_DOWN->ACTION_IDLE---\r\n");
            }
            else if (qvar_value > JUDGE_UP_VALUE)
            {
                cur_state = ACTION_SECOND_UP;
                tick_prev = tick_cur;
                prev_state = ACTION_FIRST_MIDDLE;
                printf("\r\n----ACTION_FIRST_DOWN->ACTION_SECOND_UP---\r\n");
            }
            else if (qvar_value < JUDGE_DOWN_VALUE)
            {
                cur_state = ACTION_SECOND_DOWN;
                tick_prev = tick_cur;
                prev_state = ACTION_FIRST_MIDDLE;
                printf("\r\n----ACTION_FIRST_DOWN->ACTION_SECOND_DOWN---\r\n");
            }
        }
        else
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                cur_state = ACTION_IDLE;
                prev_state = ACTION_IDLE;
                printf("\r\n----ACTION_FIRST_DOWN->ACTION_IDLE---\r\n");
            }
        }

        break;
    case ACTION_SECOND_UP:
        if ((tick_cur - tick_prev) < 15)
            break;
        if ((tick_cur - tick_prev) < 25)
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {

                if (ACTION_FIRST_MIDDLE == prev_state)
                {
                	 printf("\r\n----RESULT_LEFT_DOUBLE_CLICK---\r\n");
                    cur_state = ACTION_IDLE;
                    /*左双击判断成功*/
                    return RESULT_LEFT_DOUBLE_CLICK;
                }
                else if (ACTION_FIRST_DOWN == prev_state)
                {
                	printf("\r\n----RESULT_LEFT_SLIP---\r\n");
                    cur_state = ACTION_IDLE;
                    /*左滑判断成功*/
                    return RESULT_LEFT_SLIP;
                }
            }
        }
        else
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                cur_state = ACTION_IDLE;
            }
        }

        break;
    case ACTION_SECOND_DOWN:
        if ((tick_cur - tick_prev) < 15)
            break;
        if ((tick_cur - tick_prev) < 25)
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {

                if (ACTION_FIRST_MIDDLE == prev_state)
                {
                	printf("\r\n----RESULT_LEFT_DOUBLE_CLICK---\r\n");
                    cur_state = ACTION_IDLE;
                    /*右双击判断成功*/
                    return RESULT_LEFT_DOUBLE_CLICK;
                }
                else if (ACTION_FIRST_UP == prev_state)
                {
                	printf("\r\n----RESULT_LEFT_SLIP---\r\n");
                    cur_state = ACTION_IDLE;
                    /*右滑判断成功*/
                    return RESULT_LEFT_SLIP;
                }
            }
        }
        else
        {
            if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
            {
                cur_state = ACTION_IDLE;
            }
        }

        break;

    case ACTION_END_MIDDLE:
        if ((qvar_value < JUDGE_MIDDLE_UP) && (qvar_value > JUDGE_MIDDLE_DOWN))
        {
            if (ACTION_FIRST_UP == prev_state)
            {
            	printf("\r\n----RESULT_LEFT_SINGEL_CLICK---\r\n");
                cur_state = ACTION_IDLE;
                /*左单击判断成功*/
                return RESULT_LEFT_SINGEL_CLICK;
            }
            else if (ACTION_FIRST_DOWN == prev_state)
            {
            	printf("\r\n----RESULT_RIGHT_SINGEL_CLICK---\r\n");
                cur_state = ACTION_IDLE;
                /*右单击成功*/
                return RESULT_RIGHT_SINGEL_CLICK;
            }
        }
        else
        {
            cur_state = ACTION_IDLE;
        }

        break;
    default:
        break;
    }

    return 0;
}
