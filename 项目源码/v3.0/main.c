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
#include <time.h>


//---------宏定义---------//
//文件路径
#define FB_PATH "/dev/fb0"          //屏幕文件路径
#define TS_PATH "/dev/input/event0" //触摸屏文件路径

//图片文件路径
#define START_PATH "start.bmp"          //开始界面
#define CHOOSE_PATH "choose.bmp"        //困难选择模式 
#define PLAY_256_PATH "play_256.bmp"    //简单模式
#define PLAY_2048_PATH "play_2048.bmp"  //一般模式
#define PLAY_4096_PATH "play_4096.bmp"  //困难模式
#define BLANK_PATH "blank.bmp"          //无数字方块
#define BLOCK_2_PATH "block_2.bmp"      //2方块
#define BLOCK_4_PATH "block_4.bmp"      //4方块
#define BLOCK_8_PATH "block_8.bmp"      //8方块
#define BLOCK_16_PATH "block_16.bmp"    //16方块
#define BLOCK_32_PATH "block_32.bmp"    //32方块
#define BLOCK_64_PATH "block_64.bmp"    //64方块
#define BLOCK_128_PATH "block_128.bmp"  //128方块
#define BLOCK_256_PATH "block_256.bmp"  //256方块
#define BLOCK_512_PATH "block_512.bmp"  //512方块
#define BLOCK_1024_PATH "block_1024.bmp"//1024方块
#define BLOCK_2048_PATH "block_2048.bmp"//2048方块
#define BLOCK_4096_PATH "block_4096.bmp"//4096方块

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

//矩阵参数
#define SIZE 4
#define chance_of_2 80  //出现2的概率（百分比）

//带参宏
//将得到的坐标转化为800*480的规格
#define conver_x(x) (800.0*(x)/1024)
#define conver_y(y) (480.0*(y)/600)

//---------宏定义---------//

//---------结构体---------//

//数字矩阵
int matrix[SIZE][SIZE] = {0};

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
int blank_fd;       //空白
int block_2_fd;     //方块2
int block_4_fd;     //方块4
int block_8_fd;     //...
int block_16_fd;
int block_32_fd;
int block_64_fd;
int block_128_fd;
int block_256_fd;
int block_512_fd;
int block_1024_fd;
int block_2048_fd;
int block_4096_fd;


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

//---------矩阵运算---------//

//在0的位置任意出现数字2/4，出现2的概率能够在宏定义中控制(失败返回-1)
int random_2()
{
    //计算当次出现2还是4
    int num = 0;
    if((rand() % 100 + 1) <= 20)
    {
        num = 4;
    }
    else
    {
        num = 2;
    }
    //计算0的数量
    int count_0 = 0;
    int i, j;

    //遍历计算
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            if(matrix[i][j] == 0)
            {
                count_0++;
            }
        }
    }

    //没有零
    if(count_0 == 0)
    {
        return -1;
    }

    int rand_num = rand() % count_0 + 1;  //决定将第几个零改为1

    int n = 0;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            if(matrix[i][j] == 0)
            {
                n++;
                //判断是否是当前的数改为2
                if(n == rand_num)
                {
                    matrix[i][j] = num;
                }
            }
            
        }
    }

    count_0--;
    return count_0; //返回剩余0的个数
}

//初始化：出现两个2
int init_play()
{
    //生成两个随机的位置
    int rand_1 = rand() % (SIZE * SIZE) + 1;
    int rand_2 = rand() % (SIZE * SIZE) + 1;;
    while(rand_1 == rand_2)
    {
        rand_2 = rand() % (SIZE * SIZE) + 1;
    }

    //在这两个位置放2
    int i, j;
    int count = 0;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            count++;
            if(count == rand_1 || count == rand_2)
            {
                matrix[i][j] = 2;
            }
        }
    }

    return 0;
}

//通过矩阵画出图形
int draw_matrix()
{
    int i, j;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            switch (matrix[i][j])
            {
            case 0:
                local_show_96_90(blank_fd, i , j);
                break;
            case 2:
                local_show_96_90(block_2_fd, i , j);
                break;
            case 4:
                local_show_96_90(block_4_fd, i , j);
                break;
            case 8:
                local_show_96_90(block_8_fd, i , j);
                break;
            case 16:
                local_show_96_90(block_16_fd, i , j);
                break;
            case 32:
                local_show_96_90(block_32_fd, i , j);
                break;
            case 64:
                local_show_96_90(block_64_fd, i , j);
                break;
            case 128:
                local_show_96_90(block_128_fd, i , j);
                break;
            case 256:
                local_show_96_90(block_256_fd, i , j);
                break;
            case 512:
                local_show_96_90(block_512_fd, i , j);
                break;
            case 1024:
                local_show_96_90(block_1024_fd, i , j);
                break;
            case 2048:
                local_show_96_90(block_2048_fd, i , j);
                break;
            case 4096:
                local_show_96_90(block_4096_fd, i , j);
                break;
                
            default:
                break;
            }//switch
        }//for(j)
    }//for(i)

    return 0;
}

