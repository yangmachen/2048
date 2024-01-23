#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <linux/input.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


#define FB_FILE  "/dev/fb0"
//触摸屏文件路径
#define TS_DEV "/dev/input/event0"

unsigned int *mem_p;

int ts_fd;
int lcd_fd;


int ts_init(void);
int ts_uninit(void);
int lcd_init(void);
int lcd_uninit(void);
int ts_xy_get(int *x, int *y);
int paddle_ts(void);

int main(void)
{
    int X, Y , ret, i;

    ts_init();
    lcd_init();

    while(1)
    {

        ret = paddle_ts();
        printf("ret:%d\n",ret);
        
        switch (ret)
        {
        case 1:
            for(i=0; i<800*480; i++)
            {
                *(mem_p+i) = 0xFFFF00;
            }

            break;
        case 2:
            for(i=0; i<800*480; i++)
            {
                *(mem_p+i) = 0xFF00FF;
            }         
            break;

        case 3:
            for(i=0; i<800*480; i++)
            {
                *(mem_p+i) = 0x000000;
            }    
            break;

        case 4:
            for(i=0; i<800*480; i++)
            {
                *(mem_p+i) = 0x00FF00;
            }             
            break;

        default:
            break;
        }
    }


    lcd_uninit();
    ts_uninit();


    return 0;
}


int ts_init(void)
{
    ts_fd = open(TS_DEV, O_RDWR);
    if(ts_fd == -1)
    {
        printf("open ts fail\n");
        return -1;
    }

}
int ts_uninit(void)
{
    close(ts_fd);
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


int ts_xy_get(int *x, int *y)
{
    int count = 0;
    struct input_event ts;

    memset(&ts, 0, sizeof(struct input_event));

    while (1)
    {
        //读触摸屏
        read(ts_fd, &ts, sizeof(struct input_event));
        //决断是否为相对坐标类型
        if(ts.type == EV_ABS)
        {
            //判断是否坐标X轴
            if(ts.code == ABS_X)
            {
               // printf("X:%d\n", ts.value);
                *x = ts.value;
                count++;
            }
            //判断是否坐标Y轴
            if(ts.code == ABS_Y)
            {
                *y = ts.value;
                count++;
          
            }
            if(count == 2)
            {
                break;
            }
       
        }


    }

    *x = (*x)*(800.0/1024.0);
    *y = (*y)*(480.0/600.0);

    return 0;
    
}


//1左划 2右划  3上划 4下划  0点击
int paddle_ts(void)
{

    int  x = 0, y = 0, xx = 0, yy = 0;
    struct input_event ts;

    memset(&ts, 0, sizeof(struct input_event));   
    while (1)
    {
        //读触摸屏
        read(ts_fd, &ts, sizeof(struct input_event));
        //决断是否为相对坐标类型
        if(ts.type == EV_ABS && ts.code == ABS_X)
        {

            x = ts.value;
        }
         //决断是否为相对坐标类型
        if(ts.type == EV_ABS && ts.code == ABS_Y)
        {

            y = ts.value;
        }        
        //表示手指按下
        if(ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 1)
        {
            //记录起点坐标
            xx = x;
            yy = y;
        }       

        //表示手指松开
        if(ts.type == EV_KEY && ts.code == BTN_TOUCH && ts.value == 0)
        {
            //坐标换算  起点坐标
            xx = xx*(800.0/1024.0);
            yy = yy*(480.0/600.0);
             //坐标换算  结束坐标
            x = x*(800.0/1024.0);
            y = y*(480.0/600.0);    


            if(abs(xx-x) > abs(yy - y))
            {
                if((xx - x) > 100) 
                {
                    return 1;
                }
                if((x - xx) >= 100) 
                {
                    return 2;
                }

                return 0;

            }   
            else
            {
                if((yy - y) > 50) 
                {
                    return 3;
                }
                if((y - yy) >= 50) 
                {
                    return 4;
                }

                return 0;

            }    

        }   

    }

    return 0;

}