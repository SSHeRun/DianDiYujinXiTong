#ifndef __LCD1602_H__
#define __LCD1602_H__


#define lcd1602_io P0
sbit lcd1602_en = P2 ^ 0;
sbit lcd1602_rw = P2 ^ 1;
sbit lcd1602_rs = P2 ^ 2;


/*------------------------------------------------
              写入命令函数
------------------------------------------------*/
void lcd1602_write_cmd(unsigned char cmd)
{
    lcd1602_io = cmd;
    lcd1602_rs = 0;
    lcd1602_en = 0;
    DelayMs(1);
    lcd1602_en = 1;
    DelayMs(1);
    lcd1602_en = 0;
}
/*------------------------------------------------
              写入数据函数
------------------------------------------------*/
void lcd1602_write_data(unsigned char dat)
{
    lcd1602_io = dat;
    lcd1602_rs = 1;
    lcd1602_en = 0;
    DelayMs(1);
    lcd1602_en = 1;
    DelayMs(1);
    lcd1602_en = 0;

}
/*------------------------------------------------
              初始化函数
------------------------------------------------*/
void lcd1602_init()
{
    lcd1602_rw = 0;
    lcd1602_write_cmd(0x38);   //显示模式设置：16×2显示，5×7点阵，8位数据接口
    DelayMs(1);
    lcd1602_write_cmd(0x0e);   //显示模式设置
    DelayMs(1);
    lcd1602_write_cmd(0x06);   //显示模式设置：光标右移，字符不移
    DelayMs(1);
    lcd1602_write_cmd(0x01);   //清屏幕指令，将以前的显示内容清除
    DelayMs(1);
}

/*------------------------------------------------
                清屏函数
------------------------------------------------*/
void lcd1602_clear(void) 
{ 
	lcd1602_write_cmd(0x01); 
	DelayMs(1);
}
/*------------------------------------------------
                右移滚动函数
------------------------------------------------*/
void lcd1602_turn_left(void)
{
    lcd1602_write_cmd(0x1c);//右平移画面 
}
/*------------------------------------------------
                左移滚动函数
------------------------------------------------*/
void lcd1602_turn_right(void)
{
    lcd1602_write_cmd(0x18);//左平移画面 
}
/*------------------------------------------------
              写入字符函数
------------------------------------------------*/
void lcd1602_write_char(unsigned char x, unsigned char y, unsigned char dat) //坐标定位
{
    unsigned char address;
    if(y == 0)
        address = 0x80 + x; //y=0,写第一行
    else
        address = 0xc0 + x; //y=1,写第二行
    lcd1602_write_cmd(address);
    lcd1602_write_data(dat);
}
/*------------------------------------------------
              写入字符串函数
------------------------------------------------*/
void lcd1602_write_string(unsigned char x, unsigned char y, unsigned char *dat) //坐标定位
{
    unsigned char address;
    if(y == 0)
        address = 0x80 + x; //y=0,写第一行
    else if(y == 1)
        address = 0xc0 + x; //y=1,写第二行
    lcd1602_write_cmd(address);
    while(*dat > 0)
    {
        lcd1602_write_data(*dat);
        dat++;
        DelayMs(1);
    }
}
#endif //__LCD1602_H__
