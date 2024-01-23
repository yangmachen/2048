//包含头文件
#include "all.h"

//函数定义
//----------界面操作处理----------//

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
                    x_flag = 1;
                    y_flag = 1;
                    rt = choose_interface();
                    if(rt == 0)//表示返回
                    {
                        //显示界面
                        printf("返回开始界面!\n");
                        x_flag = 1;
                        y_flag = 1;
                        show_start();
                    }
                    else if(rt == -1)//退出
                    {
                        x_flag = 1;
                        y_flag = 1;
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
    fb_show_picture(nandu_fd);
    sleep(2);
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
                        x_flag = 1;
                        y_flag = 1;
                        show_choose();
                    }
                    else if(rt == -1)//退出游戏
                    {
                        x_flag = 1;
                        y_flag = 1;
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
                        x_flag = 1;
                        y_flag = 1;
                        show_choose();
                    }
                    else if(rt == -1)//退出游戏
                    {
                        x_flag = 1;
                        y_flag = 1;
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
                        x_flag = 1;
                        y_flag = 1;
                        show_choose();
                        printf("返回难度选择界面！\n");
                    }
                    else if(rt == -1)//退出游戏
                    {
                        x_flag = 1;
                        y_flag = 1;
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
    game_over = 0;
    int rt = 0;
    //进入界面
    if(mode == 1)
    {
        fb_show_picture(_256_fd);
    }
    else if(mode == 2)
    {
        fb_show_picture(_2048_fd);
    }
    else if(mode == 3)
    {
        fb_show_picture(_4096_fd);
    }
    sleep(1);
    
    //游戏初始化
    init_play(mode);
    
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
                if(diao_ctl == 1)
                {
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
                    diao_ctl = 0;
                    draw_matrix();
                    x_flag = 1;
                    y_flag = 1;
                }
                //判断是否退出游戏
                else if(ts_play_quit(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    printf("退出游戏！\n");
                    return -1;
                }
                //返回
                else if(ts_play_back(x_start, y_start))
                {
                    game_over = 0;
                    x_flag = 1;
                    y_flag = 1;
                    return 0;
                }
                //新游戏
                else if(ts_play_new(x_start, y_start))
                {
                    game_over = 0;
                    x_flag = 1;
                    y_flag = 1;
                    init_play(mode);
                }
                //别慌
                else if(ts_play_dont_panic(x_start, y_start))
                {
                    printf("别慌，蛇形排序！\n");
                    if(biehuang_num < 3)
                    {
                        //别慌，柳暗花明又一村
                        fb_show_picture(biehuang_fd);
                        usleep(500000);
                        fb_show_main_picture(liu_fd, BLACK);
                        usleep(500000);
                        fb_show_main_picture(an_fd, BLACK);
                        usleep(500000);
                        fb_show_main_picture(hua_fd, BLACK);
                        usleep(500000);
                        fb_show_main_picture(ming_fd, BLACK);
                        usleep(500000);
                        fb_show_main_picture(you_fd, BLACK);
                        usleep(500000);         
                        fb_show_main_picture(yi_fd, BLACK);
                        usleep(500000);
                        fb_show_main_picture(cun_fd, BLACK);
                        usleep(500000);
                        usleep(500000);                                          
                        //蛇形排序
                        matrix_sort_snake();
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

                        //显示矩阵
                        draw_matrix();
                        biehuang_num++;
                    }//if
                    else
                    {
                        fb_show_picture(biedianle_fd);
                        sleep(1);
                        fb_show_main_picture(meigua_fd, BLACK);
                        sleep(1);
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

                        //显示矩阵
                        draw_matrix();
                    }
                }
                //稳了
                else if(ts_play_has_steady(x_start, y_start))
                {
                    printf("稳了？阵法拆散！\n");
                    if(wenle_num < 3)
                    {
                        fb_show_picture(wenle_fd);
                        sleep(1);         
                        fb_show_main_picture(daxiang_fd, BLACK);
                        sleep(1);
                        fb_show_main_picture(caisan_fd, BLACK);
                        sleep(1);      

                        //拆散矩阵
                        matrix_break();
                        
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

                        //显示矩阵
                        draw_matrix();
                        wenle_num++;
                    }
                    else
                    {
                        fb_show_picture(biedianle_fd);
                        sleep(1);
                        fb_show_main_picture(meigua_fd, BLACK);
                        sleep(1);
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

                        //显示矩阵
                        draw_matrix();
                    }
                }
                //命运之骰
                else if(ts_play_dice_of_destiny(x_start, y_start))
                {
                    printf("命运之骰！\n");
                    if(touzi_num < 3)
                    {
                        //任意出现一个数
                        get_rand_num(mode);
                        draw_matrix();
                        touzi_num++;
                    }
                    else
                    {
                        fb_show_picture(biedianle_fd);
                        sleep(1);
                        fb_show_main_picture(meigua_fd, BLACK);
                        sleep(1);
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

                        //显示矩阵
                        draw_matrix();
                    }
                    
                }
                //骰子
                else if(ts_play_dice(x_start, y_start))
                {
                    printf("骰子！\n");
                    if(touzi_num < 3)
                    {
                        //任意出现一个数
                        get_rand_num(mode);
                        draw_matrix();
                        touzi_num++;
                    }
                    else
                    {
                        fb_show_picture(biedianle_fd);
                        sleep(1);
                        fb_show_main_picture(meigua_fd, BLACK);
                        sleep(1);
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

                        //显示矩阵
                        draw_matrix();
                    }
                }
                break;

            case 1://左划
                rt = turn_left(mode);
                if(rt == -1 && can_move() == -1)//失败
                {
                    printf("挑战失败！\n");
                    x_flag = 1;
                    y_flag = 1;
                    //进入失败界面
                    rt = defeat_interface();
                    //返回当前界面
                    if(rt == 0)
                    {
                        printf("返回游戏界面！\n");
                        x_flag = 1;
                        y_flag = 1;
                        //初始化
                        init_play(mode);
                        break;
                    }
                    //退出游戏
                    if(rt == -1)
                    {
                        x_flag = 1;
                        y_flag = 1;
                        return -1;
                    }
                }
                //胜利
                else if(rt == 1)
                {
                    printf("挑战成功！\n");
                    //进入成功界面
                    x_flag = 1;
                    y_flag = 1;
                    rt = win_interface();
                    if(rt == 0)
                    {
                        game_over = 1;
                        printf("继续游戏！\n");
                        win_back(mode);
                    }
                    
                }
                break;
            
            case 2://右划
                rt = turn_right(mode);
                if(rt == -1 && can_move() == -1)//失败
                {
                    printf("挑战失败！\n");
                    //进入失败界面
                    x_flag = 1;
                    y_flag = 1;
                    rt = defeat_interface();
                    //返回当前界面
                    if(rt == 0)
                    {
                        x_flag = 1;
                        y_flag = 1;
                        printf("返回游戏界面！\n");
                        //初始化
                        init_play(mode);
                        break;
                    }
                    //退出游戏
                    if(rt == -1)
                    {
                        x_flag = 1;
                        y_flag = 1;
                        return -1;
                    }
                }
                //胜利
                else if(rt == 1)
                {
                    printf("挑战成功！\n");
                    //进入成功界面
                    x_flag = 1;
                    y_flag = 1;
                    rt = win_interface();
                    if(rt == 0)
                    {
                        game_over = 1;
                        printf("继续游戏！\n");
                        win_back(mode);
                    }
                }
                break;
            
            case 3://上划
                rt = turn_up(mode);
                if(rt == -1 && can_move() == -1)//失败
                {
                    printf("挑战失败！\n");
                    x_flag = 1;
                    y_flag = 1;
                    //进入失败界面
                    rt = defeat_interface();
                    //返回当前界面
                    if(rt == 0)
                    {
                        printf("返回游戏界面！\n");
                        x_flag = 1;
                        y_flag = 1;
                        //初始化
                        init_play(mode);
                        break;
                    }
                    //退出游戏
                    if(rt == -1)
                    {
                        x_flag = 1;
                        y_flag = 1;
                        return -1;
                    }
                }
                //胜利
                else if(rt == 1)
                {
                    printf("挑战成功！\n");
                    x_flag = 1;
                    y_flag = 1;
                    //进入成功界面
                    rt = win_interface();
                    if(rt == 0)
                    {
                        game_over = 1;
                        printf("继续游戏！\n");
                        win_back(mode);
                    }
                }
                break;
            
            case 4://下划
                rt = turn_down(mode);
                if(rt == -1 && can_move() == -1)//失败
                {
                    printf("挑战失败！\n");
                    x_flag = 1;
                    y_flag = 1;
                    //进入失败界面
                    rt = defeat_interface();
                    //返回当前界面
                    if(rt == 0)
                    {
                        printf("返回游戏界面！\n");
                        x_flag = 1;
                        y_flag = 1;
                        //初始化
                        init_play(mode);
                        break;
                    }
                    //退出游戏
                    if(rt == -1)
                    {
                        x_flag = 1;
                        y_flag = 1;
                        return -1;
                    }
                }
                //胜利
                else if(rt == 1)
                {
                    printf("挑战成功！\n");
                    x_flag = 1;
                    y_flag = 1;
                    //进入成功界面
                    rt = win_interface();
                    if(rt == 0)
                    {
                        game_over = 1;
                        x_flag = 1;
                        y_flag = 1;
                        printf("继续游戏！\n");
                        win_back(mode);
                    }
                    
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

//失败界面（0：返回，-1：退出游戏）
int defeat_interface()
{
    int rt = 0;
    //显示界面
    fb_show_main_picture(lose_fd, BLACK);
    
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
                if(ts_lose_back(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 0;
                }
                //退出游戏
                else if(ts_lose_quit(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return -1;
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

//胜利界面(继续游戏：0)
int win_interface()
{
    int rt = 0;
    //显示界面
    fb_show_main_picture(win_fd, BLACK);
    
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
                //继续游戏
                if(ts_win_continue(x_start, y_start))
                {
                    x_flag = 1;
                    y_flag = 1;
                    return 0;
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

//胜利后返回游戏
int win_back(int mode)
{
    //显示背景
    if(mode == 1)
    {
        show_play_256();
    }
    else if(mode == 2)
    {
        show_play_2048();
    }
    else if(mode == 3)
    {
        show_play_4096();
    }

    //输出矩阵图形
    draw_matrix();

    x_flag = 1;
    y_flag = 1;
    return 0;
}