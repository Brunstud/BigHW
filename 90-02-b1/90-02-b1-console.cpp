/* 2252201 胡世成 计科2班 */
#include <iostream>
#include <iomanip>
//#include <conio.h>
#include <windows.h>
#include "90-02-b1.h"
#include "../include/cmd_console_tools.h" 
#include "../include/com_console.h"
using namespace std;

#define NUM_X 3 * STAR_X
#define NUM_Y 5 * STAR_Y
#define X_L0    24
#define Y_R0    2
#define NEX_X   8
#define NEX_Y   10
#define SCORE_Y   7 + NUM_Y
#define LINE_Y    4 + SCORE_Y
#define NUMBER_Y  4 + LINE_Y
#define TIME_Y    4 + NUMBER_Y
#define SPEED_Y   4 + TIME_Y
#define INSTRU_Y  4 + SPEED_Y

/***************************************************************************
  函数名称：ori_paint
  功    能：绘制游戏背景板
  输入参数：int instru 是否显示说明
  返 回 值：无
  说    明：
***************************************************************************/
void num::ori_paint(int instru)
{
	cct_cls();
	const int width = boundary.x;
	const int height = boundary.y;
	const int WID = width * STAR_X + 4 + X_L0;//窗口总宽
	const int HEI = height * STAR_Y + 2 + 4;//窗口总高
	cct_setconsoleborder(WID, HEI);
	cct_gotoxy(2, 2);
	bg_paint(18, 38 + instru * 16, COLOR_HBLACK);
	cct_gotoxy(0, 0);
	cout << "屏幕当前设置为：" << HEI << "行" << WID << "列";
	cct_setcolor(COLOR_HBLACK, COLOR_HWHITE);
	cct_gotoxy(2, NEX_Y - 8);
	cout << "下一个数字：";
	cct_gotoxy(2, SCORE_Y - 2);
	cout << "  得  分  ：";
	cct_gotoxy(NEX_X, SCORE_Y);
	cout << "0  ";
	cct_gotoxy(2, LINE_Y - 2);
	cout << "消除总行数：";
	cct_gotoxy(NEX_X, LINE_Y);
	cout << "0  ";
	cct_gotoxy(2, NUMBER_Y - 2);
	cout << "数  字  数：";
	cct_gotoxy(NEX_X, NUMBER_Y);
	cout << "1  ";
	cct_gotoxy(2, TIME_Y - 2);
	cout << "  时  间  ：";
	cct_gotoxy(NEX_X, TIME_Y);
	cout << "0  ";
	cct_gotoxy(2, SPEED_Y - 2);
	cout << "速度  等级：";
	cct_gotoxy(NEX_X, SPEED_Y);
	cout << "1  ";
	if (instru) {
		cct_gotoxy(2, INSTRU_Y);
		cout << "操作说明：";
		cct_gotoxy(2, INSTRU_Y + 1);
		cout << "向左移：    A/←键";
		cct_gotoxy(2, INSTRU_Y + 2);
		cout << "向右移：    D/→键";
		cct_gotoxy(2, INSTRU_Y + 3);
		cout << "逆时针旋转：W/↑键";
		cct_gotoxy(2, INSTRU_Y + 4);
		cout << "加速下移：  S/↓键";
		cct_gotoxy(2, INSTRU_Y + 5);
		cout << "直接坠落:空格/回车";
		cct_gotoxy(2, INSTRU_Y + 6);
		cout << "结束游戏:Q/ESC";
		cct_gotoxy(2, INSTRU_Y + 8);
		cout << "记分规则：";
		cct_gotoxy(2, INSTRU_Y + 9);
		cout << "第n次消除一行加n分";
		cct_gotoxy(2, INSTRU_Y + 10);
		cout << "同时消除多行则每行";
		cct_gotoxy(2, INSTRU_Y + 11);
		cout << "加n分。每15分速度";
		cct_gotoxy(2, INSTRU_Y + 12);
		cout << "上升一个等级";
	}
	cct_setcolor();
	cct_gotoxy(X_L0, Y_R0);
	bg_paint(width * STAR_X, height * STAR_Y);
	return;
}

/***************************************************************************
  函数名称：paint_num
  功    能：绘制数字
  输入参数：无
  返 回 值：无
  说    明：依据位置position及数组num_shape绘制数字
***************************************************************************/
void num::paint_num()
{
	point* ppoint = num_shape;
	for (; ppoint->x != end_num; ++ppoint) {
		int x = position.x + ppoint->x;
		int y = position.y + ppoint->y;
		if (x < 0 || y < 0 || x > boundary.x || y > boundary.y)
			continue;
		paint_star(X_L0 + x * STAR_X, Y_R0 + y * STAR_Y, cur_num + 9, COLOR_BLACK + cur_num);
	}
	return;
}

/***************************************************************************
  函数名称：erase_num
  功    能：擦除数字
  输入参数：无
  返 回 值：无
  说    明：依据位置position及数组num_shape擦除数字
***************************************************************************/
void num::erase_num()
{
	point* ppoint = num_shape;
	for (; ppoint->x != end_num; ++ppoint) {
		int x = position.x + ppoint->x;
		int y = position.y + ppoint->y;
		if (x < 0 || y < 0 || x > boundary.x || y > boundary.y)
			continue;
		erase_star(X_L0 + x * STAR_X, Y_R0 + y * STAR_Y, COLOR_HWHITE);
	}
	return;
}

