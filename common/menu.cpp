/* 2252201 ������ ��18 */
#include <iostream>
#include <conio.h>
#include <ctime>
#include <windows.h>
#include "../include/cmd_console_tools.h"
using namespace std;

/***************************************************************************
  �������ƣ�menu
  ��    �ܣ�
  ���������char **instru : ����ָ��˵���ַ�����ָ��������
  �� �� ֵ��char�����������ASCII��
  ˵    ����Ĭ�ϲ�������ĸ��Сд
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
	cout << "[��ѡ��:]";
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
  �������ƣ�end
  ��    ��: ������С��
  �����������
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void end()
{
	char end[4] = { '\0' };
	cout << endl << "��С�������������END����...";
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
		cout << "�����������������";
		cct_gotoxy(x, y);
	}
	return;
}