#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/fb.h>
#include <linux/input.h>

//---------宏定义---------//
//文件路径
#define FB_PATH "/dev/fb0"          //屏幕文件路径
#define TS_PATH "/dev/input/event0" //触摸屏文件路径

//图片文件路径
#define START_PATH "start.bmp"          //开始界面
#define CHOOSE_PATH "choose.bmp"        //困难选择模式 
#define PLAY_256_PATH "play_256.bmp"    //简单模式
#define PLAY_2048_PATH "play_2048.bmp"   //一般模式
#define PLAY_4096_PATH "play_4096.bmp"   //困难模式

//颜色
#define RED 0xff0000
#define GREEN 0x00ff00
#define BLUE 0x0000ff
#define BLACK 0x000000
#define WHITE 0xffffff
#define YELLOW 0xffff00
#define PURPLE 0xff00ff
#define CYAN 0x00ffff

//屏幕虚拟区y轴偏移量
#define A 0
#define B 480
#define C 960 


//带参宏
//将得到的坐标转化为800*480的规格
#define conver_x(x) (800.0*(x)/1024)
#define conver_y(y) (480.0*(y)/600)

//---------宏定义---------//

//---------结构体---------//



//---------结构体---------//

//---------全局变量---------//
int fb_fd = 0;                  //屏幕文件描述符
int ts_fd = 0;                  //触摸屏文件描述符

//图片文件描述符
int start_fd;       //开始界面
int choose_fd;      //难度选择界面
int play_256_fd;    //简单模式
int play_2048_fd;   //一般模式
int play_4096_fd;   //困难模式

unsigned int *m_map = NULL;     //屏幕映像指针

//屏幕参数
struct fb_fix_screeninfo finfo; //固定参数
struct fb_var_screeninfo vinfo; //可变参数

//触摸屏参数
struct input_event ts;          //用于读取触摸屏事件信息

//获取屏幕虚拟区缓存大小
int v_width;    //屏幕虚拟区的宽
int v_height;   //屏幕虚拟区的高
int v_bpp;      //一个像素占的位数

//触摸屏滑动的开始坐标和结束坐标
int x_start = 0;
int y_start = 0;
int x_end = 0;
int y_end = 0;
int x_flag = 1;
int y_flag = 1;

//---------全局变量---------//


//---------函数---------//

//----------系统-----------//

//系统初始化：打开屏幕和触摸屏，构建映射

//可读可写打开文件
int open_file(char *path)
{
    int fd = 0;

    //打开文件
    fd = open(path, O_RDWR);

    //打开失败
    if(fd == -1)
    {
        printf("%s 文件打开失败！\n", path);
        return -1;
    }

    return fd;
}

//系统初始化
int sys_init()
{
    //--------打开文件-----------//
    //打开屏幕
    fb_fd = open_file(FB_PATH);

    //打开触摸屏
    ts_fd = open_file(TS_PATH);

    //打开图片
    start_fd = open_file(START_PATH);           //开始界面
    choose_fd = open_file(CHOOSE_PATH);         //困难选择界面    
    play_256_fd = open_file(PLAY_256_PATH);     //简单模式界面
    play_2048_fd = open_file(PLAY_2048_PATH);   //一般模式界面
    play_4096_fd = open_file(PLAY_4096_PATH);   //困难模式界面


    //--------打开文件-----------//

    //获取屏幕信息
    if(ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo) != 0)
    {
        printf("获取屏幕固定信息失败!\n");
        return -3;
    }
    if(ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo) != 0)
    {
        printf("获取屏幕可变信息失败!\n");
        return -4;
    }

    //获取屏幕虚拟区缓存大小
    v_width = vinfo.xres_virtual;
    v_height = vinfo.yres_virtual;
    v_bpp = vinfo.bits_per_pixel;

    //建立映射
    m_map = (unsigned int *)mmap(NULL, v_width*v_height*v_bpp/8, PROT_READ | PROT_WRITE, MAP_SHARED, fb_fd, 0);

    return 0;
}

//打开所有图片文件
// int sys_open_all_picture();

// //关闭所有图片文件
// int sys_close_all_picture();

