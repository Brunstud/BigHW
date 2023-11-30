/*2252201 胡世成 信18 */
/*放内部数组方式实现的各函数*/
#include<iostream>
#include<conio.h>
#include<iomanip>
#include "../include/cmd_console_tools.h"
#include"90-01-b2.h"
using namespace std;

/***************************************************************************
  函数名称：ori_set_star
  功    能：初始设置数组star
  输入参数：int star[][LINE], int row, int line
  返 回 值：无
  说    明：
***************************************************************************/
void ori_set_star(int star[][LINE], int row, int line)
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < line; j++)
			star[i][j] = rand() % STAR + 1;
	return;
}

/***************************************************************************
  函数名称：show_array
  功    能：现实当前数组star或find
  输入参数：const int star[][LINE], const char find[][LINE], int row, int line, int sign
  返 回 值：无
  说    明：sign==0时"当前数组"；sign>0时"当前数组（不同色标识）"；sign<0时"查找结果数组"
***************************************************************************/
void show_array(const char * intro, const int star[][LINE], const char find[][LINE], int row, int line, int sign)
{
    if (intro)
	    cout << intro << endl;
	cout << "  |";
	for (int i = 0; i < line; i++)
		cout << setfill(' ') << setw(3) << i;
	cout << endl;
	cout << "--+";
	cout << setfill('-') << setw(line * 3 + 1) << '-' << endl;
	for (int i = 0; i < row; i++) {
		cout << char(i + 'A') << ' ' << '|';
		for (int j = 0; j < line; j++) {
			cout << "  ";
			if (sign < 0)
				if (find[i][j])
					cout << '*';
				else
					cout << '0';
			else {
				if (sign && find[i][j] || !sign && !star[i][j])
				    cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
				cout << star[i][j];
				if (sign && find[i][j] || !sign && !star[i][j])
					cct_setcolor();
			}
		}
		cout << endl;
	}
	return;
}

/***************************************************************************
  函数名称：input_step
  功    能：输入步骤
  输入参数：int star[][LINE], char find[][LINE], int row, int line
  返 回 值：int
  说    明：返回找到的星星数
***************************************************************************/
int input_step(int star[][LINE], char find[][LINE], int * r, int * l, int row, int line)
{
	int found = 0;
	while (1) {
		char r_char;
		int x, y;
		cout << "请以字母+数字形式[例：c2]输入矩阵坐标：";
		cct_getxy(x, y);
		while (1) {
			cin >> r_char >> *l;
			if (r_char >= 'a' && r_char <= 'a' + row - 1)
				r_char = r_char - 'a' + 'A';
			if (!cin || r_char < 'A' || r_char > 'A' + row - 1 || *l < 0 || *l > line - 1) {
				cct_gotoxy(x, y);
				cout << "                         ";
				cct_gotoxy(0, y + 1);
				cout << "输入错误，请重新输入.";
				cct_gotoxy(x, y);
				if (!cin)
					cin.clear();
			    while ('\n' != getchar())
					continue;
				continue;
			}
			break;
		}
		cout << "输入为" << r_char << "行" << *l << "列         " << endl;
		*r = r_char - 'A';
		if (!star[*r][*l]) {
			cout << "输入的矩阵坐标位置为0（非法位置），请重新输入" << endl;
			continue;
		}
		if ((found = find_star(star, find, *r, *l)) - 1)
			break;
		else {
			find[*r][*l] = '\0';//没找到连续项时删除原位的‘*’
			cout << "输入的矩阵坐标位置处无连续相同值，请重新输入" << endl;
		}
	}
	return found;
}

/***************************************************************************
  函数名称：question1
  功    能：解决第一小题
  输入参数：int star[][LINE], char find[][LINE], int row, int line
  返 回 值：int
  说    明：返回找到的星星数
***************************************************************************/
int question1(int star[][LINE], char find[][LINE], int * r, int * l, int row, int line)
{
	int found = 0, num;
	cout << endl;
	show_array("当前数组:", star, find, row, line, 0);
	cout << endl;
	if (num = check_star(star, row, line)) {
		cct_setcolor(COLOR_HYELLOW, COLOR_RED);
		cout << "剩余" << (num > 0 ? num : 0) << "个不可消除项，本关结束！";
		cout << endl;
		return 0;
	}
	found = input_step(star, find, r, l, row, line);
	cout << endl;
	show_array("查找结果数组:", star, find, row, line, -1);
	cout << endl;
	show_array("当前数组(不同色标识):", star, find, row, line, 1);
	cout << endl;
	return found;
}

/***************************************************************************
  函数名称：question2
  功    能：解决第二小题
  输入参数：int star[][LINE], char find[][LINE], int row, int line
  返 回 值：int
  说    明：返回是否继续与分数（分数为0即为结束）
***************************************************************************/
int question2(int star[][LINE], int* r, int* l, int row, int line, int score)
{
	while (1) {
		char find[ROW][LINE] = { '\0' };
		int found = question1(star, find, r, l, row, line);
		if (!found) {
			int num = check_star(star, row, line);
			score += score_in_end(num > 0 ? num : 0);
			cout << "共获得" << score << "分." << endl;
			cct_setcolor();
			score = 0;
			break;
		}
		cout << "请确认是否把" << char(*r + 'A') << *l << "及周围的相同值消除(Y/N/Q)：";
		int x, y;
		char answer;
		cct_getxy(x, y);
		do {
			cct_gotoxy(x, y);
			answer = _getch();
			cout << answer;
		} while (answer != 'Y' && answer != 'N' && answer != 'Q' && answer != 'y' && answer != 'n' && answer != 'q');
		if ('Y' == answer || 'y' == answer) {
			cout << endl << endl;
			delete_star(star, find);
			show_array("相同值归并后的数组(不同色标识)：", star, find, row, line, 1);
			cout << endl;
			cout << "本次消灭" << found << "个星星" << endl;
			cout << "本次得分：" << score_by(found) << " 总得分：" << (score += score_by(found)) << endl;
			cout << endl;
			cout << "按回车键进行数组下落操作...";
			while (_getch() != '\r')
				continue;
			cout << endl;
			fallen_star(star, find, row, line);
			show_array("下落的数组：", star, find, row, line, 0);
			cout << endl;
			break;
		}
		if ('Q' == answer || 'q' == answer) {
			score = 0;
			cout << endl;
			break;
		}
		if ('N' == answer || 'n' == answer) {
			cout << endl;
			continue;
		}
	}
	return score;
}

/***************************************************************************
  函数名称：question3
  功    能：解决第三小题
  输入参数：int star[][LINE], char find[][LINE], int row, int line
  返 回 值：无
  说    明：
***************************************************************************/
void question3(int star[][LINE], int* r, int* l, int row, int line, int score)
{
	while (1) {
		score = question2(star, r, l, row, line, score);
		if (!score)
			break;
		cout << "本次消除结束，按回车键继续新一轮的消除...";
		while (_getch() != '\r')
			continue;
		cout << endl;
	}
	return;
}