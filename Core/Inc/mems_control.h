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
typedef enum{
	LIS2MDL,
	LSM6DSV16X,
	LIS2DUXS12,
	LSM6DSO16IS,
	STTS22H,
	LPS22DF,
	SHT40AD1B

}Sensor_Type;
typedef struct __sensor_channel_status{
	uint32_t LIS2MDL_MAG_X : 1;
	uint32_t LIS2MDL_MAG_Y : 1;
	uint32_t LIS2MDL_MAG_Z : 1;

	uint32_t LSM6DSV16X_ACC_X : 1;
	uint32_t LSM6DSV16X_ACC_Y : 1;
	uint32_t LSM6DSV16X_ACC_Z : 1;
	uint32_t LSM6DSV16X_GYR_X : 1;
	uint32_t LSM6DSV16X_GYR_Y : 1;
	uint32_t LSM6DSV16X_GYR_Z : 1;

	uint32_t LIS2DUXS12_ACC_X : 1;
	uint32_t LIS2DUXS12_ACC_Y : 1;
	uint32_t LIS2DUXS12_ACC_Z : 1;

	uint32_t LSM6DSO16IS_ACC_X : 1;
	uint32_t LSM6DSO16IS_ACC_Y : 1;
	uint32_t LSM6DSO16IS_ACC_Z : 1;
	uint32_t LSM6DSO16IS_GYR_X : 1;
	uint32_t LSM6DSO16IS_GYR_Y : 1;
	uint32_t LSM6DSO16IS_GYR_Z : 1;

	uint32_t STTS22H_temp : 1;

	uint32_t LPS22DF_temp : 1;
	uint32_t LPS22DF_press : 1;

	uint32_t SHT40AD1B_temp : 1;
	uint32_t SHT40AD1B_humi : 1;
}sensor_channel_status;
#define RESULT_LEFT_SINGEL_CLICK 1
#define RESULT_RIGHT_SINGEL_CLICK 2
#define RESULT_LEFT_DOUBLE_CLICK 3
#define RESULT_RIGHT_DOUBLE_CLICK 4
#define RESULT_LEFT_SLIP 5
#define RESULT_RIGHT_SLIP 6
#define RESULT_LEFT_LONG_PRESSED 7
#define RESULT_RIGHT_LONG_PRESSED 8

uint8_t QVAR_action_check_statemachine(const int16_t qvar_value);
void BSP_SENSOR_QVAR_GetValue(int16_t *Axes);
void read_all_sensor_data();
uint8_t  get_one_sensor_data(Sensor_Type type,uint8_t *data,sensor_channel_status ch_status);

#endif /* INC_MEMS_CONTROL_H_ */