//退出系统
int sys_close()
{
    //解除映射
    munmap(m_map, v_width * v_height * v_bpp / 8);

    //关闭屏幕文件
    close(fb_fd);

    //关闭触摸屏文件
    close(ts_fd);

    //关闭图片文件
    close(start_fd);        //开始界面
    close(choose_fd);       //难度界面
    close(play_256_fd);     //简单界面
    close(play_2048_fd);    //一般界面
    close(play_4096_fd);    //困难界面
    

    return 0;
}

//=======屏幕操作=======//

//显示单张800*480bmp图片(默认图片已打开)
int show_picture(int picture_fd)
{
    //跳过文件头
    lseek(picture_fd, 54, SEEK_SET);

    //把文件像素点内容写到缓存
    char buff_char[800 * 480 * 3] = {0};
    read(picture_fd, buff_char, sizeof(buff_char));

    //将文件像素点写入缓存
    int buff_int[800 * 480] = {0};
    int i, j;
    for(i = 0; i < 800*480; i++)
    {
        //写入一个像素点
        buff_int[i] = buff_char[i*3+0] | buff_char[i*3+1] << 8 | buff_char[i*3+2] << 16;  
    }

    //倒着将像素点写入虚拟内存的第一块
    for(i = 0; i < 480; i++)
    {
        for(j = 0; j < 800; j++)
        {
            *(m_map + i*800 + j) = buff_int[(480-i) * 800 + j];
        }
    }

    return 0;
}

//显示全屏同种颜色
int show_color(int color)
{
    int i, j;
    for(i = 0; i < 480; i++)
    {
        for(j = 0; j < 800; j++)
        {
            *(m_map + i*800 + j) = color;
        }
    }

    return 0;
}

// //单元化bmp图片,将一张图片转换为800*480的规格输出
// int picture_cell(int fd_picture)
// {
//     //跳过文件头
//     lseek(fd_picture, 54, SEEK_SET);

//     //用缓存读取数据
//     char buff_int
// }

//将图片定位到指定位置

//显示开始界面
int show_start()
{
    return show_picture(start_fd);
}

//显示难度界面
int show_choose()
{
    return show_picture(choose_fd);
}

//显示简单界面
int show_play_256()
{
    return show_picture(play_256_fd);
}

//显示一般界面
int show_play_2048()
{
    return show_picture(play_2048_fd);
}

//显示困难界面
int show_play_4096()
{
    return show_picture(play_4096_fd);
}

//切换虚拟界面
int change_interface(int offset)
{
    //修改偏移量
    vinfo.yoffset = offset;
    ioctl(fb_fd, FBIOPAN_DISPLAY, &vinfo);

    return 0;
}



//------触摸屏操作------//

//判断左右划和上下划(0:点击 1:左划 2：右划 3：上划 4：下划)
int judgment(int x_start, int y_start, int x_end, int y_end)//参数为 开始按下的坐标 和 松开时候的坐标
{
    //单击
    if((x_start == x_end && y_start == y_end) || (abs(x_start-x_end) + abs(y_start-y_end) < 30))
    {
        return 0;
    }
    //左右划
    else if(abs(x_start - x_end) > abs(y_start - y_end))//左右划（按下和松开 x坐标的差值 大于 y坐标的差值）
    {
        if(x_start > x_end)
        {
            return 1;
        }
        else
        {
            return 2;
        }
    }
    //上下划
    else
    {
        if(y_start > y_end)
        {
            return 3;
        }
        else
        {
            return 4;
        }
    }

    return 0;
}

//检测是否单击指定位置(是：1， 不是：0)
int ts_trigger(int x_start, int y_start, int x_end, int y_end, int x_real, int y_real)//参数：起始坐标，结束坐标，单击坐标
{
    if(x_real > x_start && x_real < x_end)      //x符合要求
    {
        if(y_real > y_start && y_real < y_end)  //y符合要求
        {
            return 1;       //满足条件
        }
        return 0;
    }
    return 0;
}

//开始界面
//单击开始游戏
int ts_get_start(int x_real, int y_real)
{
    return ts_trigger(332, 210, 474, 313, x_real, y_real);
}

//单击退出游戏
int ts_get_quit(int x_real, int y_real)
{
    return ts_trigger(248, 375, 562, 444, x_real, y_real);
}

