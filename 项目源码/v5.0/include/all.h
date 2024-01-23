#ifndef __ALL_H
#define __ALL_H


//头文件
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

//宏定义
//---------宏定义---------//
//文件路径
#define FB_PATH "/dev/fb0"          //屏幕文件路径
#define TS_PATH "/dev/input/event0" //触摸屏文件路径

//图片文件路径
#define START_PATH          "start.bmp"         //开始界面
#define CHOOSE_PATH         "choose.bmp"        //困难选择模式 
#define PLAY_256_PATH       "play_256.bmp"      //简单模式
#define PLAY_2048_PATH      "play_2048.bmp"     //一般模式
#define PLAY_4096_PATH      "play_4096.bmp"     //困难模式

#define BLANK_PATH          "blank.bmp"         //无数字方块
#define BLOCK_2_PATH        "block_2.bmp"       //2方块
#define BLOCK_4_PATH        "block_4.bmp"       //4方块
#define BLOCK_8_PATH        "block_8.bmp"       //8方块
#define BLOCK_16_PATH       "block_16.bmp"      //16方块
#define BLOCK_32_PATH       "block_32.bmp"      //32方块
#define BLOCK_64_PATH       "block_64.bmp"      //64方块
#define BLOCK_128_PATH      "block_128.bmp"     //128方块
#define BLOCK_256_PATH      "block_256.bmp"     //256方块
#define BLOCK_512_PATH      "block_512.bmp"     //512方块
#define BLOCK_1024_PATH     "block_1024.bmp"    //1024方块
#define BLOCK_2048_PATH     "block_2048.bmp"    //2048方块
#define BLOCK_4096_PATH     "block_4096.bmp"    //4096方块

#define YUEQIAN_PATH        "yueqian.bmp"
#define NANDU_PATH          "nandu.bmp"
#define _256_PATH           "256.bmp"
#define _2048_PATH          "2048.bmp"
#define _4096_PATH          "4096.bmp"
#define BIEHUANG_PATH       "biehuang.bmp"
#define LIU_PATH            "liu.bmp"
#define AN_PATH             "an.bmp"
#define HUA_PATH            "hua.bmp"
#define MING_PATH           "ming.bmp"
#define YOU_PATH            "you.bmp"
#define YI_PATH             "yi.bmp"
#define CUN_PATH            "cun.bmp"
#define WENLE_PATH          "wenle.bmp"
#define DAXIANG_PATH        "daxiang.bmp"
#define CAISAN_PATH         "caisan.bmp"
#define BIEDIANLE_PATH      "biedianle.bmp"
#define MEIGUA_PATH         "meigua.bmp"

#define DIAO_PATH           "diao.bmp"
#define DIAO2_PATH          "diao2.bmp"
#define DIAO3_PATH          "diao3.bmp"



#define WIN_PATH            "win.bmp"           //胜利
#define LOSE_PATH           "lose.bmp"          //失败
#define QUIT_PATH           "quit.bmp"          //退出游戏

//颜色
#define RED     0xff0000
#define GREEN   0x00ff00
#define BLUE    0x0000ff
#define BLACK   0x000000
#define WHITE   0xffffff
#define YELLOW  0xffff00
#define PURPLE  0xff00ff
#define CYAN    0x00ffff

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
int matrix[SIZE][SIZE];

//---------结构体---------//

//---------全局变量---------//
int fb_fd;                  //屏幕文件描述符
int ts_fd;                  //触摸屏文件描述符

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

int yueqian_fd;
int nandu_fd;
int _256_fd;
int _2048_fd;
int _4096_fd;
int biehuang_fd;
int liu_fd;
int an_fd;
int hua_fd;
int ming_fd;
int you_fd;
int yi_fd;
int cun_fd;
int wenle_fd;
int daxiang_fd;
int caisan_fd;
int biedianle_fd;
int meigua_fd;

int diao_ctl;
int diao_fd;
int diao2_fd;
int diao3_fd;


int win_fd;         //胜利
int lose_fd;        //失败
int quit_fd;        //退出游戏

int game_over;  //值为1时不检测是否挑战成功

unsigned int *m_map;     //屏幕映像指针

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
int x_start;
int y_start;
int x_end;
int y_end;
int x_flag;
int y_flag;

//开挂次数
int biehuang_num;
int wenle_num;
int touzi_num;

//---------全局变量---------//


//---------函数---------//

//---------矩阵运算---------//

//在0的位置任意出现数字2/4，出现2的概率能够在宏定义中控制(失败返回-1)
int random_2();

//任意出现两个2
int random_2_2();

//在0的位置任意出现数字2-4096(失败返回-1)
int get_rand_num(int mode);

