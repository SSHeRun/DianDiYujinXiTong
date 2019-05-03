#include<reg52.h>
#include<intrins.h>
#include "delay.h"
#include "lcd1602.h"
#include "steper.h"
#include "keypress.h"

uchar dishu_sheding = 0;//设定滴数
uchar dishu_shiji = 0;//实际滴数
uchar diandi_number = 0;//滴数数值
uchar Number = 0;//时间计数
uchar PuZh[4] = "";//存储发送数据
uchar level = 0;//当前步进电机的位置

sbit buzzer = P2^0;//蜂鸣器
/*******************************************************************************
* 函 数 名       : Timer_Init()
* 函数功能		   : 设置定时
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/
void Timer_Init()
{
	TMOD = 0x11;//定时器0定时器1为工作方式1
	/*  装载初值  */
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	
	EA = 1;//开总中断
	/*  开启定时器中断  */
	ET0 = 1;//定时器0
	/*  启动定时器  */
	TR0 = 1;//定时器0
	/*  外部中断  */
	EX0 = 1;
	EX1 = 1;
	IT0 = 1;
	IT1 = 1;
}

/*******************************************************************************
* 函 数 名       : UsartConfiguration()
* 函数功能		   : 设置串口
* 输    入       : 无
* 输    出       : 无
*******************************************************************************/

void UsartConfiguration()
{	
	SCON=0X50;			//设置为工作方式1,8位数据，可变波特率
	TMOD=0X20;			//设置计数器工作方式2
	PCON=0X00;			//波特率不加倍
	TH1=0XFd;		    //计数器初始值设置，9600@11.0592MHz
	TL1=0XFd;
	TR1=1;					//打开计数器
	ES = 1;         //开串口中断
  EA = 1;         //开总中断
}


/*------------------------------------------------
 中断0函数，检测滴管里面下落点滴的数量
------------------------------------------------*/
void Interrupt0() interrupt 0
{
	diandi_number++;
	DelayMs(100);//软件防止波动
}
/*------------------------------------------------
定时计数0，设定为60s，检测点滴速度，时间越长，精度越高
------------------------------------------------*/
void T0_Time() interrupt 1 
{
	/*  重装初值  */
	TH0 = (65536-50000)/256;
	TL0 = (65536-50000)%256;
	Number++;//自动校正流速程序
	if(Number==20*60)//定时1分钟
	{
		Number = 0;
		dishu_shiji = diandi_number;
		if((dishu_shiji-dishu_sheding)>6)//滴速过快，步进电机正向转动，减慢滴速
		{
			
			if(level<=13)//步进电机是否转到最大角度
				{
					level++;
					Move(1,10);//正向转动
				}
		}
		else if((dishu_shiji-dishu_sheding)<-6)//滴速过慢，步进电机反向转动，加快滴速
		{
			
			if(level>=0)//步进是否转到最小角度
				{
					level--;
					Move(0,10);//反向转动
				}
			
		}
		else;
		
		diandi_number = 0;
	}
}
/*------------------------------------------------
 中断1函数，检测点滴袋是否有液体，当发生中断，蜂鸣器报警，并且关闭输液管
------------------------------------------------*/
void Interrupt1() interrupt 2
{
	uchar i=0;
	for(;i<13-level;i++)
	{
		Move(1,10);
	}
	dishu_sheding=0;
	buzzer = 0;
}

/********************************************************************
* 名称 : Com_Int()
* 功能 : 串口中断子函数
* 输入 : 无 * 输出 : 无
***********************************************************************/
void Com_Int(void) interrupt 4
{
	uchar i;
  uchar receive_data;
	
  EA = 0;
	
  if(RI == 1) //当硬件接收到一个数据时，RI会置位
	{ 		
		RI = 0;
		receive_data = SBUF;//接收到的数据
			
		if(receive_data == '1')	 
		{
			PuZh[0]='0'+dishu_shiji/100;
			PuZh[1]='0'+dishu_shiji%100/10;
			PuZh[2]='0'+dishu_shiji%10;
			PuZh[3]='\0';
			for(i=0; i<4; i++)
			{
					SBUF = PuZh[i];   //将要发送的数据放入到发送寄存器
					while(!TI);		    //等待发送数据完成
					TI=0;			        //清除发送完成标志位
					DelayMs(1);
					//LED =0;
			}
		}
		else if(receive_data == '2')//关闭蜂鸣器
		{
				buzzer=1; 
		}
		else if(receive_data == '3')//正向调整步进电机
		{
				if(level<=13)
				{
					level++;
					Move(1,10);
				} 
		}
			else if(receive_data == '4')//反向调整步进电机
		{
					if(level>=0)
				{
					level--;
					Move(0,10);
				}
		}
			else if(receive_data == '5')//关闭点滴
		{
					uchar i=0;
					for(;i<13-level;i++)
					{
						Move(1,10);
					}
					dishu_sheding=0;
			}
		else;
		
	}
		
	
		EA = 1;
}


