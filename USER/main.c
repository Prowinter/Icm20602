#include "stm32f4xx.h"
#include "usart.h"
#include "delay.h"
#include "spi.h"
#include "icm20602.h"

extern int16_t icm_gyro_x,icm_gyro_y,icm_gyro_z;
extern int16_t icm_acc_x,icm_acc_y,icm_acc_z;
// 该程序 ICM20602 使用SPI2 PB12-15
int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置系统中断优先级分组2
	uart_init(115200);
	delay_init(168);
	printf("SPI2初始化!\r\n");
	SPI2_Init();
	printf("ICM20602初始化！\r\n");
	icm20602_init_spi();
  while(1){
    get_icm20602_accdata_spi();
		get_icm20602_gyro_spi();
		printf("icm_gyro_x = %d,icm_gyro_y = %d,icm_gyro_z = %d\r\n",icm_gyro_x,icm_gyro_y,icm_gyro_z);
		printf("icm_acc_x = %d,icm_acc_y = %d,icm_acc_z = %d\r\n",icm_acc_x,icm_acc_y,icm_acc_z);
		delay_ms(1000);
	}
}

