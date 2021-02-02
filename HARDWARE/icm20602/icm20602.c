#include "icm20602.h"
#include "spi.h"
#include "delay.h"
#include "usart.h"

uint16_t icm_gyro_x,icm_gyro_y,icm_gyro_z;
uint16_t icm_acc_x,icm_acc_y,icm_acc_z;

void icm20602_init_spi(void)
{
		uint8_t val = 0x0;

    delay_ms(10);  //�ϵ���ʱ


    icm20602_self3_check();//���

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,0x80);//��λ�豸
    delay_ms(2);
    do
    {   //�ȴ���λ�ɹ�
        icm_spi_r_reg_byte(ICM20602_PWR_MGMT_1,&val);
    } while(0x41 != val);

    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_1,     0x01);            //ʱ������
    icm_spi_w_reg_byte(ICM20602_PWR_MGMT_2,     0x00);            //���������Ǻͼ��ٶȼ�
    icm_spi_w_reg_byte(ICM20602_CONFIG,         0x05);            //20HZ 1KHZ
    icm_spi_w_reg_byte(ICM20602_SMPLRT_DIV,     0x00);            //�������� SAMPLE_RATE = INTERNAL_SAMPLE_RATE / (1 + SMPLRT_DIV)
    icm_spi_w_reg_byte(ICM20602_GYRO_CONFIG,    0x18);            //��2000 dps
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG,   0x18);            //��16g
    icm_spi_w_reg_byte(ICM20602_ACCEL_CONFIG_2, 0x03);            //Average 4 samples   44.8HZ   //0x23 Average 16 samples
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602�Լ캯��
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

void icm20602_self3_check(void)
{
    uint8_t dat=0;
    while(0x12 != dat)   //��ȡICM20602 ID
    {
        icm_spi_r_reg_byte(ICM20602_WHO_AM_I,&dat);
        delay_ms(10);
				printf("�Լ��������");
        //��������ԭ�������¼���
        //1 MPU6050���ˣ�������µ������ĸ��ʼ���
        //2 ���ߴ������û�нӺ�
        //3 ��������Ҫ����������裬������3.3V
    }
		printf("dat=%d\r\n",dat);

}


//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPIд�Ĵ���
//  @param      cmd     �Ĵ�����ַ
//  @param      val     ��Ҫд�������
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_spi_w_reg_byte(uint8_t cmd, uint8_t val)
{
		ICM20602_CS = ENABLE_ICM20602;    //ƬѡSPI2
		SPI2_ReadWriteByte(cmd | ICM20602_SPI_W);
    SPI2_ReadWriteByte(val);
		ICM20602_CS = DISABLE_ICM20602;

}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI���Ĵ���
//  @param      cmd     �Ĵ�����ַ
//  @param      *val    �������ݵĵ�ַ
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------

void icm_spi_r_reg_byte(uint8_t cmd, uint8_t *val)
{
		ICM20602_CS = ENABLE_ICM20602;   //ƬѡSPI2
    SPI2_ReadWriteByte(cmd | ICM20602_SPI_R);
		*val = SPI2_ReadWriteByte(ICM20602_SPI_R);
		ICM20602_CS = DISABLE_ICM20602;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ICM20602 SPI���ֽڶ��Ĵ���
//  @param      cmd     �Ĵ�����ַ
//  @param      *val    �������ݵĵ�ַ
//  @param      num     ��ȡ����
//  @return     void
//  @since      v1.0
//  Sample usage:
//-------------------------------------------------------------------------------------------------------------------
void icm_spi_r_reg_bytes(uint8_t cmd ,uint16_t * val, uint8_t num)
{
		int i=0;
		ICM20602_CS = ENABLE_ICM20602;   //ƬѡSPI2
		SPI2_ReadWriteByte(cmd);
		while(i<num)
		{
			*(val++) = SPI2_ReadWriteByte(ICM20602_SPI_R);
			i++;
		}
		ICM20602_CS = DISABLE_ICM20602;
}

//-------------------------------------------------------------------------------------------------------------------
//  @brief      ��ȡICM20602���ٶȼ�����
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
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
//  @brief      ��ȡICM20602����������
//  @param      NULL
//  @return     void
//  @since      v1.0
//  Sample usage:				ִ�иú�����ֱ�Ӳ鿴��Ӧ�ı�������
//-------------------------------------------------------------------------------------------------------------------
void get_icm20602_gyro_spi(void)
{
		uint16_t dat[6];
		icm_spi_r_reg_bytes(ICM20602_GYRO_XOUT_H | ICM20602_SPI_R,dat, 6);
    icm_gyro_x = ((uint16_t)dat[0]<<8 | dat[1]);
    icm_gyro_y = ((uint16_t)dat[2]<<8 | dat[3]);
    icm_gyro_z = ((uint16_t)dat[4]<<8 | dat[5]);
}