//寻找矩阵中是否有某一数字(有：0，没有：-1)
int find_num(int num)
{
    int rt = -1; //初始化为找不到的返回值

    int i, j;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            if(matrix[i][j] == num)
            {
                return 0;
            }
        }
    }

    return -1;
}

//判断矩阵是否已满（未满：0，已满：-1）
int matrix_is_full()
{
    return find_num(0);
}

//矩阵上划(移动：0，每移动：-1)
int matrix_up()
{
//修修修
    int move = 0;   //检测当次是否有移动
    int temp[SIZE];
    int count = 0;
    //一列一列遍历
    int i, j, k;   //i表示行，j表示列
    for(j = 0; j < SIZE; j++)
    {
        count = 0;
        //找到有多少个非零元素
        for(i = 0; i < SIZE; i++)
        {
            if(matrix[i][j] != 0)
            {
                //将元素放入数组
                temp[count++] = matrix[i][j];
            }//if
        }//for(i)

//修修修
        //检测这些非零元素是否在一起
        for(i = 0; i < count; i++)
        {
            if(matrix[i][j] == 0)
            {
                move = 1;
                break;
            }
        }
///////////
        //将非零元素从第一行开始放
        if(count != 0 && count != SIZE)
        {
            for(k = 0; k < count; k++)
            {
                matrix[k][j] = temp[k];
            }
            //下面的方块赋0
            for(k = k; k < SIZE; k++)
            {
                matrix[k][j] = 0;
            }
        }
        

        //判断是否有相等的
        for(i = 0; i < count-1; i++)
        {
            if(matrix[i][j] == matrix[i+1][j])//当前值与下一个相等
            {
//修修修
                move = 1;
                matrix[i][j] *= 2;
                matrix[i+1][j] = 0;
                //后面的往前移一格
                for(k = i+2; k < count; k++)//上一个等这一个
                {
                    matrix[k-1][j] = matrix[k][j];
                    matrix[k][j] = 0;//将当前的赋0
                }
                //非零数少了一个
                count--;
            }
        }//for(i) 
        
    }//for(j)

    //没有移动
    if(move == 0)
    {
        return -1;
    }

    return 0;
}

//矩阵下划
int matrix_down()
{
    int move = 0;
    int temp[SIZE];
    int count = 0;
    //一列一列遍历
    int i, j, k;   //i表示行，j表示列
    for(j = 0; j < SIZE; j++)
    {
        count = 0;
        //找到有多少个非零元素
        for(i = 0; i < SIZE; i++)
        {
            if(matrix[i][j] != 0)
            {
                //将元素放入数组
                temp[count++] = matrix[i][j];
            }//if
        }//for(i)

        //检测这些非零元素是否在一起
        for(i = SIZE-1; i >= SIZE-count; i--)
        {
            if(matrix[i][j] == 0)
            {
                move = 1;
                break;
            }
        }

        //将非零元素从第一行开始放
        if(count != 0 && count != SIZE)
        {
            for(k = 0; k < count; k++)
            {
                matrix[k+SIZE-count][j] = temp[k];
            }
            //上面的方块赋0
            int temp = k;
            for(k = k; k < SIZE; k++)
            {
                matrix[k-temp][j] = 0;
            }
        }
        

        //判断是否有相等的
        for(i = SIZE-1; i > SIZE-count; i--)//从最后一个开始到判断
        {
            if(matrix[i][j] == matrix[i-1][j])//当前值与上一个相等
            {
                move = 1;
                matrix[i][j] *= 2;
                matrix[i-1][j] = 0;
                //后面的往前移一格
                for(k = i-2; k >= SIZE-count; k--)//下一个等这一个
                {
                    matrix[k+1][j] = matrix[k][j];
                    matrix[k][j] = 0;//将当前的赋0
                }
                //非零数少了一个
                count--;
            }
        }//for(i)
        

    }//for(j)

    //没有移动
    if(move == 0)
    {
        return -1;
    }
    return 0;
}

