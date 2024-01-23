//包含头文件
#include "all.h"


//函数定义
//---------函数---------//

//---------矩阵运算---------//

//在0的位置任意出现数字2/4，出现2的概率能够在宏定义中控制(失败返回-1)
int random_2()
{
    //计算当次出现2还是4
    int num = 0;
    if((rand() % 100 + 1) <= chance_of_2)
    {
        num = 2;
    }
    else
    {
        num = 4;
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

//----------------------添加---------------------------//
//在0的位置任意出现数字2-128或2-1024或2-2048(失败返回-1)
int get_rand_num(int mode)
{
    int max_num = 0;
    if(mode == 1)
    {
        max_num = 7;
    }
    else if(mode == 2)
    {
        max_num = 10;
    }
    else if(mode == 3)
    {
        max_num = 11;
    }
    else
    {
        return -1;
    }
    int i, j;
    //计算当次出现2还是4
    int num = 1;
    int rand_n = rand()%max_num + 1;   //生成1-12
    // num = pow(2, rand_n);
    for(i = 0; i < rand_n; i++)
    {
        num *= 2;
    }
    //计算0的数量
    int count_0 = 0;
    

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

//----------------------添加---------------------------//

//任意出现两个2
int random_2_2()
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

//游戏初始化
int init_play(int mode)
{
    game_over = 0;
    //矩阵清空
    int i, j;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            matrix[i][j] = 0;
        }
    }

    //显示图片
    if(mode == 1)
    {
        biehuang_num = 2;
        wenle_num = 2;
        touzi_num = 2;
        fb_show_picture(play_256_fd);
    }
    else if(mode == 2)
    {
        biehuang_num = 1;
        wenle_num = 1;
        touzi_num = 1;
        fb_show_picture(play_2048_fd);
    }
    else if(mode == 3)
    {
        biehuang_num = 0;
        wenle_num = 0;
        touzi_num = 0;
        fb_show_picture(play_4096_fd);
    }

    //任意生成两个2
    random_2_2();

    //显示矩阵
    draw_matrix();
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
                matrix[i][j] = 0;
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

//矩阵上划(移动：0，没移动：-1)
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

//矩阵下划(移动：0，没移动：-1)
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

//矩阵左划(移动：0，没移动：-1)
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

//矩阵右划(移动：0，没移动：-1)
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

//矩阵复制（SIZE*SIZE）
int matrix_copy(int (*dest)[SIZE], int (*src)[SIZE])
{
    int i, j;
    for(i = 0; i < SIZE; i++)
    {
        for(j = 0; j < SIZE; j++)
        {
            dest[i][j] = src[i][j];
        }
    }

    return 0;
}

//------------------添加--------------------//

//矩阵排序
int matrix_sort()
{
    //冒泡排序
    int i, j;
    int flag = 0;
    for(i = 0; i < SIZE*SIZE - 1; i++)  //
    {
        flag = 0;
        for(j = 0; j < SIZE*SIZE - 1 - i; j++)
        {
            if(matrix[0][j] < matrix[0][j+1])//小 大
            {
                flag = 1;
                matrix[0][j] = matrix[0][j] ^ matrix[0][j+1];
                matrix[0][j+1] = matrix[0][j] ^ matrix[0][j+1];
                matrix[0][j] = matrix[0][j] ^ matrix[0][j+1];
            }
        }
        if(flag == 0)
        {
            break;
        }
    }
    return 0;
}

//矩阵蛇形排序(从大到小)
int matrix_sort_snake()
{
    //冒泡排序
    int i, j;
    matrix_sort();

    //奇数行翻转
    for(i = 0; i < SIZE; i++)
    {
        if(i % 2 == 1)//计数行
        {
            for(j = 0; j < SIZE/2; j++)
            {
                matrix[i][j] = matrix[i][j] ^ matrix[i][SIZE - 1 -j];
                matrix[i][SIZE - 1 -j] = matrix[i][j] ^ matrix[i][SIZE - 1 -j];
                matrix[i][j] = matrix[i][j] ^ matrix[i][SIZE - 1 -j];
            }
        }
    }

    return 0;
}

//矩阵拆散(实现矩阵的随机排列)
int matrix_break()
{
    //将原矩阵复制到临时矩阵，并把原矩阵清零
    int temp[SIZE][SIZE] = {0};
    matrix_copy(temp, matrix);
    bzero(matrix, sizeof(int)*SIZE*SIZE);

    int i, j;
    int count_0 = SIZE*SIZE;    //当前原矩阵0的个数
    int rand_num = 0;           //随机生成一个位置（随机数表示原矩阵的第几个零）
    for(i = 0; i < SIZE*SIZE; i++)
    {
        if(temp[0][i] != 0) //只将非零元素放入原矩阵
        {
            //生成随机数1-count_0
            rand_num = rand()%count_0 + 1;
            int id_0 = 0;   //记录当前是第几个零，当变量与rand_num相匹配时，说明当前位置就是要放的位置
            for(j = 0; j < SIZE*SIZE; j++)
            {
                if(matrix[0][j] == 0)
                {
                    id_0++;
                    if(id_0 == rand_num)
                    {
                        matrix[0][j] = temp[0][i];
                        count_0--;
                    }//if
                }//if
            }//for(j)
        }//if
    }//for(i)

    return 0;
}

//------------------添加--------------------//

//判断当前矩阵是否能够移动（能移动：0，不能：-1）
int can_move()
{
    int temp[SIZE][SIZE];

    //保存矩阵
    int i, j;
    matrix_copy(temp, matrix);

    //左右上下都不能移动
    if(matrix_left() == -1 && matrix_right() == -1 && matrix_up() == -1 && matrix_down() == -1)
    {
        return -1;
    }

    //还原矩阵
    matrix_copy(matrix, temp);
    return 0;
}

//上划(-1:失败 1：胜利 0：继续游戏)
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
    if(mode == 1 && find_num(256) == 0 && game_over == 0)
    {
        return 1;
    }
    if(mode == 2 && find_num(2048) == 0 && game_over == 0)
    {
        return 1;
    }
    if(mode == 3 && find_num(4096) == 0 && game_over == 0)
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

//下划(-1:失败 1：胜利 0：继续游戏)
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
    if(mode == 1 && find_num(256) == 0 && game_over == 0)
    {
        return 1;
    }
    if(mode == 2 && find_num(2048) == 0 && game_over == 0)
    {
        return 1;
    }
    if(mode == 3 && find_num(4096) == 0 && game_over == 0)
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

//左划(-1:失败 1：胜利 0：继续游戏)
int turn_left(int mode)
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
    if(mode == 1 && find_num(256) == 0 && game_over == 0)
    {
        return 1;
    }
    if(mode == 2 && find_num(2048) == 0 && game_over == 0)
    {
        return 1;
    }
    if(mode == 3 && find_num(4096) == 0 && game_over == 0)
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

//右划(-1:失败 1：胜利 0：继续游戏)
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
    if(mode == 1 && find_num(256) == 0 && game_over == 0)
    {
        return 1;
    }
    if(mode == 2 && find_num(2048) == 0 && game_over == 0)
    {
        return 1;
    }
    if(mode == 3 && find_num(4096) == 0 && game_over == 0)
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
