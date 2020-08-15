#include "usartx.h"
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
u8 Usart_ON_Flag=0,Usart_Flag;//���ڿ��Ʊ�־λ
u8 Uart3_Receive;	//���ڽ�������
u8 Urxbuf[8];//���ڽ�������

/**************************ʵ�ֺ���**********************************************
*��    ��:		usart����һ���ֽ�
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART2->SR&0x40)==0);	
}


/**************************************************************************
�������ܣ�����3��ʼ��
��ڲ����� bound:������
����  ֵ����
**************************************************************************/
void usart3_init(u32 bound)
{  	 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//ʹ��GPIOʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//ʹ��USARTʱ��

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);	 //GPIO.C10����Ϊ����3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11 ,GPIO_AF_USART3);	 //GPIO.C11����Ϊ����3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//PC10.11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //����50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOC, &GPIO_InitStructure);  		      //��ʼ��
	

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��VIC�Ĵ���

	USART_InitStructure.USART_BaudRate = bound;				//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;		//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ
	
	USART_Init(USART3, &USART_InitStructure);     			//��ʼ������
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//�������ڽ����ж�
	USART_Cmd(USART3, ENABLE);                    			//ʹ�ܴ���
}

/**************************************************************************
�������ܣ�����5�����ж�
��ڲ�������
����  ֵ����
**************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //���յ�����
	{	  
				u8 temp;
				static u8 count,last_data,last_last_data,Usart_ON_Count;
		    if(Usart_ON_Flag==0)
				{	
		    if(++Usart_ON_Count>10)Usart_ON_Flag=1;
				}
				temp=USART3->DR;
				 if(Usart_Flag==0)
					{	
					if(last_data==0xfe&&last_last_data==0xff) 
					Usart_Flag=1,count=0;	
					}
				 if(Usart_Flag==1)
					{	
						Urxbuf[count]=temp;     
						count++;                
						if(count==8)Usart_Flag=0;
					}
					last_last_data=last_data;
					last_data=temp;
	}  											 
} 


/**************************************************************************
�������ܣ�ͨ�����ڰ�����Ĵ��������ͳ�ȥ
**************************************************************************/
void USART3_TX(void)
{
        u8 Direction_A,Direction_B,Direction_C,Direction_D;
	      u16 Temp_GZ;
	           if(Encoder_A>0) Direction_A=0;
        else if(Encoder_A<0) Direction_A=2;
	      else                 Direction_A=1;
		         if(Encoder_B>0) Direction_B=0;
        else if(Encoder_B<0) Direction_B=2;
	      else                 Direction_B=1;     
		         if(Encoder_C>0) Direction_C=0;
        else if(Encoder_C<0) Direction_C=2;
	      else                 Direction_C=1;
			       if(Encoder_D>0) Direction_D=0;
        else if(Encoder_D<0) Direction_D=2;
	      else                 Direction_D=1;   
      	Temp_GZ=gyro[2]+32768;
			
				usart3_send(0xff);		
				usart3_send(0xfe);	
				usart3_send(abs(Encoder_A));		
				usart3_send(Direction_A);	
				usart3_send(abs(Encoder_B));		
				usart3_send(Direction_B);	
				usart3_send(abs(Encoder_C));		
				usart3_send(Direction_C);	
				usart3_send(abs(Encoder_D));		
				usart3_send(Direction_D);	
				usart3_send(Temp_GZ>>8);		
				usart3_send(Temp_GZ&0x00ff);	
}
