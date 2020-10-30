#include "sys.h"
#include "usart.h"
#include "string.h"

char DebugBufferCounter, 	DebugBuffer[100]; 
char USART3_RXBUF[100];
uint16_t RxCounter3;
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{      
		/* ����һ���ֽ����ݵ����� */
		USART_SendData(USART1, (uint8_t) ch);
		
		/* �ȴ�������� */
		while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);		
	
		return (ch);
}
#endif 

void DebugUsart_init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
	 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
  
		//USART1_TX   GPIOA.9
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9; //PA.9
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.9
		 
		//USART1_RX	  GPIOA.10��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//PA10
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA.10  

		//Usart1 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
		 //USART ��ʼ������

		USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx|USART_Mode_Tx ;	//����ģʽ

		USART_Init(USART1, &USART_InitStructure); //��ʼ������1
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
		USART_Cmd(USART1, ENABLE);                    //ʹ�ܴ���1 
}
void DebugUsart_SendData(u8 Data)
{
		 USART_SendData(USART1,Data);	  //����һ���ֽ�����
		 while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=1);  //�ȴ��������ݼĴ���Ϊ��
}
void DebugUsart_SendStr(char*SendBuf)//����1��ӡ����
{
	while(*SendBuf)
	{
        while((USART1->SR&0X40)==0);//�ȴ�������� 
        USART1->DR = (u8) *SendBuf; 
        SendBuf++;
	}
}

void USART1_IRQHandler(void)                	//����1��Debug���жϷ������
{
		if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
			DebugBuffer[DebugBufferCounter++]=USART_ReceiveData(USART1);		
    } 
} 

void Usart3_init(u32 bound)
{
		GPIO_InitTypeDef GPIO_InitStructure;
		USART_InitTypeDef USART_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;
		 
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��USART1��GPIOAʱ��
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART3ʱ��
		
		//USART3_TX   GPIOB.10
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10; //PB10
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.10
		 
		//USART3_RX	  GPIOB.11��ʼ��
		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;//PB11
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
		GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIOB.11

		//Usart3 NVIC ����
		NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3 ;//��ռ���ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;		//�����ȼ�3
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
		NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
		
		//USART ��ʼ������
		USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
		USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
		USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
		USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
		USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
		USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

		USART_Init(USART3, &USART_InitStructure); //��ʼ������3
		USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);//�������ڽ����ж�
		USART_Cmd(USART3, ENABLE);
}


void Usart3_SendData(u8 Data)
{
		 USART_SendData(USART3,Data);	  //����һ���ֽ�����
		 while(USART_GetFlagStatus(USART3,USART_FLAG_TC)!=1);  //�ȴ��������ݼĴ���Ϊ��
}
void Usart3_SendNByte(u8 *Data,u8 length)
{
		u8 i;
    for(i=0;i<length;i++)
    {
        USART_SendData(USART3, Data[i]);
			  //Usart3_SendData(Data[i]);
        while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);//?????? 
    }
}
void CleanRxBuf3(void)
{
		memset(USART3_RXBUF,0,100);
		RxCounter3 = 0;
		
}
void CleanDebugBuffer(void)
{
		memset(DebugBuffer,0,100);
		DebugBufferCounter = 0;
		
}




//��֤�������ԡ�$����ͷ
void USART3_IRQHandler(void)                	//����3�жϷ������
{
		uint16_t res;
		
		if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{	
			/*
			ͨ������ʹÿ�������Ķ���һ����Ч������ÿ�δ�Ĳ�һ��������
			���ʹ����USART1�������ж��������ݿ��ܻᵼ�£������Ƶ�ʺʹ����Ƶ�ʲ�һ�¶����¶�����
			�����ӿ��Բ������ݣ�����ÿ�ν��ܵ����ݶ�����������      
			*/
				res=USART_ReceiveData(USART3);
				if(res=='$')
				{
					CleanRxBuf3();
				}
				
				USART3_RXBUF[RxCounter3++] = res;
			
		} 
}


