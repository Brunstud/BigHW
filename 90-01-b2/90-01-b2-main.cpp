/*2252201 胡世成 信18 */
#include<iostream>
#include<conio.h>
#include<ctime>
#include<windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
#include"90-01-b2.h"
using namespace std;

void input(int* row, int* line);//输入行数与列数

int main()
{
	srand(unsigned int(time(0)));
	int cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines;
	char cur_title[256];
	cct_getconsoletitle(cur_title, sizeof(cur_title));
	cct_getconsoleborder(cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines);
	const char* instru[] = { "☆消灭星星☆", "A.命令行找出可消除项并标识", "B.命令行完成一次消除（分步骤显示）", "C.命令行完成一关（分步骤显示）", "D.伪图形界面下用鼠标选择一个色块（无分割线）",
		"E.伪图像界面下用鼠标选择一个色块（有分割线）", "F.伪图形界面完成一次消除（分步骤）", "G.伪图形界面完整版", "H.自动狙击最优策略(建议行*列不大于40)", "Q.退出", NULL};
	while (1) {
		int star[ROW][LINE] = { 0 };
		cct_setconsoletitle(cur_title);
		cct_setconsoleborder(cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines);
		char choice = menu(instru);
		if ('Q' == choice)
			break;
		cct_cls();
		int row, line;
		if ('H' == choice)
			cout << "建议行*列不大于40" << endl << "(千万不要试图跑10*10！！还没优化的说)" << endl;
		input(&row, &line);
		ori_set_star(star, row, line);
		operate_by_choice(star, row, line, choice);
		end();
		cct_cls();
		cct_gotoxy(0, 0);
	}
	return 0;
}

void input(int* row, int* line)//输入行数与列数
{
	while (1) {
		cout << "请输入行数(" << MIN << '-' << ROW << ")" << endl;
		cin >> *row;
		if (*row >= MIN && *row <= ROW)
			break;
		if (!cin) {
			cin.clear();
			while ('\n' == getchar())
				continue;
		}
	}
	while (1) {
		cout << "请输入列数(" << MIN << '-' << LINE << ")" << endl;
		cin >> *line;
		if (*line >= MIN && *line <= LINE)
			break;
		if (!cin) {
			cin.clear();
			while ('\n' == getchar())
				continue;
		}
	}
	return;
}

int score_by(int found)//每局得分计算
{
	return found * found * 5;
}
int score_in_end(int num)//该局结束结算分数
{
	if (num >= 10)
		return 10;
	return (10 - num) * 180 + 10;
}