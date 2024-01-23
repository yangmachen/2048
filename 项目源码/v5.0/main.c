#include "all.h"

int main(void)
{
    x_flag = 1;
    y_flag = 1;

    //随机数种子
    srand((unsigned int)time(NULL));

    //打开设备文件
    sys_init();

    int rt = 0;
    
    //开始游戏
    fb_show_picture(yueqian_fd);
    sleep(1);
    rt = start_interface();
    
    if(rt == -1)//退出游戏
    {
        //游戏退出界面
        fb_show_picture(quit_fd);
    }

    //关闭系统
    sys_close();

    return 0;
}
