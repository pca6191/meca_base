#include "motor.h"
#include "stm32f4xx_tim.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/

void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 		
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);// 
 	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC , ENABLE);  //ʹ��GPIO����ʱ��ʹ��
	
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource6,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource7,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource8,GPIO_AF_TIM8);
	GPIO_PinAFConfig(GPIOC,GPIO_PinSource9,GPIO_AF_TIM8);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_8|GPIO_Pin_9; //GPIOC6.7.8.9
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  			//�������
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//���츴��
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//�ٶ�100MHZ
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//��ʼ��
	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
//	TIM_OCInitStructure.TIM_Pulse = 0;                            //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;     //�������:TIM����Ƚϼ��Ը�
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

    TIM_CtrlPWMOutputs(TIM8,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��	
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��		
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);  //CH4Ԥװ��ʹ��	 
	
	TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM8, ENABLE);  //ʹ��TIM
 
} 

void MiniBalance_Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOC, ENABLE); //ʹ�ܶ˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;    //GPIOB0.1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;           //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;      //����100MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);           	  //��ʼ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;    //GPIOB0.1
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;           //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;      //����100MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);           	  //��ʼ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;    //GPIOC4.5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;           //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;      //����100MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);           	  //��ʼ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;    //GPIOC4.5
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;           //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //���
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;      //����100MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOC,&GPIO_InitStructure);           	  //��ʼ��
	
	GPIOB->BSRRL = GPIO_Pin_0;
	GPIOB->BSRRL = GPIO_Pin_1;
	GPIOC->BSRRL = GPIO_Pin_4;
	GPIOC->BSRRL = GPIO_Pin_5;

}





