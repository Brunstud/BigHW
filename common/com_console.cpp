/* 2252201 胡世成 计科2班 */
#include <iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
using namespace std;

/***************************************************************************
  函数名称：paint_star
  功    能：绘制星星
  输入参数：int x, int y, int bg_color, int fg_color
  返 回 值：无
  说    明：结束返回默认设置
***************************************************************************/
void paint_star(const int x, const int y, const int bg_color, const int fg_color)
{
	cct_setcolor(bg_color, fg_color);
	cct_gotoxy(x, y);
	cout << "╔═╗";
	cct_gotoxy(x, y + 1);
	cout << "║☆║";
	cct_gotoxy(x, y + 2);
	cout << "╚═╝";
	cct_setcolor();
	return;
}

/***************************************************************************
  函数名称：erase_star
  功    能：擦除星星
  输入参数：int x, int y, int bg_color
  返 回 值：无
  说    明：结束返回默认设置
***************************************************************************/
void erase_star(const int x, const int y, const int bg_color)
{
	cct_setcolor(bg_color);
	cct_gotoxy(x, y);
	cout << "      ";
	cct_gotoxy(x, y + 1);
	cout << "      ";
	cct_gotoxy(x, y + 2);
	cout << "      ";
	cct_setcolor();
	return;
}

/***************************************************************************
  函数名称：bg_paint
  功    能：从光标位置开始绘制背景板
  输入参数：int width, int height, int bg_color = COLOR_HWHITE, int fg_color = COLOR_BLACK
  返 回 值：无
  说    明：默认黑边白底，w和h表示内部空间宽高
  注    意：（1）在绘制背景板前，注意调整窗口大小
            （2）width最好为2的倍数，否则会减一
***************************************************************************/
void bg_paint(const int width, const int height, const int bg_color, const int fg_color)
{
	int x, y;
	cct_getxy(x, y);
	cct_setcolor(bg_color, fg_color);
	//==============================打印背景板
	cct_gotoxy(x-2, y-1);
	cout << "╔";
	for (int i = 0; i < width / 2; i++)
		cout << "═";
	cout << "╗";
	if (width % 2) {
		cct_gotoxy(x-2, y-1);
		cout << "warning: width is not an even!";
	}
	for (int i = 0; i < height; i++) {
		cct_gotoxy(x-2, y + i);
		cout << "║";
		for (int j = 0; j < width / 2; j++)
			cout << "  ";
		cout << "║";
	}
	cct_gotoxy(x-2, y + height);
	cout << "╚";
	for (int i = 0; i < width / 2; i++)
		cout << "═";
	cout << "╝";
	cct_setcolor();
	return;
}

/***************************************************************************
  函数名称：move_keyboard
  功    能：判断是否有键盘按动，并判断移动方向
  输入参数：无
  返 回 值：上1，下2，左3，右4；结束返回-1；确定返回-2
            若无规定键盘操作，返回0；
  说    明：W与上箭头表示“上”；S与下箭头表示“下”；A与左箭头表示“左”；D与右箭头表示“右”；
            Q与ESC表示“结束”；回车与空格表示“确定”
  注    意：
***************************************************************************/
int move_keyboard()
{
	if (_kbhit()) {
		int X = 0, Y = 0;
		int ret, maction;
		int keycode1, keycode2;
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_KEYBOARD_EVENT) {
			switch (keycode1) {
				case 13:    //回车
				case 32:    //空格
					return -2;
				case 'Q':
				case 'q':
				case 27:    //ESC
					return -1;
					break;
				case 'W':
				case 'w':
					return 1;
				case 'S':
				case 's':
					return 2;
				case 'A':
				case 'a':
					return 3;
				case 'D':
				case 'd':
					return 4;
				case 224:
					switch (keycode2) {
						case KB_ARROW_UP:            //上箭头
							return 1;
						case KB_ARROW_DOWN:          //下箭头
							return 2;
						case KB_ARROW_LEFT:          //左箭头
							return 3;
						case KB_ARROW_RIGHT:          //右箭头
							return 4;
					}
				default:
					return 0;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT）
	}
	return 0;
}