//游戏初始化
int init_play(int mode);

//通过矩阵画出图形
int draw_matrix();

//寻找矩阵中是否有某一数字(有：0，没有：-1)
int find_num(int num);

//判断矩阵是否已满（未满：0，已满：-1）
int matrix_is_full();

//矩阵上划(移动：0，没移动：-1)
int matrix_up();

//矩阵下划(移动：0，没移动：-1)
int matrix_down();

//矩阵左划(移动：0，没移动：-1)
int matrix_left();

//矩阵右划(移动：0，没移动：-1)
int matrix_right();

//矩阵赋值（SIZE*SIZE）
int matrix_copy(int (*dest)[SIZE], int (*src)[SIZE]);

//------------------添加--------------------//

//矩阵排序
int matrix_sort();

//矩阵蛇形排序
int matrix_sort_snake();

//矩阵拆散
int matrix_break();

//单击别慌
int ts_play_dont_panic(int x_real, int y_real);

//单击稳了
int ts_play_has_steady(int x_real, int y_real);

//单击命运之骰
int ts_play_dice_of_destiny(int x_real, int y_real);

//单击骰子
int ts_play_dice(int x_real, int y_real);

//------------------添加--------------------//

//判断当前矩阵是否能够移动（能移动：0，不能：-1）
int can_move();

//上划(-1:失败 1：胜利 0：继续游戏)
int turn_up(int mode);//-1:失败 1：胜利 0：继续游戏

//下划(-1:失败 1：胜利 0：继续游戏)
int turn_down(int mode);//-1:失败 1：胜利 0：继续游戏

//左划(-1:失败 1：胜利 0：继续游戏)
int turn_left(int mode);

//右划(-1:失败 1：胜利 0：继续游戏)
int turn_right(int mode);//-1:失败 1：胜利 0：继续游戏

//----------系统-----------//

//系统初始化：打开屏幕和触摸屏，构建映射

//可读可写打开文件
int open_file(char *path);

//系统初始化
int sys_init();


//退出系统
int sys_close();

//=======屏幕操作=======//

//显示单张800*480bmp图片(默认图片已打开)
int fb_show_picture(int picture_fd);

//显示一张800*480的照片不显示底色
int fb_show_main_picture(int picture_fd, int color);

//显示全屏同种颜色
int show_color(int color);

//将图片定位到指定位置
int local_picture(int picture_fd, int x_start, int y_start, int x_size, int y_size);//文件描述符，起始地址，图片大小

//在指定坐标点（比如（0，2））显示97*90的图片
int local_show_96_90(int picture_fd, int x, int y);

//显示开始界面
int show_start();

//显示难度界面
int show_choose();

//显示简单界面
int show_play_256();

//显示一般界面
int show_play_2048();

//显示困难界面
int show_play_4096();

//切换虚拟界面
int change_interface(int offset);



//------触摸屏操作------//

//判断左右划和上下划(0:点击 1:左划 2：右划 3：上划 4：下划)
int judgment(int x_start, int y_start, int x_end, int y_end);//参数为 开始按下的坐标 和 松开时候的坐标

//检测是否单击指定位置(是：1， 不是：0)
int ts_trigger(int x_start, int y_start, int x_end, int y_end, int x_real, int y_real);//参数：起始坐标，结束坐标，单击坐标

//开始界面
//单击开始游戏
int ts_get_start(int x_real, int y_real);

//单击退出游戏
int ts_get_quit(int x_real, int y_real);

//难度选择界面
//单击选择界面的返回
int ts_choose_back(int x_real, int y_real);

//单击简单模式
int ts_choose_easy(int x_real, int y_real);

//单击一般模式
int ts_choose_general(int x_real, int y_real);

//单击困难模式
int ts_choose_diff(int x_real, int y_real);

//胜利——继续游戏
int ts_win_continue(int x_real, int y_real);

//失败——单击返回
int ts_lose_back(int x_real, int y_real);

//失败——单击退出游戏
int ts_lose_quit(int x_real, int y_real);

//游戏界面
//单击返回
int ts_play_back(int x_real, int y_real);

//单击退出游戏
int ts_play_quit(int x_real, int y_real);

//单击新游戏
int ts_play_new(int x_real, int y_real);

//----------界面操作处理----------//
//游戏界面(退出：-1，返回:0，新游戏：1)
int play_interface(int mode);

//难度选择界面(0:返回，1：简单，2：中等，3：困难)
int choose_interface();

//开始界面(-1:退出游戏，1：开始游戏)
int start_interface();

//失败界面
int defeat_interface();

//胜利界面
int win_interface();

//胜利后返回游戏
int win_back(int mode);

//---------函数---------//

#endif