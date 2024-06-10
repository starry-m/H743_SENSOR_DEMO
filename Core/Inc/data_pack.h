/*
 * data_pack.h
 *
 *  Created on: Jun 10, 2024
 *      Author: pc
 */

#ifndef INC_DATA_PACK_H_
#define INC_DATA_PACK_H_

#include "stdint.h"

uint8_t sensor_data_pack(uint8_t cmd, uint8_t *sendbuff, int32_t *data1_in, int32_t *data2_in, int32_t *data3_in,
                         int32_t *data4_in, int32_t *data5_in, int32_t *data6_in, float fdata1_in, float fdata2_in,
                         float fdata3_in, float fdata4_in, float fdata5_in);

#endif /* INC_DATA_PACK_H_ */
