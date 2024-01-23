#include <stdio.h>
#include <dlfcn.h>  // ��̬���ض�̬���ͷ�ļ���dlopen()��dlsym()
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

// BMP��ʽͷ�淶
struct bitmap_header
{
	int16_t type;
	int32_t size; // ͼ���ļ���С
	int16_t reserved1;
	int16_t reserved2;
	int32_t offbits; // bmpͼ������ƫ����
}__attribute__((packed));

struct bitmap_info
{
	int32_t size;   // ���ṹ��С	
	int32_t width;  // ͼ���
	int32_t height; // ͼ���
	int16_t planes;

	int16_t bit_count; // ɫ��
	int32_t compression;
	int32_t size_img; // bmp���ݴ�С��������4��������
	int32_t X_pel;
	int32_t Y_pel;
	int32_t clrused;
	int32_t clrImportant;
}__attribute__((packed));

// ���½ṹ�岻һ��������BMP�ļ��У����ǣ�
// bitmap_info.compressionΪ��
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
    //��file.txt�ļ�, �ļ������ڣ����ʧ�ܣ����ʹ��O_CREAT���Ǳ���Ҫ����ļ�Ȩ�ޡ�
    lcd_fd = open(FB_FILE, O_RDWR);
    if(lcd_fd == -1)
    {
        printf("open a.txt fail\n");
        return -1;
    }   

    //��Ļӳ��
    mem_p = (unsigned int *)mmap(NULL, 800*480*4, PROT_READ|PROT_WRITE, MAP_SHARED, lcd_fd, 0);
    if(mem_p == MAP_FAILED)
    {
        printf("mmap fail\n");
    }

    return 0;
}
int lcd_uninit(void)
{
   // ���ӳ��
    munmap(mem_p, 800*480*4);
    close(lcd_fd);

}
/**************************************
����˵��
�������ܣ�����Ļ��ʾͼƬ

������
const char *pathname��ͼƬ·�� 
int start_x�� ͼƬX���������
int start_y�� ͼƬY���������

**************************************/
int show_bmp(const char *pathname, int start_x, int start_y)
{
    int width, high; //ͼƬ������߶�
    int i, j, x, y;
	// ��ȡBMP��ʽͷ����ȡͼƬ��Ϣ
	struct bitmap_header header;
	struct bitmap_info info;
	struct rgb_quad quad;

	int bmp_fd = open(pathname, O_RDONLY);
    if(bmp_fd == -1)
    {
        printf("open bmp fail\n");
        return -1;
    }
    // ��һ�������ṹ���Ǳ�����Ϣ
	read(bmp_fd, &header, sizeof(header));
	read(bmp_fd, &info, sizeof(info));

    width = info.width;
    high  = info.height;

    if(start_x+width >= 800 || start_y+high >=480)
    {
        printf("ͼƬ��ʾ�����\n");
        close(bmp_fd);
        return -1;
    }
    //�䳤����
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
    //��ʾ����1
    for( y = start_y; y < (start_y+high); y++)
    {
        //x��
        for( x = start_x; x< (start_x+width); x++)
        {
            // buff[(high+start_y - 1 -y)*800+  (x- start_x) ]; ��������ݷ�Χ��0��width*high
            *(mem_p+y*800+x) =   buff[(high+start_y - 1 -y)*width+  (x- start_x) ];                        
        }
    }
*/

    //ͼƬ���ص㵹��
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
