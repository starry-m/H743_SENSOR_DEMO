# 基于STM32H743+SENSOR板的可视化感知交互系统


板载传感器：
LSM6DSO16IS：MEMS 3D加速度计  + 3D陀螺仪 与ISPU
LIS2MDL：MEMS 3D磁力计
LIS2DUXS12：超低功耗MEMS 3轴加速度计
LPS22DF:低功率和高精度MEMS绝对数字输出气压计
SHT40AD1B：高精度超低功耗的温湿度传感器
STTS22H：低电压，超低功耗，0.5°C精度的温度传感器
LSM6DSV16X：MEMS 3D加速度计+ 3D陀螺仪

|Motion|ID|ACCELEROMETER|GYROSCOPE|MAGNETOMETER|
|-|-|-|-|-|
|LIS2MDL|0|&#10006;|&#10006;| &#10004;|
|LSM6DSV16X|1|&#10004;|&#10004;|&#10006;|
|LIS2DUXS12|2|&#10004;|&#10006;|&#10006;|
|LSM6DSO16IS|3|&#10004;|&#10004;|&#10006;|


|Environmental|ID|TEMPERATURE|PRESSURE|HUMIDITY|
|-|-|-|-|-|
|STTS22H|0|&#10004;|&#10006;|&#10006;|
|LPS22DF|1|&#10004;|&#10004;|&#10006;|
|SHT40AD1B|2|&#10004;|&#10006;|&#10004;|



`QVAR 动作分析`
|动作||||
|-|-|-|-|
|||||

$\Delta$t大概为30ms

`左单击`:0->  1  ->0
`右单击`:0-> -1 ->0

$\Delta$t大概为20ms

`左双击`:0->1->0->1->0
`右双击`:0->-1->0->-1->0

$\Delta$t大概为20ms
`右滑`:0->1->-1->0
`左滑`:0->-1->1->0