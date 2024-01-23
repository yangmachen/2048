//包含头文件
#include "all.h"


//函数定义
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
    return ts_trigger(128, 146, 275, 380, x_real, y_real);
}

//单击一般模式
int ts_choose_general(int x_real, int y_real)
{
    return ts_trigger(317, 150, 455, 379, x_real, y_real);
}

//单击困难模式
int ts_choose_diff(int x_real, int y_real)
{
    return ts_trigger(489, 108, 669, 398, x_real, y_real);
}

//胜利——继续游戏
int ts_win_continue(int x_real, int y_real)
{
    return ts_trigger(0, 0, 800, 480, x_real, y_real);//(294, 402, 497, 469, x_real, y_real);
}

//失败——单击返回
int ts_lose_back(int x_real, int y_real)
{
    return ts_trigger(331, 342, 493, 448, x_real, y_real);
}

//失败——单击退出游戏
int ts_lose_quit(int x_real, int y_real)
{
    return ts_trigger(591, 385, 777, 464, x_real, y_real);
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

//------------------添加--------------------//

//单击别慌
int ts_play_dont_panic(int x_real, int y_real)
{
    return ts_trigger(638, 91, 778, 150, x_real, y_real);
}

//单击稳了
int ts_play_has_steady(int x_real, int y_real)
{
    return ts_trigger(642, 185, 766, 251, x_real, y_real);
}

//单击命运之骰
int ts_play_dice_of_destiny(int x_real, int y_real)
{
    return ts_trigger(4, 276, 181, 355, x_real, y_real);
}

//单击骰子
int ts_play_dice(int x_real, int y_real)
{
    return ts_trigger(621, 333, 788, 461, x_real, y_real);
}

//------------------添加--------------------//