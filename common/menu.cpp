/* 2252201 胡世成 信18 */
#include <iostream>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include "../include/cmd_console_tools.h"
using namespace std;

/***************************************************************************
  函数名称：menu
  功    能：
  输入参数：char **instru : 传递指向说明字符串的指针数组名
  返 回 值：char：返回输入的ASCII码
  说    明：默认不区分字母大小写
***************************************************************************/
char menu(const char **instru)
{
	cct_cls();
	cout << "---------------------------------------------" << endl;
	cout << *instru++ << endl;
	cout << "---------------------------------------------" << endl;
	for (const char **pin = instru; *pin; ++pin)
		cout << *pin << endl;
	cout << "---------------------------------------------" << endl;
	cout << "[请选择:]";
	char input;
	const char** title = NULL;
	{
		int loop = 1;
		while (loop) {
			input = _getch();
			if (input >= 'a' && input <= 'z')
				input += 'A' - 'a';
			for (const char** pin = instru; *pin; ++pin)
				if (**pin == input) {
					title = pin;
					loop = 0;
					break;
				}
		}
	}
	cout << input;
	Sleep(100);
	if (title)
	    cct_setconsoletitle(*title + 2);
	return input;
}

/***************************************************************************
  函数名称：end
  功    能: 结束本小题
  输入参数：无
  返 回 值：无
  说    明：
***************************************************************************/
void end()
{
	char end[4] = { '\0' };
	cout << endl << "本小题结束，请输入END继续...";
	int x, y;
	cct_getxy(x, y);
	getchar();
	while (1) {
		for (int i = 0; i < 4 && (end[i] = getchar()) != '\n'; i++)
			continue;
		if (('E' == end[0] || 'e' == end[0]) && ('N' == end[1] || 'n' == end[1]) && ('D' == end[2] || 'd' == end[2]))
			break;
		cct_gotoxy(x, y);
		cout << "           ";
		cct_gotoxy(0, y + 1);
		cout << "输入错误，请重新输入";
		cct_gotoxy(x, y);
	}
	return;
}