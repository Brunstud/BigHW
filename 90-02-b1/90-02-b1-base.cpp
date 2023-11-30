/* 2252201 胡世成 计科2班 */
/*放内部数组方式实现的各函数*/
#include<iostream>
//#include<iomanip>
//#include<conio.h>
#include<windows.h>
//#include "../include/cmd_console_tools.h"
//#include "../include/com_console.h"
#include"90-02-b1.h"
using namespace std;

/***************************************************************************
  函数名称：num::set_boundary
  功    能：设置类num的初始边界
  输入参数：int x, int y
  返 回 值：无
  说    明：重置map，同时绘制背景板
***************************************************************************/
void num::set_boundary(int x, int y)
{
    boundary.x = x;
    boundary.y = y;
    reset_map();
    line = score = time = 0;
    //ori_paint();
    return;
}

/***************************************************************************
  函数名称：num::reset_map
  功    能：重置map
  输入参数：无
  返 回 值：无
  说    明：类num内置函数
***************************************************************************/
void num::reset_map()
{
    int* pm = map[0];
    for (; pm - map[0] < MAX_HEIGHT * MAX_WIDTH; pm++)
        *pm = 0;
    return;
}

/***************************************************************************
  函数名称：num::set_pos
  功    能：设置类num的position位置
  输入参数：int x, int y
  返 回 值：无
  说    明：
***************************************************************************/
void num::set_pos(int x, int y)
{
    position.x = x;
    position.y = y;
    return;
}

/***************************************************************************
  函数名称：num::set_num
  功    能：将num记录在cur_num中，并通过数组num_shape记录数字形状
  输入参数：int num
  返 回 值：无
  说    明：初始方向为0（正向）
***************************************************************************/
void num::set_num(int num)
{
    cur_num = num;
    direction = 0;
    point* ppoint = num_shape;
    ppoint->x = ppoint->y = end_num;
    for (ppoint++; ppoint - num_shape < NUM_SHAPE; ppoint++)
        ppoint->x = ppoint->y = 0;
    {
        ppoint = num_shape;
        switch (num) {
            case 1: //完整设置1
                for (int iy = -2; iy <= 2; ++ppoint, ++iy) {
                    ppoint->x = 0;
                    ppoint->y = iy;
                }
                break;
            case 0: //完整设置0
                for (int ix = -1; ix <= 0; ++ppoint, ++ix) {
                    ppoint->x = ix;
                    ppoint->y = 2;
                }
                for (int iy = -1; iy <= 1; ++ppoint, ++iy) {
                    ppoint->x = -1;
                    ppoint->y = iy;
                }
            case 7: //完整设置7
                for (int ix = -1; ix <= 0; ++ppoint, ++ix) {
                    ppoint->x = ix;
                    ppoint->y = -2;
                }
            case 4:  //不完整设置4
                for (int iy = -2; iy <= 2; ++ppoint, ++iy) {
                    ppoint->x = 1;
                    ppoint->y = iy;
                }
                break;
            case 2:
            case 3:
            case 5:
            case 6:
            case 8:
            case 9:
                for (int iy = -2; iy <= 2; ++ppoint, iy += 2) {
                    for (int jx = -1; jx <= 1; ++ppoint, ++jx) {
                        ppoint->x = jx;
                        ppoint->y = iy;
                    }
                }
                break;
            default:
                cout << "error:set_num(" << num << ")!";
        }
        switch (num) {
            case 4:
                for (int iy = -2; iy <= 0; ++ppoint, ++iy) {
                    ppoint->x = -1;
                    ppoint->y = iy;
                }
                ppoint->x = ppoint->y = 0;
                ppoint++;
                break;
            case 2:
                ppoint->x = 1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = -1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 3:
                ppoint->x = 1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 5:
                ppoint->x = -1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 6:
                ppoint->x = -1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = -1;
                ppoint->y = 1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 8:
                ppoint->x = -1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = -1;
                ppoint->y = 1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 9:
                ppoint->x = -1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
        }
        ppoint->x = ppoint->y = end_num;//表示该数结束
    }
    return;
}