//矩阵左划
int matrix_left()
{
    int move = 0;
    int temp[SIZE];
    int count = 0;
    //一列一列遍历
    int i, j, k;   //i表示行，j表示列
    for(i = 0; i < SIZE; i++)
    {
        count = 0;
        //找到有多少个非零元素
        for(j = 0; j < SIZE; j++)
        {
            if(matrix[i][j] != 0)
            {
                //将元素放入数组
                temp[count++] = matrix[i][j];
            }//if
        }//for(i)

        //检测这些非零元素是否在一起
        for(j = 0; j < count; j++)
        {
            if(matrix[i][j] == 0)
            {
                move = 1;
                break;
            }
        }

        //将非零元素从第一行开始放
        if(count != 0 && count != SIZE)
        {
            for(k = 0; k < count; k++)
            {
                matrix[i][k] = temp[k];
            }
            //下面的方块赋0
            for(k = k; k < SIZE; k++)
            {
                matrix[i][k] = 0;
            }
        }
        

        //判断是否有相等的
        for(j = 0; j < count-1; j++)
        {
            if(matrix[i][j] == matrix[i][j+1])//当前值与下一个相等
            {
                move = 1;
                matrix[i][j] *= 2;
                matrix[i][j+1] = 0;
                //后面的往前移一格
                for(k = j+2; k < count; k++)//上一个等这一个
                {
                    matrix[i][k-1] = matrix[i][k];
                    matrix[i][k] = 0;//将当前的赋0
                }
                //非零数少了一个
                count--;
            }
        }//for(i)

    }//for(j)
    
    //没有移动
    if(move == 0)
    {
        return -1;
    }
    return 0;
}

//矩阵右划
int matrix_right()
{
    int move = 0;   //初始化为没有移动
    int temp[SIZE];
    int count = 0;  //用于计算当前零的个数
    //一列一列遍历
    int i, j, k;   //i表示行，j表示列
    for(i = 0; i < SIZE; i++)
    {
        //在分析每一行的开始处零的个数初始化为0
        count = 0;
        //找到有多少个非零元素
        for(j = 0; j < SIZE; j++)
        {
            if(matrix[i][j] != 0)
            {
                //将元素放入数组
                temp[count++] = matrix[i][j];
            }//if
        }//for(i)

        //检测这些非零元素是否在一起
        for(j = SIZE-1; j >= SIZE-count; j--)
        {
            if(matrix[i][j] == 0)
            {
                move = 1;
                break;
            }
        }

        //将非零元素从变化后的第一个非零位置开始放
        if(count != 0 && count != SIZE)
        {
            for(k = 0; k < count; k++)
            {
                matrix[i][k+SIZE-count] = temp[k];
            }
            //左边的方块赋0
            int temp = k;
            for(k = k; k < SIZE; k++)
            {
                matrix[i][k-temp] = 0;
            }
        }


        //判断是否有相等的
        for(j = SIZE-1; j > SIZE-count; j--)//从最后一个开始到判断
        {
            if(matrix[i][j] == matrix[i][j-1])//当前值与上一个相等
            {
                move = 1;
                matrix[i][j] *= 2;
                matrix[i][j-1] = 0;
                //后面的往前移一格
                for(k = j-2; k >= SIZE-count; k--)//下一个等这一个
                {
                    matrix[i][k+1] = matrix[i][k];
                    matrix[i][k] = 0;//将当前的赋0
                }
                //非零数少了一个
                count--;
            }
        }//for(i)

    }//for(j)

    //没有移动
    if(move == 0)
    {
        return -1;
    }
    return 0;
}

//上划
int turn_up(int mode)//-1:失败 1：胜利 0：继续游戏
{
    int rt = 0;
    int rt2 = 0;
    //提示
    printf("这是上划！\n");

    //矩阵上划
    rt2 = matrix_up();
    draw_matrix();

    //判断游戏是否结束
    rt = matrix_is_full();
    if(rt == -1)
    {
        return -1;
    }

    //判断游戏是否胜利
    if(mode == 1 && find_num(256) == 0)
    {
        return 1;
    }
    if(mode == 2 && find_num(2048) == 0)
    {
        return 1;
    }
    if(mode == 3 && find_num(4096) == 0)
    {
        return 1;
    }

    //继续游戏
    //随机生成一个2
    if(rt2 == 0)
    {
        random_2();
        draw_matrix();
    }
    
    return 0;
}

