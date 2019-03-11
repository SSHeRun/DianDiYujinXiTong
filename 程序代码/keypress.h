#define uchar unsigned char
#define uint unsigned int
uchar KEYIN;                 
/*------------------------------------------------
 按键检测函数，当有按键按下，返回1，否则，返回0
------------------------------------------------*/
uchar KeyPress(void)
{
    uchar keypress;
    P1 = 0xF0;                                     
    keypress = P1;
    if (keypress == 0xF0)
    {
        return 0;
    }
    else 
    {
        KEYIN = keypress;
        return 1;
    }
}
/*------------------------------------------------
 按键数值返回函数，输入参数无，返回按键数值
------------------------------------------------*/
uchar KeyScan(void)
{//键盘扫描函数，使用行列逐级扫描法
    uchar keyout,index,keycode,keyvalue;
    DelayMs(20);
    P1 = 0xF7;
    index = 0;
    if (KeyPress())
    {
        while (1)
        {
            P1 = (0xF7 >> index) | 0xF0; 
            index++;
            index = index & 0x03;
            keyout = P1 & 0x0F;
            if ((P1 & 0xF0) != 0xF0)
            {
                keycode = KEYIN + keyout;
                break;  
            }

        }
        switch (keycode)
        {//返回扫键值
            case 0x77 : keyvalue = 16;     break;//按下相应的键显示相对应的码值
            case 0x7B : keyvalue = 15;     break;
            case 0x7D : keyvalue = 14;     break;
            case 0x7E : keyvalue = 13;     break;

            case 0xB7 : keyvalue = 12;     break;
            case 0xBB : keyvalue = 11;     break;
            case 0xBD : keyvalue = 0;     break;
            case 0xBE : keyvalue = 9;     break;

            case 0xD7 : keyvalue = 8;     break;
            case 0xDB : keyvalue = 7;     break;
            case 0xDD : keyvalue = 6;     break;
            case 0xDE : keyvalue = 5;     break;

            case 0xE7 : keyvalue = 4;     break;
            case 0xEB : keyvalue = 3;     break;
            case 0xED : keyvalue = 2;     break;
            case 0xEE : keyvalue = 1;     break;
            default : keyvalue = 0xFF;
        }
    }
    else
    {
        keyvalue = 0xFF;
    }
    return keyvalue;
}
