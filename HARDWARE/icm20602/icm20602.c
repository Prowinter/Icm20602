#include "icm20602.h"
#include "spi.h"
#include "delay.h"
#include "usart.h"

uint16_t icm_gyro_x,icm_gyro_y,icm_gyro_z;
uint16_t icm_acc_x,icm_acc_y,icm_acc_z;

void icm20602_init_spi(void)
{
		uint8_t val = 0x0;

    delay_ms(10);  //上电延时


    icm20602_self3_check();//检测

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,0x80);//复位设备
    delay_ms(2);
    do
    {   //等待复位成功
        icm_spi_r_reg_byte(ICM20602_PWR_MGMT_1,&val);
    } while(0x41 != val);

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,     0x01);            //时钟设置
    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_2,     0x00);            //开启陀螺仪和加速度计
    icm_spi_w_reg_byte(ICM20602_CONFIG,         0x05);            //20HZ 1KHZ
    icm_spi_w_reg_byte(ICM20602_SMPLRT_DIV,     0x00);            //采样速率 SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm_spi_w_reg_byte(ICM20602_GYRO_CONFIG,    0x18);            //±2000 dps
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG,   0x18);            //±16g
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG_2, 0x03);            //Average 4 samples   44.8HZ   //0x23 Average 16 samples
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602自检函数
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

void icm20602_self3_check(void)
{
    uint8_t dat=0;
    while(0x12 != dat)   //读取ICM20602 ID
    {
        icm_spi_r_reg_byte(ICM20602_WHO_AM_I,&dat);
        delay_ms(10);
				printf("自检程序启动");
        //卡在这里原因有以下几点
        //1 MPU6050坏了，如果是新的这样的概率极低
        //2 接线错误或者没有接好
        //3 可能你需要外接上拉电阻，上拉到3.3V
    }
		printf("dat=%d\r\n",dat);

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI写寄存器
//  @param      cmd     寄存器地址
//  @param      val     需要写入的数据
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_spi_w_reg_byte(uint8_t cmd, uint8_t val)
{
		ICM20602_CS = ENABLE_ICM20602;    //片选SPI2
		SPI2_ReadWriteByte(cmd | ICM20602_SPI_W);
    SPI2_ReadWriteByte(val);
		ICM20602_CS = DISABLE_ICM20602;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI读寄存器
//  @param      cmd     寄存器地址
//  @param      *val    接收数据的地址
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

void icm_spi_r_reg_byte(uint8_t cmd, uint8_t *val)
{
		ICM20602_CS = ENABLE_ICM20602;   //片选SPI2
    SPI2_ReadWriteByte(cmd | ICM20602_SPI_R);
		*val = SPI2_ReadWriteByte(ICM20602_SPI_R);
		ICM20602_CS = DISABLE_ICM20602;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI多字节读寄存器
//  @param      cmd     寄存器地址
//  @param      *val    接收数据的地址
//  @param      num     读取数量
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_spi_r_reg_bytes(uint8_t cmd ,uint16_t * val, uint8_t num)
{
		int i=0;
		ICM20602_CS = ENABLE_ICM20602;   //片选SPI2
		SPI2_ReadWriteByte(cmd);
		while(i<num)
		{
			*(val++) = SPI2_ReadWriteByte(ICM20602_SPI_R);
			i++;
		}
		ICM20602_CS = DISABLE_ICM20602;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602加速度计数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_icm20602_accdata_spi(void)
{
		uint16_t dat[6];
    icm_spi_r_reg_bytes(ICM20602_ACCEL_XOUT_H | ICM20602_SPI_R,dat, 6);
    icm_acc_x = ((uint16_t)dat[0]<<8 | dat[1]);
    icm_acc_y = ((uint16_t)dat[2]<<8 | dat[3]);
    icm_acc_z = ((uint16_t)dat[4]<<8 | dat[5]);
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      获取ICM20602陀螺仪数据
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				执行该函数后，直接查看对应的变量即可
//-------------------------------------------------------------------------------------------------------------------
void get_icm20602_gyro_spi(void)
{
		uint16_t dat[6];
		icm_spi_r_reg_bytes(ICM20602_GYRO_XOUT_H | ICM20602_SPI_R,dat, 6);
    icm_gyro_x = ((uint16_t)dat[0]<<8 | dat[1]);
    icm_gyro_y = ((uint16_t)dat[2]<<8 | dat[3]);
    icm_gyro_z = ((uint16_t)dat[4]<<8 | dat[5]);
}