void main()
{
	
	uchar keyvalue;//键值
	uchar k;//设定滴数显示的位置
	uchar value[3]={0,0,0};//保存输入设定滴数的值,默认为0
	uchar i;//临时变量
	
	buzzer=1;//初始化蜂鸣器
	lcd1602_init();//初始化lcd
	Timer_Init();//初始化定时
	UsartConfiguration();//初始化串口
		
	
	while(1)
	{		
		lcd1602_clear();//清屏
		lcd1602_write_string(0,0,"now:");
		lcd1602_write_string(9,0,"d/min");
		lcd1602_write_string(0,1,"set:");
		lcd1602_write_string(9,1,"d/min");
		lcd1602_write_char(6,0,'0'+dishu_shiji/100);
		lcd1602_write_char(7,0,'0'+dishu_shiji%100/10);
		lcd1602_write_char(8,0,'0'+dishu_shiji%10);
			
			
		if (KeyPress())
        {//按键处理程序
            keyvalue = KeyScan();
					
						if (keyvalue ==14)
						{//手动校准步进电机
							DelayMs(200);//去抖
							while(1)
							{
								if(KeyPress())
								{
									keyvalue = KeyScan();
									if(keyvalue == 11)//正向调整
									{
										Move(1,10);
									}						
									if(keyvalue == 12)//反向调整
									{
										Move(0,10);
									}
									if(keyvalue ==16)//退出
									{
										level = 0;
										break;
									}
									while (KeyPress());
								}
							}	
						}
						
						if(keyvalue ==15)
						{//设置显示模式
							DelayMs(200);//去抖
							while(1)
							{
								if(KeyPress())
								{
									keyvalue = KeyScan();
									if(keyvalue ==11)//向左滚动
									{
											i=40;
											while(i--)
											{
												DelayMs(200);
												lcd1602_turn_right();
											}		
									}
									if(keyvalue ==12)//向右滚动
									{
											i=40;
											while(i--)
											{
												DelayMs(200);
												lcd1602_turn_left();
											}
									}
									if(keyvalue ==16)//清屏并退出
									{
										lcd1602_clear();
										break;
									}
									while (KeyPress());
								}
							}
							
						}
										
						
						if(keyvalue==13)
						{//设定滴数
							DelayMs(200);//去抖
							k=6;
							while(1)
							{
								DelayMs(200);//去抖
								if(KeyPress())
								{
									keyvalue = KeyScan();
									if(keyvalue==0||keyvalue==1||keyvalue==2||keyvalue==3||keyvalue==4||keyvalue==5||keyvalue==6||keyvalue==7||keyvalue==8||keyvalue==9)
									{
										if(k==6)
										{
											value[0]=keyvalue;
										}
										else if(k==7)
										{
											value[1]=keyvalue;
										}
										else if(k==8)
										{
											value[2]=keyvalue;
										}
										keyvalue = '0'+ keyvalue;						
										lcd1602_write_char(k,1,keyvalue);
									}
									else if(keyvalue == 11)
									{
										k=k-1;
									}
									else if(keyvalue == 12)
									{
										k=k+1;
									}
									else if (keyvalue==16)
									{
										dishu_sheding = value[0]*100+value[1]*10+value[2]; 
										break;
									}
									else;
									while (KeyPress());
								}
							}
						}
            while (KeyPress());
        }else{
					lcd1602_write_char(6,1,'0'+dishu_sheding/100);
					lcd1602_write_char(7,2,'0'+dishu_sheding%100/10);
					lcd1602_write_char(8,3,'0'+dishu_sheding%10);
				}
				
		DelayMs(200);
	}
} 

