#include <stdio.h>
#include <dlfcn.h>  // 动态加载动态库的头文件：dlopen()、dlsym()
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>

#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/mman.h>

// BMP格式头规范
struct bitmap_header
{
	int16_t type;
	int32_t size; // 图像文件大小
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits; // bmp图像数据偏移量
}__attribute__((packed));

struct bitmap_info
{
	int32_t size;   // 本结构大小	
	int32_t width;  // 图像宽
	int32_t height; // 图像高
	int16_t planes;

	int16_t bit_count; // 色深
	int32_t compression;
	int32_t size_img; // bmp数据大小，必须是4的整数倍
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
}__attribute__((packed));

// 以下结构体不一定存在于BMP文件中，除非：
// bitmap_info.compression为真
struct rgb_quad
{
	int8_t blue;
	int8_t green;
	int8_t red;
	int8_t reserved;
}__attribute__((packed));


#define FB_FILE  "/dev/fb0"

unsigned int *mem_p;


int lcd_fd;

int lcd_init(void);
int lcd_uninit(void);
int show_bmp(const char *pathname, int start_x, int start_y);



int main(void)
{
    lcd_init();
    show_bmp("63.bmp", 200, 100);

    lcd_uninit();
    return 0;
}


int lcd_init(void)
{
    //打开file.txt文件, 文件不存在，则打开失败，如果使用O_CREAT，那必须要添加文件权限。
    lcd_fd = open(FB_FILE, O_RDWR);
    if(lcd_fd == -1)
    {
        printf("open a.txt fail\n");
        return -1;
    }   

    //屏幕映射
    mem_p = (unsigned int *)mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, lcd_fd, 0);
    if(mem_p == MAP_FAILED)
    {
        printf("mmap fail\n");
    }

    return 0;
}
int lcd_uninit(void)
{
   // 解除映射
    munmap(mem_p, 800*480*4);
    close(lcd_fd);

}
/**************************************
函数说明
函数功能：在屏幕显示图片

参数：
const char *pathname：图片路径 
int start_x： 图片X轴起点坐标
int start_y： 图片Y轴起点坐标

**************************************/
int show_bmp(const char *pathname, int start_x, int start_y)
{
    int width, high; //图片亮度与高度
    int i, j, x, y;
	// 读取BMP格式头，获取图片信息
	struct bitmap_header header;
	struct bitmap_info info;
	struct rgb_quad quad;

	int bmp_fd = open(pathname, O_RDONLY);
    if(bmp_fd == -1)
    {
        printf("open bmp fail\n");
        return -1;
    }
    // 第一、二个结构体是必有信息
	read(bmp_fd, &header, sizeof(header));
	read(bmp_fd, &info, sizeof(info));

    width = info.width;
    high  = info.height;

    if(start_x+width >= 800 || start_y+high >=480)
    {
        printf("图片显示溢出！\n");
        close(bmp_fd);
        return -1;
    }
    //变长数组
    unsigned char bmpbuff[width*high*3];
    unsigned int buff[width*high];
    unsigned int tmpbuff[width*high];


    read(bmp_fd, bmpbuff, sizeof(bmpbuff));

    for(i=0; i<width*high; i++)
    {
        //buff[0] = bmpbuff[0]<<16 | bmpbuff[1]<<8 | bmpbuff[2];
        buff[i] = bmpbuff[3*i+0] | bmpbuff[3*i+1]<<8 | bmpbuff[3*i+2]<<16;
    }

/*
    for(y=0; y<480; y++)
    {
        for(x=0; x<800; x++)
        {
            *(mem_p+y*800+x) = buff[y*800+x];
        }
    }
*/

/*
    //显示方法1
    for( y = start_y; y < (start_y+high); y++)
    {
        //x轴
        for( x = start_x; x< (start_x+width); x++)
        {
            // buff[(high+start_y - 1 -y)*800+  (x- start_x) ]; 里面的数据范围：0～width*high
            *(mem_p+y*800+x) =   buff[(high+start_y - 1 -y)*width+  (x- start_x) ];                        
        }
    }
*/

    //图片像素点倒置
    for(y=0; y<high; y++)
    {
        for(x=0; x<width; x++)
        {
           // tmpbuff[y*width+x] = buff[ (high - 1 -y) * width + x];
            tmpbuff[(high - 1 -y) * width + x] = buff[y*width+x];
        }
    }


    for(y=0; y<high; y++)
    {
        for(x=0; x<width; x++)
        {

           *(mem_p + (start_y+y)*800+start_x + x) = tmpbuff[y*width+x];

        }

    }




    close(bmp_fd);

    return 0;
}
