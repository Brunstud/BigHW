/* 2252201 胡世成 信18 */
#include<iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
#include"90-01-b1-hanoi.h"
using namespace std;

/* ----------------------------------------------------------------------------------

     本文件功能：
	1、放main函数
	2、初始化屏幕
	3、调用菜单函数（hanoi_menu.cpp中）并返回选项
	4、根据选项调用菜单各项对应的执行函数（hanoi_multiple_solutions.cpp中）

     本文件要求：
	1、不允许定义全局变量（含外部全局和静态全局，const及#define不在限制范围内）
	2、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	3、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main()
{
	/* demo中首先执行此句，将cmd窗口设置为40行x120列（缓冲区宽度120列，行数9000行，即cmd窗口右侧带有垂直滚动杆）*/
	cct_setconsoleborder(120, 40, 120, 9000);
	int choice;
	const char* instru[] = { "☆汉诺塔☆", "1.基本解", "2.基本解(步数记录)", "3.内部数组显示(横向)", "4.内部数组显示(纵向 + 横向)", "5.图形解 - 预备 - 画三个圆柱",
							 "6.图形解 - 预备 - 在起始柱上画n个盘子", "7.图形解 - 预备 - 第一次移动", "8.图形解 - 自动移动版本", "9.图形解 - 游戏版", "0.退出", NULL};
	while (choice = menu(instru) - '0') {
		int N = 0;//用于计数
		int top[3] = { 0 };//栈顶指针
		int stack[3][Layers] = { 0 };//Tony Stark
		int n, speed = -1;
		char src, tmp, dst;
		if (5 == choice) {
			cct_cls();
			print_base();
		}
		else {
			if (4 == choice || 8 == choice)
				speed = input(&n, &src, &tmp, &dst, 1);
			else
				input(&n, &src, &tmp, &dst, 0);
			ori_set_stack(stack, top, n, src);
			start(stack, top, n, src, dst, speed, choice);
		}
		if (9 != choice && !(choice >= 5 && choice <= 7))
			hanoi(stack, top, n, src, tmp, dst, 1, speed, choice);
		if (9 == choice)
			hanoi_game(stack, top, n, src, dst);
		if (8 == choice)
			cct_gotoxy(0, 38);
		cout << endl << "按回车键继续";
		while (_getch() != '\r')
			continue;
	}
	return 0;
}