/***************************************************************************
  函数名称：num::rotate
  功    能：逆时针旋转数字并重新显示
  输入参数：无
  返 回 值：无
  说    明：自带擦除与重新绘制功能,需要先使用paint_num或start_num
***************************************************************************/
void num::rotate()
{
    erase_num();
    int possible = 0;
    while (!possible) {
        possible = 1;
        for (point* ppoint = num_shape; ppoint->x != end_num; ppoint++) {
            int tmp = ppoint->x;
            ppoint->x = -ppoint->y;
            ppoint->y = tmp;
            int x = position.x + ppoint->x;
            int y = position.y + ppoint->y;
            if (possible && (x < 0 || x >= boundary.x || y >= boundary.y || y >= 0 && map[y][x]))
                possible = 0;
        }
        direction = ++direction % 4;
    }
    paint_num();
    return;
}

/***************************************************************************
  函数名称：num::start_num
  功    能：使已经设置的数字从中间开始出现
  输入参数：无
  返 回 值：如果遇到阻塞返回1并结束，否则返回0
  说    明：自带绘制功能，需要先通过set_num或show_nex_num设置好num
***************************************************************************/
int num::start_num()
{
    //set_pos(boundary.x / 2, -2);
    set_pos(rand()%(boundary.x - 2) + 1, -2);
    for (point* ppoint = num_shape; ppoint->x != end_num; ppoint++)
        if (2 == ppoint->y && map[0][position.x + ppoint->x])
            return 1;
    paint_num();
    return 0;
}

/***************************************************************************
  函数名称：num::drop_num
  功    能：使数字下降一格
  输入参数：无
  返 回 值：落到底部返回1,游戏结束返回-1,正常下落返回0
  说    明：自带擦除与重新绘制功能，需要先使用start_num
***************************************************************************/
int num::drop_num()
{
    if (land()) {
        if (over()) {
            //cout << "over!";
            return -1;
        }
        return 1;
    }
    erase_num();
    position.y++;
    paint_num();
    return 0;
}

/***************************************************************************
  函数名称：num::land()
  功    能：判断数字是否已经到底部
  输入参数：无
  返 回 值：已经到底部返回1，否则返回0
  说    明：类num内部函数
***************************************************************************/
int num::land()
{
    point* ppoint = num_shape;
    for (; ppoint->x != end_num; ppoint++) {
        int x = position.x + ppoint->x;
        int y = position.y + ppoint->y;
        if (boundary.y - 1 == y || y + 1 >= 0 && map[y + 1][x]) {
           // cout << "already_land!";
            return 1;
        }
    }
    return 0;
}

/***************************************************************************
  函数名称：num::over()
  功    能：判断该局游戏是否结束
  输入参数：无
  返 回 值：已经结束部返回1，否则返回0
  说    明：类num内部函数，在land()之后进行判断
***************************************************************************/
int num::over()
{
    point* ppoint = num_shape;
    for (; ppoint->x != end_num; ppoint++) {
        if (position.y + ppoint->y < 0)
            return 1;
    }
    return 0;
}

/***************************************************************************
  函数名称：num::record_num()
  功    能：将当前数字及其位置记录在map上
  输入参数：无
  返 回 值：无
  说    明：在land()之后使用
***************************************************************************/
void num::record_num()
{
    point* ppoint = num_shape;
    for (; ppoint->x != end_num; ppoint++) {
        int x = position.x + ppoint->x;
        int y = position.y + ppoint->y;
        map[y][x] = 1 + cur_num;
    }
    return;
}

/***************************************************************************
  函数名称：num::check_line()
  功    能：检查可消除的行数，并调用eliminate()进行消除
  输入参数：无
  返 回 值：当前消除行数
  说    明：在record_num()之后使用
***************************************************************************/
int num::check_line()
{
    int line = 0;
    int(*pmap)[MAX_WIDTH] = map;
    int* ppoint = map[0];
    for (pmap = map; pmap - map < boundary.y; pmap++) {
        for (ppoint = *pmap; ppoint - *pmap < boundary.x; ppoint++)
            if (!*ppoint)
                break;
        if (ppoint - *pmap == boundary.x) {//发现可消除行
            line++;
            eliminate(pmap - map);
        }
    }
    return line;
}

/***************************************************************************
  函数名称：num::end_a_num()
  功    能：数字落到底之后记录当前数字，消除行并更新分数
  输入参数：无
  返 回 值：无
  说    明：在land()判断为1之后使用
***************************************************************************/
void num::end_a_num()
{
    record_num();
    renew_score(check_line());
    return;
}