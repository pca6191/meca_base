#include "led.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
/**************************************************************************
�������ܣ�LED�ӿڳ�ʼ��
��ڲ������� 
����  ֵ����
**************************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); //ʹ�ܶ˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_13;              //GPIO.B13
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;           //���ģʽ
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;          //�������
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;       //����50MHZ
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;            //����
	GPIO_Init(GPIOB,&GPIO_InitStructure);           	  //��ʼ��
}

/**************************************************************************
�������ܣ�LED��˸
��ڲ�������˸Ƶ�� 
����  ֵ����
**************************************************************************/
void Led_Flash(u16 time)
{
	  static int temp;
	  if(0==time) LED=0;
	  else		if(++temp==time)	LED=~LED,temp=0;
}