//难度选择界面
//单击选择界面的返回
int ts_choose_back(int x_real, int y_real)
{
    return ts_trigger(30, 28, 69, 65, x_real, y_real) | ts_trigger(707, 104, 746, 139, x_real, y_real);
}

//单击简单模式
int ts_choose_easy(int x_real, int y_real)
{
    return ts_trigger(148, 317, 252, 356, x_real, y_real);
}

//单击一般模式
int ts_choose_general(int x_real, int y_real)
{
    return ts_trigger(333, 318, 438, 357, x_real, y_real);
}

//单击困难模式
int ts_choose_diff(int x_real, int y_real)
{
    return ts_trigger(503, 326, 637, 376, x_real, y_real);
}

//游戏界面
//单击返回
int ts_play_back(int x_real, int y_real)
{
    return ts_trigger(35, 8, 143, 63, x_real, y_real);
}

//单击退出游戏
int ts_play_quit(int x_real, int y_real)
{
    return ts_trigger(4, 79, 192, 141, x_real, y_real);
}

//单击新游戏
int ts_play_new(int x_real, int y_real)
{
    return ts_trigger(469, 20, 663, 82, x_real, y_real);
}

//----------界面操作处理----------//

//开始界面(-1:退出游戏，1：开始游戏)
int start_interface()
{
    //显示图片
    show_start();

     //循环读取触摸屏
    while(1)
    {
        //读触摸屏
        read(ts_fd, &ts, sizeof(struct input_event));

        //判断是否为绝对坐标类型
        if(ts.type == EV_ABS && ts.code == ABS_X && x_flag == 1)
        {
            x_start = x_end = ts.value;
            x_flag = 0;
        }

        if(ts.type == EV_ABS && ts.code == ABS_Y && y_flag == 1)
        {
            y_start = y_end = ts.value;
            y_flag = 0;
        }

        if(ts.type == EV_ABS && ts.code == ABS_X)
        {
            x_end = ts.value;
        }

        if(ts.type == EV_ABS && ts.code == ABS_Y)
        {
            y_end = ts.value;
        }


        //将压力值类型为按键
        if(ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 1);


        if(ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 0)
        {
            //转换
            x_start = conver_x(x_start);
            x_end = conver_x(x_end);
            y_start = conver_y(y_start);
            y_end = conver_y(y_end);

            printf("x_start:%d,y_start:%d\n", x_start, y_start);
            printf("x_end:%d,y_end:%d\n", x_end, y_end);
            int result = judgment(x_start, y_start, x_end, y_end);
            switch (result)
            {
            case 0://单击
                //判断是否退出游戏
                if(ts_get_quit(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return -1;
                }
                else if(ts_get_start(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 1;
                }
                break;

            case 1:
                printf("这是左划！\n");
                break;
            
            case 2:
                printf("这是右划！\n");
                break;
            
            case 3:
                printf("这是上划！\n");
                break;
            
            case 4:
                printf("这是下划！\n");
                break;
            
            default:
                break;
            }//switch

            //当次操作结束，重置flag值，使之能够记录下次操作的开始值
            x_flag = 1;
            y_flag = 1;
        }//if
    }//while(1)
}

//难度选择界面(0:返回，1：简单，2：中等，3：困难)
int choose_interface()
{
    //显示图片
    show_choose();

     //循环读取触摸屏
    while(1)
    {
        //读触摸屏
        read(ts_fd, &ts, sizeof(struct input_event));

        //判断是否为绝对坐标类型
        if(ts.type == EV_ABS && ts.code == ABS_X && x_flag == 1)
        {
            x_start = x_end = ts.value;
            x_flag = 0;
        }

        if(ts.type == EV_ABS && ts.code == ABS_Y && y_flag == 1)
        {
            y_start = y_end = ts.value;
            y_flag = 0;
        }

        if(ts.type == EV_ABS && ts.code == ABS_X)
        {
            x_end = ts.value;
        }

        if(ts.type == EV_ABS && ts.code == ABS_Y)
        {
            y_end = ts.value;
        }


        //将压力值类型为按键
        if(ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 1);


        if(ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 0)
        {
            //转换
            x_start = conver_x(x_start);
            x_end = conver_x(x_end);
            y_start = conver_y(y_start);
            y_end = conver_y(y_end);

            printf("x_start:%d,y_start:%d\n", x_start, y_start);
            printf("x_end:%d,y_end:%d\n", x_end, y_end);
            //判断
            int result = judgment(x_start, y_start, x_end, y_end);
            switch (result)
            {
            case 0://单击
                //判断是否退出游戏
                if(ts_choose_back(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 0;
                }
                else if(ts_choose_easy(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 1;
                }
                else if(ts_choose_general(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 2;
                }
                else if(ts_choose_diff(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 3;
                }
                break;

            case 1://左划显示黑色
                printf("这是左划！\n");
                break;
            
            case 2://右划显示白色
                printf("这是右划！\n");
                break;
            
            case 3://上划显示红色
                printf("这是上划！\n");
                break;
            
            case 4://下划显示绿色
                printf("这是下划！\n");
                break;
            
            default:
                break;
            }//switch

            //当次操作结束，重置flag值，使之能够记录下次操作的开始值
            x_flag = 1;
            y_flag = 1;
        }//if
    }//while(1)
}

//游戏界面(退出：-1，返回:0，新游戏：1)
int play_interface(int mode)
{
    //显示图片
    if(mode == 1)
    {
        show_picture(play_256_fd);
    }
    else if(mode == 2)
    {
        show_picture(play_2048_fd);
    }
    else if(mode == 3)
    {
        show_picture(play_4096_fd);
    }

    //循环读取触摸屏
    while(1)
    {
        //读触摸屏
        read(ts_fd, &ts, sizeof(struct input_event));

        //判断是否为绝对坐标类型
        if(ts.type == EV_ABS && ts.code == ABS_X && x_flag == 1)
        {
            x_start = x_end = ts.value;
            x_flag = 0;
        }

        if(ts.type == EV_ABS && ts.code == ABS_Y && y_flag == 1)
        {
            y_start = y_end = ts.value;
            y_flag = 0;
        }

        if(ts.type == EV_ABS && ts.code == ABS_X)
        {
            x_end = ts.value;
        }

        if(ts.type == EV_ABS && ts.code == ABS_Y)
        {
            y_end = ts.value;
        }


        //将压力值类型为按键
        if(ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 1);


        if(ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 0)
        {
            //转换
            x_start = conver_x(x_start);
            x_end = conver_x(x_end);
            y_start = conver_y(y_start);
            y_end = conver_y(y_end);

            printf("x_start:%d,y_start:%d\n", x_start, y_start);
            printf("x_end:%d,y_end:%d\n", x_end, y_end);
            //判断
            int result = judgment(x_start, y_start, x_end, y_end);
            switch (result)
            {
            case 0://单击
                //判断是否退出游戏
                if(ts_play_quit(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return -1;
                }
                //返回
                else if(ts_play_back(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 0;
                }
                else if(ts_play_new(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 1;
                }
                break;

            case 1://左划显示黑色
                printf("这是左划！\n");
                break;
            
            case 2://右划显示白色
                printf("这是右划！\n");
                break;
            
            case 3://上划显示红色
                printf("这是上划！\n");
                break;
            
            case 4://下划显示绿色
                printf("这是下划！\n");
                break;
            
            default:
                break;
            }//switch

            //当次操作结束，重置flag值，使之能够记录下次操作的开始值
            x_flag = 1;
            y_flag = 1;
        }//if
    }//while(1)
}

//---------函数---------//



int main(void)
{
    //打开设备文件
    sys_init();

    int rt = -1;
    //打开开始界面
    rt = start_interface();

    //开始界面
    if(rt == -1)//退出游戏
    {
        //黑屏
        show_color(BLACK);
        //退出程序
        sys_close();
        return 0;
    }
    else if(rt == 1)
    {
        rt = choose_interface();
    }

    //难度选择
    if(rt == 0)
    {
        //返回
        rt = start_interface();
    }
    else
    {
        //游戏
        rt = play_interface(rt);
    }

    //游戏
    if(rt == -1)
    {
        //黑屏
        show_color(BLACK);
        //退出程序
        sys_close();
    }
    else if(rt == 0)
    {
        //返回难度选择
        rt = choose_interface();
    }
    else if(rt == 1)
    {
        //新游戏
        play_interface(1);
    }

    //关闭系统
    sys_close();

    return 0;
}