/***************************************************************************
  函数名称：show_nex_num
  功    能：在提示栏提示下一个数字
  输入参数：无
  返 回 值：无
  说    明：依据位置position及数组num_shape擦除数字
***************************************************************************/
void num::show_nex_num(int num)
{
	if (nex_num >=0 && nex_num < 10) {
		set_num(nex_num);//用下一个数替代当前数
		for (point* ppoint = num_shape; ppoint->x != end_num; ++ppoint) {
			int x = ppoint->x;
			int y = ppoint->y;
			erase_star(NEX_X + x * STAR_X, NEX_Y + y * STAR_Y, COLOR_HBLACK);
		}
	}
	int cur = cur_num;//记录当前数
	nex_num = num;
	set_num(nex_num);
	for (point* ppoint = num_shape; ppoint->x != end_num; ++ppoint) {
		int x = ppoint->x;
		int y = ppoint->y;
		paint_star(NEX_X + x * STAR_X, NEX_Y + y * STAR_Y, cur_num + 9, COLOR_BLACK + cur_num);
	}
	set_num(cur);//恢复当前数
	return;
}

/***************************************************************************
  函数名称：num::eliminate
  功    能：消除行，并实现图形下落
  输入参数：int line : 该次消除第line行
  返 回 值：无
  说    明：类num内置函数，在check_line()中使用
***************************************************************************/
void num::eliminate(int line)
{
	for (int y = line; y > 0; --y) {
		for (int x = 0; x < boundary.x; ++x) {
			map[y][x] = map[y - 1][x];
			if (map[y - 1][x])
				paint_star(X_L0 + x * STAR_X, Y_R0 + y * STAR_Y, map[y - 1][x] + 8, COLOR_BLACK + map[y - 1][x] - 1);
			else
				erase_star(X_L0 + x * STAR_X, Y_R0 + y * STAR_Y, COLOR_HWHITE);
		}
	}
	for (int x = 0; x < boundary.x; ++x) {
		map[0][x] = 0;
		erase_star(X_L0 + x * STAR_X, Y_R0, COLOR_HWHITE);
	}
	return;
}

/***************************************************************************
  函数名称：num::renew_score
  功    能：根据消除行，更新分数及行数显示
  输入参数：int lines : 该次消除行数(0-5)
  返 回 值：无
  说    明：类num内置函数，在end_a_num()中使用
***************************************************************************/
void num::renew_score(int lines)
{
	cct_setcolor(COLOR_HBLACK, COLOR_HWHITE);
	score += (line + 1) * lines;
	cct_gotoxy(NEX_X, SCORE_Y);
	cout << setw(3) << left << score;
	line += lines;
	cct_gotoxy(NEX_X, LINE_Y);
	cout << setw(3) << left << line;
	numbers++;
	cct_gotoxy(NEX_X, NUMBER_Y);
	cout << setw(3) << left << numbers;
	cout << right;
	cct_setcolor();
	return;
}

/***************************************************************************
  函数名称：num::move
  功    能：根据键盘输入，调整数字块的位置
  输入参数：无
  返 回 值：如果落底返回1,退出返回-1，否则返回0
  说    明：与drop_num()一同使用
***************************************************************************/
int num::move()
{
	int feedback;
	int possible = 1, drop;
	if (feedback = move_keyboard()) {
		switch (feedback) {
			case 1:
				rotate();
				break;
			case 2:
				drop = drop_num();
				return drop;
			case -2:
				while (!(drop = drop_num()))
					;
				return drop;
			case 3:
				for (point* ppoint = num_shape; ppoint->x != end_num; ++ppoint) {
					int x = position.x + ppoint->x - 1;//预判左移之后的情况
					int y = position.y + ppoint->y;
					if (x < 0 || y >= 0 && map[y][x]) {
						possible = 0;
						break;
					}
				}
				if (possible) {
					erase_num();
					position.x--;
					paint_num();
				}
				break;
			case 4:
				for (point* ppoint = num_shape; ppoint->x != end_num; ++ppoint) {
					int x = position.x + ppoint->x + 1;//预判右移之后的情况
					int y = position.y + ppoint->y;
					if (x >= boundary.x || y > 0 && map[y][x]) {
						possible = 0;
						break;
					}
				}
				if (possible) {
					erase_num();
					position.x++;
					paint_num();
				}
				break;
			case -1:
				return -1;
		}
	}
	return 0;
}

/***************************************************************************
  函数名称：num::time_fly(int t)
  功    能：暂停t毫秒(根据速度speed调整),并更新时间
  输入参数：该次流逝的时间（单位：ms）
  返 回 值：当前时间（单位：ms）
  说    明：下落时间控制在1.0-0.3s时,t一般取100，重复10次为1秒
***************************************************************************/
int num::time_fly(int t)
{
	if (speed < 8)
	    speed = 1 + score / 15;
	t = t * (11 - speed) / 10;
	//t = (t < 30 ? 30 : t);
	Sleep(t);
	time += t;
	cct_setcolor(COLOR_HBLACK, COLOR_HWHITE);
	cct_gotoxy(NEX_X, TIME_Y);
	cout << setw(3) << left << time / 1000;
	cct_gotoxy(NEX_X, SPEED_Y);
	cout << setw(3) << left << speed;
	cout << right;
	cct_setcolor();
	return time;
}