//下划
int turn_down(int mode)//-1:失败 1：胜利 0：继续游戏
{
    int rt2 = 0;
    //提示
    printf("这是下划！\n");

    //矩阵下划
    rt2 = matrix_down();
    draw_matrix();

    //判断游戏是否结束
    int rt = matrix_is_full();
    if(rt == -1)
    {
        return -1;
    }

    //判断游戏是否胜利
    if(mode == 1 && find_num(256) == 0)
    {
        return 1;
    }
    if(mode == 2 && find_num(2048) == 0)
    {
        return 1;
    }
    if(mode == 3 && find_num(4096) == 0)
    {
        return 1;
    }

    //继续游戏
    //随机生成一个2
    if(rt2 == 0)
    {
        random_2();
        draw_matrix();
    }
    
    return 0;
}

//左划
int turn_left(int mode)//-1:失败 1：胜利 0：继续游戏
{
    int rt2 = 0;
    //提示
    printf("这是左划！\n");

    //矩阵左划
    rt2 = matrix_left();
    draw_matrix();

    //判断游戏是否结束
    int rt = matrix_is_full();
    if(rt == -1)
    {
        return -1;
    }

    //判断游戏是否胜利
    if(mode == 1 && find_num(256) == 0)
    {
        return 1;
    }
    if(mode == 2 && find_num(2048) == 0)
    {
        return 1;
    }
    if(mode == 3 && find_num(4096) == 0)
    {
        return 1;
    }

    //继续游戏
    //随机生成一个2
    if(rt2 == 0)
    {
        random_2();
        draw_matrix();        
    }

    return 0;
}

//右划
int turn_right(int mode)//-1:失败 1：胜利 0：继续游戏
{
    int rt2 = 0;
    //提示
    printf("这是右划！\n");

    //矩阵右划
    rt2 = matrix_right();
    draw_matrix();

    //判断游戏是否结束
    int rt = matrix_is_full();
    if(rt == -1)
    {
        return -1;
    }

    //判断游戏是否胜利
    if(mode == 1 && find_num(256) == 0)
    {
        return 1;
    }
    if(mode == 2 && find_num(2048) == 0)
    {
        return 1;
    }
    if(mode == 3 && find_num(4096) == 0)
    {
        return 1;
    }

    //继续游戏
    //随机生成一个2
    if(rt2 == 0)
    {
        random_2();
        draw_matrix();
    }

    return 0;
}
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
    blank_fd = open_file(BLANK_PATH);           //空白块
    block_2_fd = open_file(BLOCK_2_PATH);       //2方块
    block_4_fd = open_file(BLOCK_4_PATH);       //4方块
    block_8_fd = open_file(BLOCK_8_PATH);       //...
    block_16_fd = open_file(BLOCK_16_PATH);
    block_32_fd = open_file(BLOCK_32_PATH);
    block_64_fd = open_file(BLOCK_64_PATH);
    block_128_fd = open_file(BLOCK_128_PATH);
    block_256_fd = open_file(BLOCK_256_PATH);
    block_512_fd = open_file(BLOCK_512_PATH);
    block_1024_fd = open_file(BLOCK_1024_PATH);
    block_2048_fd = open_file(BLOCK_2048_PATH);
    block_4096_fd = open_file(BLOCK_4096_PATH);


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
    close(blank_fd);        //空白快
    close(block_2_fd);      //2方块
    close(block_4_fd);      //4方块
    close(block_8_fd);      //...
    close(block_16_fd);
    close(block_32_fd);
    close(block_64_fd);
    close(block_128_fd);
    close(block_256_fd);
    close(block_512_fd);
    close(block_1024_fd);
    close(block_2048_fd);
    close(block_4096_fd);

    return 0;
}

//=======屏幕操作=======//

//显示单张800*480bmp图片(默认图片已打开)
int fb_show_picture(int picture_fd)
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
int local_picture(int picture_fd, int x_start, int y_start, int x_size, int y_size)//文件描述符，起始地址，图片大小
{
    //参数不合理
    if(x_start + x_size > 800 || y_start + y_size > 480)
    {
        return -1;
    }

    //跳过文件头
    lseek(picture_fd, 54, SEEK_SET);

    //把文件像素点内容写到缓存
    unsigned char buff_char[x_size * y_size * 3];
    read(picture_fd, buff_char, sizeof(buff_char));

    //将文件像素点写入缓存
    unsigned int buff_int[x_size * y_size];
    int i, j;
    for(i = 0; i < x_size*y_size; i++)
    {
        //写入一个像素点
        buff_int[i] = buff_char[i*3+0] | buff_char[i*3+1] << 8 | buff_char[i*3+2] << 16;  
    }

    //倒着将像素点写入虚拟内存的第一块
    for(i = 0; i < y_size; i++)
    {
        for(j = 0; j < x_size; j++)
        {
            *(m_map + (i+y_start)*800 + (j+x_start)) = buff_int[(y_size-i-1) * x_size + j];
        }
    }
    


    return 0;
}

