#ifndef __LED_H
#define __LED_H
#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
//LED �˿ڶ���
#define LED PBout(13)  //LED��������
void LED_Init(void);  //��ʼ��
void Led_Flash(u16 time);
#endif