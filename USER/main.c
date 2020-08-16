#include "sys.h"
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
u8 Flag_Left,Flag_Right,Flag_Direction=0;   //����ң����صı���
u8 Flag_Stop=1,Flag_Show=0;                 //ֹͣ��־λ�� ��ʾ��־λ Ĭ��ֹͣ ��ʾ��
int Encoder_A,Encoder_B,Encoder_C,Encoder_D;          //���������������
long int Position_A,Position_B,Position_C,Position_D,Rate_A,Rate_B,Rate_C,Rate_D; //PID������ر���
long int Motor_A,Motor_B,Motor_C,Motor_D;        //���PWM����
long int Target_A,Target_B,Target_C,Target_D;     //���Ŀ��ֵ
int Voltage;                             //��ص�ѹ������صı���
float Show_Data_Mb;                      //ȫ����ʾ������������ʾ��Ҫ�鿴������                         
u8 delay_50,delay_flag;                          //��ʱ��ر���
u8 Run_Flag=0;  												//����ң����ر���������״̬��־λ
u8 PS2_ON_Flag=0,Flash_Send;  //CAN�ʹ��ڿ�����ر���
u8 Turn_Flag;             //CAN������ر���
float Move_X,Move_Y,Move_Z;   //����ǶȺ�XYZ��Ŀ���ٶ�
u16 PID_Parameter[10],Flash_Parameter[10];  //Flash�������
float	Position_KP=40,Position_KI=0,Position_KD=40;  //λ�ÿ���PID����
float Velocity_KP=10,Velocity_KI=10;	          //�ٶȿ���PID����
int RC_Velocity=30,RC_Position=3000;         //����ң�ص��ٶȺ�λ��ֵ

#if 1  // KC_test start
int main(void)
{
	delay_init(168);
	LED_Init_407();
	
	while(1) {
		delay_ms(300);
		Led_toggle_407();
	}
}
#else
int main(void)
  { 
	  
		delay_init(168);	    	    //=====��ʱ������ʼ��
	  	uart1_init(128000);             //=====����1��ʼ��
		usart2_init(9600);              //=====����2��ʼ��
		usart3_init(115200);            //=====����3��ʼ�� 
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//=====�����жϷ���	

		KEY_Init();                     //=====������ʼ��
		LED_Init();                     //=====��ʼ���� LED ���ӵ�Ӳ���ӿ�
	  	MiniBalance_PWM_Init(8399,1);  //=====��ʼ��PWM 10KHZ�������������
		MiniBalance_Motor_Init(); 		//=====��ʼIO�������������

	  	Encoder_Init_TIM2();            //=====�������ӿ�
		Encoder_Init_TIM3();            //=====�������ӿ�
		Encoder_Init_TIM4();            //=====��ʼ��������C
		Encoder_Init_TIM5();            //=====��ʼ��������D
		if(MODE==0)Run_Flag=1;          //=====�����Ĺ����У�����ģʽѡ�񿪹�ȷ������λ��ģʽ�����ٶ�ģʽ
		else Run_Flag=0;                //=====�����Ĺ����У�����ģʽѡ�񿪹�ȷ������λ��ģʽ�����ٶ�ģʽ
		OLED_Init();                    //=====OLED��ʼ��
		Adc_Init();                     //=====adc��ʼ��
		delay_ms(500);
		IIC_Init();                     //=====IIC��ʼ��
		MPU6050_initialize();           //=====MPU6050��ʼ��	
		DMP_Init();                     //=====��ʼ��DMP     
		PS2_Init();						//=====ps2�����˿ڳ�ʼ��
		PS2_SetInit();		 			//=====ps2���ó�ʼ��,���á����̵�ģʽ������ѡ���Ƿ�����޸�
		MiniBalance_EXTI_Init();        //=====MPU6050 5ms��ʱ�жϳ�ʼ��
		CAN1_Mode_Init(1,7,6,3,CAN_Mode_Normal);      //=====CAN��ʼ��   ĩλ0=��ͨģʽ��1=�ػ�ģʽ
		delay_ms(500);
    while(1)
	   {
		   
	    if(Flag_Show==0)           //ʹ��MiniBalance APP��OLED��ʾ��
			{
				APP_Show();	              
				oled_show();             //===��ʾ����
	    }
    	else                       //ʹ��MiniBalance��λ�� ��λ��ʹ�õ�ʱ����Ҫ�ϸ��ʱ�򣬹ʴ�ʱ�ر�app��ز��ֺ�OLED��ʾ��
  		{
				DataScope();             //����MiniBalance��λ��
			}
			CAN1_SEND();             //CAN����		
			USART3_TX();             //���ڷ���						
			PS2_LX=PS2_AnologData(PSS_LX);
			PS2_LY=PS2_AnologData(PSS_LY);
			PS2_RX=PS2_AnologData(PSS_RX);
			PS2_RY=PS2_AnologData(PSS_RY);
			PS2_KEY=PS2_DataKey();
			delay_flag=1;	
			delay_50=0;
			while(delay_flag);	       //ͨ��MPU6050��INT�ж�ʵ�ֵ�50ms��׼��ʱ				
	  } 
}
#endif
// KC_TEST end
