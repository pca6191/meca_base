#ifndef __USRATX_H
#define __USRATX_H 
#include "sys.h"	  	
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
void usart3_init(u32 bound);
void USART3_IRQHandler(void);
void USART3_TX(void);
void usart3_send(u8 data);
#endif

