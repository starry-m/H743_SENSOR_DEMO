/*
 * data_pack.c
 *
 *  Created on: Jun 10, 2024
 *      Author: pc
 */
#include "data_pack.h"

#include "main.h"
#include <stdio.h>

#include "iks4a1_env_sensors.h"
#include "iks4a1_motion_sensors.h"
#include "math.h"
#include "mems_control.h"
#include "stm32h7xx_nucleo.h"

typedef struct
{
    uint8_t head;
    uint8_t tail;
    uint8_t length;
    uint8_t cmd;
    uint8_t data[100];
    uint8_t crc_H;
    uint8_t crc_L;
} data_frame_t;

// typedef struct
// {
//     int8_t sign; /* 0 means positive, 1 means negative*/
//     uint32_t out_int;
//     uint32_t out_dec;
// } sensorInt_t;
// typedef struct
// {
//     int32_t LIS2MDL_Magneto[3];
//     int32_t LSM6DSV16X_Acc[3];
//     int32_t LSM6DSV16X_Gyro[3];
//     int32_t LIS2DUXS12_Acc[3];
//     int32_t LSM6DSO16IS_Acc[3];
//     int32_t LSM6DSO16IS_Gyro[3];
//     float STTS22H_temperature;
//     float LPS22DF_temperature;
//     float LPS22DF_pressure;
//     float SHT40AD1B_temperature;
//     float SHT40AD1B_humidity;
// } motion_and_sensor_info_t;
static data_frame_t send_dataFrame;

// static motion_and_sensor_info_t all_sensor_info;

static uint8_t data_pack_init(uint8_t cmd_s)
{
    send_dataFrame.head = 0xAA;
    send_dataFrame.tail = 0x55;
    send_dataFrame.cmd = cmd_s;
    send_dataFrame.length = 92;
    send_dataFrame.crc_H = 0;
    send_dataFrame.crc_L = 0;
    return 0;
}

void u32_transfer_u8(int32_t d_in, uint8_t *buff)
{
    buff[0] = 0xff & (d_in >> 24);
    buff[1] = 0xff & (d_in >> 16);
    buff[2] = 0xff & (d_in >> 8);
    buff[3] = 0xff & d_in;
}

static void motion_data_buff_fit(int32_t *data1_in, int32_t *data2_in, int32_t *data3_in, int32_t *data4_in,
                                 int32_t *data5_in, int32_t *data6_in)
{
    uint16_t index = 0, index2 = 0;
    uint8_t *p_data = &(send_dataFrame.data[index2]);

    for (index = 0; index < 3; index++)
    {
        u32_transfer_u8(data1_in[index], p_data + index * 4);
    }
    index2 += 12;
    p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 3; index++)
    {
        u32_transfer_u8(data2_in[index], p_data + index * 4);
    }
    index2 += 12;
    p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 3; index++)
    {
        u32_transfer_u8(data3_in[index], p_data + index * 4);
    }
    index2 += 12;
    p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 3; index++)
    {
        u32_transfer_u8(data4_in[index], p_data + index * 4);
    }
    index2 += 12;
    p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 3; index++)
    {
        u32_transfer_u8(data5_in[index], p_data + index * 4);
    }
    index2 += 12;
    p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 3; index++)
    {
        u32_transfer_u8(data6_in[index], p_data + index * 4);
    }
}


static void env_data_buff_fit(float data1_in, float data2_in, float data3_in, float data4_in, float data5_in)
{
    uint16_t index = 0, index2 = 72;
    __IO UserFtoCtoI temp_fdata;
    temp_fdata.fdata = data1_in;
    uint8_t *p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 4; index++)
    {
        p_data[index] = temp_fdata.ldata[index];
    }
    index2 += 4;
    temp_fdata.fdata = data2_in;
    p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 4; index++)
    {
        p_data[index] = temp_fdata.ldata[index];
    }
    index2 += 4;
    temp_fdata.fdata = data3_in;
    p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 4; index++)
    {
        p_data[index] = temp_fdata.ldata[index];
    }
    index2 += 4;
    temp_fdata.fdata = data4_in;
    p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 4; index++)
    {
        p_data[index] = temp_fdata.ldata[index];
    }
    index2 += 4;
    temp_fdata.fdata = data5_in;
    p_data = &(send_dataFrame.data[index2]);
    for (index = 0; index < 4; index++)
    {
        p_data[index] = temp_fdata.ldata[index];
    }
}
/*
    Little - Endian Modemessage：data pointer
    len ：data length
    return ：16bits CRC Value
*/
static uint16_t GetCRC(uint8_t *message, uint16_t len)
{
    uint32_t CRCFull = 0xFFFF;
    uint8_t CRCLSB;
    uint8_t i = 0, j = 0;
    uint8_t *mess = message;
    for (i = 0; i < len; i++)
    {
        CRCFull = (uint16_t)(CRCFull ^ mess[i]);
        for (j = 0; j < 8; j++)
        {
            CRCLSB = (uint8_t)(CRCFull & 0x0001);
            CRCFull = (uint16_t)((CRCFull >> 1) & 0x7FFF);
            if (CRCLSB == 1)
                CRCFull = (uint16_t)(CRCFull ^ 0xA001);
        }
    }
    return CRCFull;
}

uint8_t sensor_data_pack(uint8_t cmd, uint8_t *sendbuff, int32_t *data1_in, int32_t *data2_in, int32_t *data3_in,
                         int32_t *data4_in, int32_t *data5_in, int32_t *data6_in, float fdata1_in, float fdata2_in,
                         float fdata3_in, float fdata4_in, float fdata5_in)
{
    uint8_t i = 0;
    uint16_t crc_val = 0;
//    uint8_t send_length = 0;
    data_pack_init(cmd);
    motion_data_buff_fit(data1_in, data2_in, data3_in, data4_in, data5_in, data6_in);
    env_data_buff_fit(fdata1_in, fdata2_in, fdata3_in, fdata4_in, fdata5_in);
    crc_val = GetCRC(send_dataFrame.data, send_dataFrame.length);

    sendbuff[0] = send_dataFrame.head;
    sendbuff[1] = send_dataFrame.cmd;
    sendbuff[2] = send_dataFrame.length;

    for (i = 3; i < send_dataFrame.length + 3; i++)
    {
        sendbuff[i] = send_dataFrame.data[i - 3];
    }
    sendbuff[i++] = crc_val >> 8;
    sendbuff[i++] = crc_val & 0xFF;
    sendbuff[i++] = send_dataFrame.tail;

    return i;
}
