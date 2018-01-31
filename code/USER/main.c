#include "sys.h"
#include "math.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "pwm.h"
#include "hmc5883l.h"
#include "iic.h"
#include "exti.h"
#include "motor_pid.h"
#include "steer_pid.h"
#include "hcsr04.h"
struct STEER_PID  SD5_PID;
extern u8 recive_ble[6]; 
extern u8 recive_flag; 
extern u32 s; 
extern int speed_count;
u8 flag_turn=0;
u8 flag_turn_left=0;
u8 flag_turn_right=0;
u8 flag_turn_big=0;
u8 last_flag_turn;
float offsetX=-59.5;  
float offsetY=-76.5;  
short int data[3];
float angle;
void calibrate()  
{  
	u8 i;
  float xMax, xMin, yMax, yMin;
  xMax=xMin=data[0];  
  yMax=yMin=data[2];  
  for(i=0;i<250;i++)  
  {  
    hmc_read_XYZ(data);
    if (data[0] > xMax)  
      xMax =data[0];  
    if (data[0] < xMin )  
      xMin = data[0];  
    if(data[2] > yMax )  
      yMax = data[2];  
    if(data[2] < yMin )  
      yMin = data[2];  
    delay_ms(100);  
  }  
  offsetX = (xMax + xMin)/2;  
  offsetY = (yMax + yMin)/2;  
}  
int main(void)
{  
	int angle_diff;
	u8 h;
	u8 i=0;
	u8 an_count;
	u8 first_flag=1;
	u8 flag_dis_retreat=0;
  u8 distance_1=64,distance_2=59,distance_3=46;
	u32 distance=0;
	u32 last_distance;
	u32 distance_proceed=0;
  u32 last_distance_proceed;
	u32 dis_data[5];
	int err_angle=0;
	int angle_expected;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//����ϵͳ�ж����ȼ�����2
  STEER_PID_init(&SD5_PID,0.32,0.18,0.05); 
	delay_init(168);  //��ʼ����ʱ���
	uart_init(115200);//��ʼ�����ڲ�����Ϊ115200  A9/A10
	EXTIX_Init(); //�ⲿ�ж�  E2
	BEEP_Init();//������ C8
	hcsr04_init();
	TIM1_Init(20000-1,84-1);// A8  84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ20000
 	TIM3_PWM_Init(20000-1,84-1);	// A6  84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ20000������PWMƵ��Ϊ 1M/20000=50hz. ���50HZ��7%~9%  
	TIM5_PWM_Init(50-1,84-1);//  A0 84M/84=1Mhz�ļ���Ƶ��,��װ��ֵ50������PWMƵ��Ϊ 1M/50=20000hz. ��� 20000hz  5%
	TIM4_PWM_Init();//������������� e0
	TIM2_Init(10000-1,84-1);//ÿ10ms����������
  TIM_SetCompare1(TIM3,18400);	//�޸ıȽ�ֵ���޸�ռ�ձ�	0~19999 ռ�ձ�=(19999-x)/19999 
	i2c_init();  //ָ���� b6b7
  hmc_init();
  //printf("�Ƕ�\n");
	//calibrate();
	while(1)
	{

	  hmc_read_XYZ(data);
		data[0]=(float)data[0]-offsetX;
    data[2]=(float)data[2]-offsetY;
		angle=atan2((double)data[2],(double)data[0])*(180 / 3.14159265)+180;
	  //printf("offsetX:%f\n",offsetX);
	 // printf("offsetY:%f\n",offsetY);
		//printf("�Ƕ�:%d\n",(int)angle);

		//	printf("angle_diff%d:\n",angle_diff);
		delay_ms(10);
		//hcsr04_start();
		//delay_ms(500);
	//	printf("s:%d\n",s);
//		if(s<100)
//			while(s<100)
//			{
//				 
//					  	hcsr04_start();
//			 		  	delay_ms(500);
//							motorPID(1);
//							motorPID_retreat(1);	
//			}
		//TIM_SetCompare1(TIM3,18200);	
		//delay_ms(500);
		//TIM_SetCompare1(TIM3,18600);
		if(recive_flag==1)
		{
		 while(recive_ble[h]!='\r')
     {
        h++;
     }
		 // printf("%c\r\n",recive_ble[0]);
     if(recive_ble[0]=='A')//�Ƕ����� 
     {
        switch(h)
        { 
            case 2:
               angle_expected=(recive_ble[1]-'0');
               break;
                        
            case 3:    
               angle_expected=(recive_ble[1]-'0')*10+(recive_ble[2]-'0');
               break;   
						
            default:                    
               angle_expected=(recive_ble[1]-'0')*100+(recive_ble[2]-'0')*10+(recive_ble[3]-'0');
               break;
        }
				an_count++;
				if(first_flag)//��һ�²��
				{
					angle_diff=angle_expected-(int)angle;
					first_flag=0;
				}
				if(an_count%40==0)
				{
				//	angle_diff=angle_expected-(int)angle;//У��һ�½ǶȲ�ֵ
				//	if(an_count%80==0)
				//	{
						an_count=0;
//						if(s>0&&s<80)
//           {
//							s=0;
//							motorPID(1);
//							motorPID_retreat(1);
//			 		  	delay_ms(3000);
//	    		 }
//					}
//					else if(an_count%80!=0)
//						hcsr04_start();
					//printf("s:%d\n",s);
				}
			  	//printf("�Ƕ� diff:%d\n",angle_diff);
				 // printf("�Ƕ�:%d\n",angle_expected);
			// if(flag_dis_retreat==0)
					steerPIDA(&SD5_PID,(float)(angle_expected-angle_diff));//û�к���ʱ
		//		else
			//		steerPIDA(&SD5_PID,angle);//����ʱ�����Ըı䷽��
				if(flag_turn==1)//����ת��
				{		 
				 motorPID_retreat(1);
				 err_angle=angle_expected-angle_diff-(int)angle;					
					while(err_angle>5||err_angle<-5)//ת��Ĺ���
					{
						//__disable_irq();
//						printf("angle_expected%d:\n",angle_expected);
//						printf("angle_diff%d:\n",angle_diff);
//					  printf("(int)angle%d:\n",(int)angle);
//					  printf("�Ƕ� %d:\n",angle_expected-angle_diff-(int)angle);
						motorPID(48);
						delay_ms(60);
					  hmc_read_XYZ(data);
						data[0]=(float)data[0]-offsetX;
            data[2]=(float)data[2]-offsetY;
						angle=atan2((double)data[2],(double)data[0])*(180 / 3.14159265)+180;
						if(recive_flag==1)
						{	
						  while(recive_ble[h]!='\r')
							 {
									h++;
							 }
							if(recive_ble[0]=='A')//�Ƕ����� 
								 {
										switch(h)
										{ 
												case 2:
													 angle_expected=(recive_ble[1]-'0');
													 break;
																		
												case 3:    
													 angle_expected=(recive_ble[1]-'0')*10+(recive_ble[2]-'0');
													 break;   
												
												default:                    
													 angle_expected=(recive_ble[1]-'0')*100+(recive_ble[2]-'0')*10+(recive_ble[3]-'0');
													 break;
										}
									}
							h=0;recive_flag=0; //����	
						}
						err_angle=angle_expected-angle_diff-(int)angle;
					  if(err_angle>180)
							err_angle=err_angle-360;  
						else if(err_angle<-180)
							err_angle=err_angle+360;
					}
				//	if(flag_turn_big)
			//			delay_ms(3000);//����
		  			motorPID(1);
				//	if(!flag_turn_big)//С��
				//	{
						hmc_read_XYZ(data);
						data[0]=(float)data[0]-offsetX;
						data[2]=(float)data[2]-offsetY;
						angle=atan2((double)data[2],(double)data[0])*(180 / 3.14159265)+180;
						angle_diff=angle_expected-(int)angle;
						steerPIDA(&SD5_PID,(float)(angle_expected-angle_diff));
				//	}
				  motorPID(23);
					delay_ms(10);
					motorPID_retreat(1);
					//motorPID(1);

					delay_ms(1300);
					motorPID(1);
					an_count=0;
					flag_turn=0;
					flag_turn_big=0;
				}
				if(flag_turn_left)//��ʱС���Ѿ�����������ǰ��
				{
					distance_1=60;
					distance_2=54;
					flag_turn_left=0;
				}
				else if(flag_turn_right)//��ʱС���Ѿ�����������ǰ��
				{
					distance_1=66;
					distance_2=60;
					flag_turn_right=0;
				}
				
      }    
      else//��������
      { 	 
        switch(h)
        { 
                   case 2:
                           distance=(recive_ble[1]-'0');
                           break;
                        
                   case 3:    
                           distance=(recive_ble[1]-'0')*10+(recive_ble[2]-'0');
                           break;
                        
                   default:                    
                           //distance=(str[0]-'0')*100+(str[1]-'0')*10+(str[2]-'0');
                           break;
        }
				dis_data[i++]=distance;
		   }
			 h=0;recive_flag=0; //����
			 if(i==5)
			 {
				 i=0;
				 distance_proceed=data_process(dis_data);
//				 printf("����  %d\r\n",dis_data[0]);
//				  printf("����  %d\r\n",dis_data[1]);
//				  printf("����  %d\r\n",dis_data[2]);
//				  printf("����  %d\r\n",dis_data[3]);
//				  printf("����  %d\r\n",dis_data[4]);
			 }
			 //printf("����  %d\r\n",distance_proceed);
			 if(distance_proceed)//��������
       {
				    //flag_dis=0;
			  	  if(distance_proceed>=distance_1)
						{
               motorPID_retreat(51);//
							 motorPID(5);//����
							 //flag_dis=1;
							// if(flag_dis_retreat==1&&(distance>last_distance)) //���ߵ�ǰ��ȥ��
							//	  delay_ms(500);
							 flag_dis_retreat=1;
						}	
            else if(distance_proceed>=distance_2&&distance_proceed<distance_1)//����
						{
               motorPID_retreat(5);//
							 motorPID(5);//����
							// flag_dis=1;
							 flag_dis_retreat=0;
						}
            else if(distance_proceed<distance_2&&distance_proceed>=distance_3) //��Ҫǰ��������
            {
							motorPID_retreat(5);
              motorPID(52);
							//flag_dis=1;
							flag_dis_retreat=0;
						//printf("distanc\r\n");
            }
             else //С��distance_3
						{
							 motorPID_retreat(5);
               motorPID(60);//������ǰ��
							 flag_dis_retreat=0;
						}
       }
		   last_distance_proceed=distance_proceed;
			 //printf("����  %d\r\n",distance);
			 // printf("flag_dis_retreat %d\r\n",flag_dis_retreat); 
		}
		
	}
}
