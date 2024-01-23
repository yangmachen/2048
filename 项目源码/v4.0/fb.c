//包含头文件
#include "all.h"


//函数定义
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

//显示一张800*480的照片不显示底色
int fb_show_main_picture(int picture_fd, int color)
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
            if(buff_int[(480-i) * 800 + j] != color)
            {
                *(m_map + i*800 + j) = buff_int[(480-i) * 800 + j];
            }
            
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