//在指定坐标点（比如（0，2））显示97*90的图片
int local_show_96_90(int picture_fd, int x, int y)
{
    return local_picture(picture_fd, 206+y*95, 109+x*87, 96, 90);
}

//显示开始界面
int show_start()
{
    return fb_show_picture(start_fd);
}

//显示难度界面
int show_choose()
{
    return fb_show_picture(choose_fd);
}

//显示简单界面
int show_play_256()
{
    return fb_show_picture(play_256_fd);
}

//显示一般界面
int show_play_2048()
{
    return fb_show_picture(play_2048_fd);
}

//显示困难界面
int show_play_4096()
{
    return fb_show_picture(play_4096_fd);
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
//游戏界面(退出：-1，返回:0，新游戏：1)
int play_interface(int mode);

//难度选择界面(0:返回，1：简单，2：中等，3：困难)
int choose_interface();

//开始界面(-1:退出游戏，1：开始游戏)
int start_interface();

//---------函数---------//



int main(void)
{
    //随机数种子
    srand((unsigned int)time(NULL));

    //打开设备文件
    sys_init();

    int rt = 0;
    
    //开始游戏
    rt = start_interface();
    
    if(rt == -1)//退出游戏
    {
        //黑屏
        show_color(BLACK);
    }

    //关闭系统
    sys_close();

    return 0;
}

//开始界面(-1:退出游戏，1：开始游戏)
int start_interface()
{
    int rt = 0;
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
                    //进入难度选择界面
                    printf("进入难度选择界面！\n");
                    rt = choose_interface();
                    if(rt == 0)//表示返回
                    {
                        //显示界面
                        printf("返回开始界面!\n");
                        show_start();
                    }
                    else if(rt == -1)//退出
                    {
                        return -1;
                    }
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
    return 10;
}

//难度选择界面(0:返回，1：简单，2：中等，3：困难)
int choose_interface()
{
    int rt = 0;
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
                //返回
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
                    //进入游戏简单界面
                    rt = play_interface(1);
                    if(rt == 0)//返回当前界面
                    {
                        //显示界面
                        printf("返回难度选择界面！\n");
                        show_choose();
                    }
                    else if(rt == -1)//退出游戏
                    {
                        //退出
                        return -1;
                    }
                }
                else if(ts_choose_general(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    rt = play_interface(2);
                    if(rt == 0)//返回当前界面
                    {
                        //显示界面
                        printf("返回难度选择界面！\n");
                        show_choose();
                    }
                    else if(rt == -1)//退出游戏
                    {
                        //退出
                        return -1;
                    }
                }
                else if(ts_choose_diff(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    rt = play_interface(3);
                    if(rt == 0)//返回当前界面
                    {
                        //显示界面
                        show_choose();
                        printf("返回难度选择界面！\n");
                    }
                    else if(rt == -1)//退出游戏
                    {
                        //退出
                        return -1;
                    }
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
    return 10;
}

//游戏界面(退出：-1，返回:0，新游戏：1)
int play_interface(int mode)
{
    //显示图片
    if(mode == 1)
    {
        fb_show_picture(play_256_fd);
    }
    else if(mode == 2)
    {
        fb_show_picture(play_2048_fd);
    }
    else if(mode == 3)
    {
        fb_show_picture(play_4096_fd);
    }

    //任意生成两个2
    init_play();

    // //显示矩阵
    draw_matrix();
    
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
                    printf("退出游戏！\n");
                    return -1;
                }
                //返回
                else if(ts_play_back(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 0;
                }
                //新游戏
                else if(ts_play_new(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    //显示图片
                    if(mode == 1)
                    {
                        fb_show_picture(play_256_fd);
                    }
                    else if(mode == 2)
                    {
                        fb_show_picture(play_2048_fd);
                    }
                    else if(mode == 3)
                    {
                        fb_show_picture(play_4096_fd);
                    }
                }
                break;

            case 1://左划显示黑色
                if(turn_left(mode) == -1)
                {
                    return -1;
                }
                break;
            
            case 2://右划显示白色
                if(turn_right(mode) == -1)
                {
                    return -1;
                }
                break;
            
            case 3://上划显示红色
                if(turn_up(mode) == -1)
                {
                    return -1;
                }
                break;
            
            case 4://下划显示绿色
                if(turn_down(mode) == -1)
                {
                    return -1;
                }
                break;
            
            default:
                break;
            }//switch

            //当次操作结束，重置flag值，使之能够记录下次操作的开始值
            x_flag = 1;
            y_flag = 1;
        }//if
    }//while(1)
    return 10;
}