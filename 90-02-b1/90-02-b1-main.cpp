/* 2252201 胡世成 计科2班 */
#include <iostream>
#include "90-02-b1.h"
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
using namespace std;

void input(int* height, int* width);//输入高度与宽度

int main()
{
	srand(unsigned int(time(0)));
	int cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines;
	char cur_title[256];
	cct_getconsoletitle(cur_title, sizeof(cur_title));
	cct_getconsoleborder(cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines);
	const char* instru[] = {"☆数字俄罗斯方块☆", "A.显示背景界面", "B.按顺序显示数字及旋转", "C.完成一次数字下落", "D.数字随机旋转下落",
		                    "E.单次游戏版（随机数字）", "F.完整游戏版（种子联机）", "Q.退出", NULL};
	while (1) {
		cct_setconsoletitle(cur_title);
		cct_setconsoleborder(cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines);
		cct_setfontsize("新宋体", 28);
		char choice = menu(instru);
		if ('Q' == choice)
			break;
		cct_cls();
		{//START
		    int width, height;
		    input(&width, &height);
		    cct_setfontsize("Terminal", 18, 10);
			num game;//类game
			game.set_boundary(width, height);
			operate(&game, choice);
		}//END
		end();
		cct_cls();
		cct_gotoxy(0, 0);
	}
	return 0;
}

void input(int* width, int* height)//输入高度与宽度
{
	while (1) {
		cout << "请输入列数(" << MIN_WIDTH << '-' << MAX_WIDTH << ")" << endl;
		cin >> *width;
		if (*width >= MIN_WIDTH && *width <= MAX_WIDTH && !(*width % 3))
			break;
		if (!cin) {
			cin.clear();
			while ('\n' == getchar())
				continue;
		}
		if (*width % 3)
			cout << "不是3的倍数" << endl;
	}
	while (1) {
		cout << "请输入高度(" << MIN_HEIGHT << '-' << MAX_HEIGHT << ")" << endl;
		cin >> *height;
		if (*height >= MIN_HEIGHT && *height <= MAX_HEIGHT)
			break;
		if (!cin) {
			cin.clear();
			while ('\n' == getchar())
				continue;
		}
	}
	return;
}