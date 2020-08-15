#include "usartx.h"
#include "sys.h"
  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
u8 Usart_ON_Flag=0,Usart_Flag;//串口控制标志位
u8 Uart3_Receive;	//串口接收数组
u8 Urxbuf[8];//串口接收数组

/**************************实现函数**********************************************
*功    能:		usart发送一个字节
*********************************************************************************/
void usart3_send(u8 data)
{
	USART3->DR = data;
	while((USART2->SR&0x40)==0);	
}


/**************************************************************************
函数功能：串口3初始化
入口参数： bound:波特率
返回  值：无
**************************************************************************/
void usart3_init(u32 bound)
{  	 
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);//使能GPIO时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);//使能USART时钟

	GPIO_PinAFConfig(GPIOC,GPIO_PinSource10,GPIO_AF_USART3);	 //GPIO.C10复用为串口3
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource11 ,GPIO_AF_USART3);	 //GPIO.C11复用为串口3
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_11;//PC10.11
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AF;            //输出模式
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //推挽输出
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;       //高速50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //上拉
	GPIO_Init(GPIOC, &GPIO_InitStructure);  		      //初始化
	

	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0 ;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//子优先级
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化VIC寄存器

	USART_InitStructure.USART_BaudRate = bound;				//串口波特率
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//字长为8位数据格式
	USART_InitStructure.USART_StopBits = USART_StopBits_1;	//一个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;		//无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//无硬件数据流控制
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//收发模式
	
	USART_Init(USART3, &USART_InitStructure);     			//初始化串口
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);			//开启串口接受中断
	USART_Cmd(USART3, ENABLE);                    			//使能串口
}

/**************************************************************************
函数功能：串口5接收中断
入口参数：无
返回  值：无
**************************************************************************/
void USART3_IRQHandler(void)
{	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET) //接收到数据
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
函数功能：通过串口把自身的传感器发送出去
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
