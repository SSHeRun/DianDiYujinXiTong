#define uchar unsigned char
#define uint unsigned int

sbit A1=P2^4; //定义步进电机连接端口
sbit B1=P2^5;
sbit C1=P2^6;
sbit D1=P2^7;


#define Coil_A1 {A1=1;B1=0;C1=0;D1=0;}//A相通电，其他相断电
#define Coil_B1 {A1=0;B1=1;C1=0;D1=0;}//B相通电，其他相断电
#define Coil_C1 {A1=0;B1=0;C1=1;D1=0;}//C相通电，其他相断电
#define Coil_D1 {A1=0;B1=0;C1=0;D1=1;}//D相通电，其他相断电
#define Coil_AB1 {A1=1;B1=1;C1=0;D1=0;}//AB相通电，其他相断电
#define Coil_BC1 {A1=0;B1=1;C1=1;D1=0;}//BC相通电，其他相断电
#define Coil_CD1 {A1=0;B1=0;C1=1;D1=1;}//CD相通电，其他相断电
#define Coil_DA1 {A1=1;B1=0;C1=0;D1=1;}//D相通电，其他相断电
#define Coil_OFF {A1=0;B1=0;C1=0;D1=0;}//全部断电

unsigned char Speed;

/*------------------------------------------------
 步进电机移动函数，含有输入参数 unsigned char flag 是
 定义移动的方向，1为正转，0为反转，unsigned char s 是
 定义转动的数值，512为一圈
------------------------------------------------*/
void Move(uchar flag,uchar s)
{
	Speed=8;
	Coil_OFF
	if (flag==1)
	{
		//Coil_OFF
		while(s--)  //正向
		{         
			Coil_A1                //遇到Coil_A1  用{A1=1;B1=0;C1=0;D1=0;}代替
			DelayMs(Speed);         //改变这个参数可以调整电机转速 ,
									//数字越小，转速越大,力矩越小
			Coil_B1
			DelayMs(Speed);
			Coil_C1
			DelayMs(Speed);
			Coil_D1
			DelayMs(Speed);
		}
	}else if(flag==0)
	{
		//Coil_OFF
		while(s--)//反向
		{
			Coil_D1                //遇到Coil_A1  用{A1=1;B1=0;C1=0;D1=0;}代替
			DelayMs(Speed);         //改变这个参数可以调整电机转速 ,
									//数字越小，转速越大,力矩越小
			Coil_C1
			DelayMs(Speed);
			Coil_B1
			DelayMs(Speed);
			Coil_A1
			DelayMs(Speed);
		}
	}
}



