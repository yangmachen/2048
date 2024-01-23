//包含头文件
#include "all.h"

// //---------全局变量---------//
// extern int start_fd;
// extern int choose_fd;    
// extern int play_256_fd;
// extern int play_2048_fd;
// extern int play_4096_fd;
// extern int blank_fd;
// extern int block_2_fd;
// extern int block_4_fd;
// extern int block_8_fd;
// extern int block_16_fd;
// extern int block_32_fd;
// extern int block_64_fd;
// extern int block_128_fd;
// extern int block_256_fd;
// extern int block_512_fd;
// extern int block_1024_fd;
// extern int block_2048_fd;
// extern int block_4096_fd;
// extern int win_fd;
// extern int lose_fd;
// extern int quit_fd;

// extern struct fb_fix_screeninfo finfo; //固定参数
// extern struct fb_var_screeninfo vinfo; //可变参数

// extern int v_width;    //屏幕虚拟区的宽
// extern int v_height;   //屏幕虚拟区的高
// extern int v_bpp;      //一个像素占的位数

// extern unsigned int *m_map = NULL;     //屏幕映像指针

//函数定义
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
    win_fd = open_file(WIN_PATH);               //胜利
    lose_fd = open_file(LOSE_PATH);             //失败
    quit_fd = open_file(QUIT_PATH);             //退出游戏
    yueqian_fd = open_file(YUEQIAN_PATH);
    nandu_fd = open_file(NANDU_PATH);
    _256_fd = open_file(_256_PATH);
    _2048_fd = open_file(_2048_PATH);
    _4096_fd = open_file(_4096_PATH);
    biehuang_fd = open_file(BIEHUANG_PATH);
    liu_fd = open_file(LIU_PATH);
    an_fd = open_file(AN_PATH);
    hua_fd = open_file(HUA_PATH);
    ming_fd = open_file(MING_PATH);
    you_fd = open_file(YOU_PATH);
    yi_fd = open_file(YI_PATH);
    cun_fd = open_file(CUN_PATH);
    wenle_fd = open_file(WENLE_PATH);
    daxiang_fd = open_file(DAXIANG_PATH);
    caisan_fd = open_file(CAISAN_PATH);
    biedianle_fd = open_file(BIEDIANLE_PATH);
    meigua_fd = open_file(MEIGUA_PATH);

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
    close(win_fd);          //胜利
    close(lose_fd);         //失败
    close(quit_fd);         //退出游戏

    close(yueqian_fd);
    close(nandu_fd);
    close(_256_fd);
    close(_2048_fd);
    close(_4096_fd);
    close(biehuang_fd);
    close(liu_fd);
    close(an_fd);
    close(hua_fd);
    close(ming_fd);
    close(you_fd);
    close(yi_fd);
    close(cun_fd);
    close(wenle_fd);
    close(daxiang_fd);
    close(caisan_fd);
    close(biedianle_fd);
    close(meigua_fd);

    return 0;
}
