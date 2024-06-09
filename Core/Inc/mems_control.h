/*
 * mems_control.h
 *
 *  Created on: Jun 9, 2024
 *      Author: pc
 */

#ifndef INC_MEMS_CONTROL_H_
#define INC_MEMS_CONTROL_H_

#include "iks4a1_env_sensors.h"
#include "iks4a1_motion_sensors.h"
#include "iks4a1_motion_sensors_ex.h"
// #include "sensor_unicleo_id.h"

uint8_t QVAR_action_check_statemachine(const int16_t qvar_value);
void BSP_SENSOR_QVAR_GetValue(int16_t *Axes);

#endif /* INC_MEMS_CONTROL_H_ */
