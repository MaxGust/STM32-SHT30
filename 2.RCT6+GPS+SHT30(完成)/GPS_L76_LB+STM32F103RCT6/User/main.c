#include "stm32f10x.h"
#include "usart.h"
#include "delay.h"
#include "L76X.h"
#include "SHT30.h"

int main(void)
{

	int t[20];
	u16 h[20];
//	NVIC_Configuration();
	delay_init();
//	Usart3_init(115200);
	DebugUsart_init(115200);
	SHT30_Init();
	
	printf("��ʼ\n");
	
	while(1)
	{
		SHT30_Read_Humiture(t,h);
		printf("��ʪ��Ϊ%f,%f\n",t[0]/10.0,h[0]/10.0);
		delay_ms(1000);
		delay_ms(1000);
		
	}
	
	
#if 0
	GNRMC_number GPS;
	Coordinates GPS_Baidu;
	
	printf("��ʼ\n");
	delay_ms(100);
	
	//���ô�΢Ц�ɼ���Ϣ���Ϊ4s
	L76X_Send_Command(SET_POS_FIX_4S);
	delay_ms(1000);


	//��η��ͣ���ֹû�յ�
	L76X_Send_Command(SET_NMEA_OUTPUT);
	delay_ms(1000);
	L76X_Send_Command(SET_NMEA_OUTPUT);
	delay_ms(1000);

	L76X_Send_Command(SET_NMEA_OUTPUT);
	delay_ms(1000);
	L76X_Send_Command(SET_NMEA_OUTPUT);
	delay_ms(1000);


	
	while(1)
	{
		GPS=L76X_Gat_GNRMC();
		printf("ԭʼ��γ��Ϊ%lf,%lf\n",GPS.Lon,GPS.Lat);
		delay_ms(1000);	
		
		GPS_Baidu = L76X_Baidu_Coordinates();
		printf("ת��Ϊ�ٶȾ�γ��\n%.10lf,%.10lf\n",GPS_Baidu.Lon,GPS_Baidu.Lat);
		
		delay_ms(1000);	

	}
#endif	
	
}
