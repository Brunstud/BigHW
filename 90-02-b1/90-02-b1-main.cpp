/* 2252201 ������ �ƿ�2�� */
#include <iostream>
#include "90-02-b1.h"
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
using namespace std;

void input(int* height, int* width);//����߶�����

int main()
{
	srand(unsigned int(time(0)));
	int cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines;
	char cur_title[256];
	cct_getconsoletitle(cur_title, sizeof(cur_title));
	cct_getconsoleborder(cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines);
	const char* instru[] = {"�����ֶ���˹�����", "A.��ʾ��������", "B.��˳����ʾ���ּ���ת", "C.���һ����������", "D.���������ת����",
		                    "E.������Ϸ�棨������֣�", "F.������Ϸ�棨����������", "Q.�˳�", NULL};
	while (1) {
		cct_setconsoletitle(cur_title);
		cct_setconsoleborder(cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines);
		cct_setfontsize("������", 28);
		char choice = menu(instru);
		if ('Q' == choice)
			break;
		cct_cls();
		{//START
		    int width, height;
		    input(&width, &height);
		    cct_setfontsize("Terminal", 18, 10);
			num game;//��game
			game.set_boundary(width, height);
			operate(&game, choice);
		}//END
		end();
		cct_cls();
		cct_gotoxy(0, 0);
	}
	return 0;
}

void input(int* width, int* height)//����߶�����
{
	while (1) {
		cout << "����������(" << MIN_WIDTH << '-' << MAX_WIDTH << ")" << endl;
		cin >> *width;
		if (*width >= MIN_WIDTH && *width <= MAX_WIDTH && !(*width % 3))
			break;
		if (!cin) {
			cin.clear();
			while ('\n' == getchar())
				continue;
		}
		if (*width % 3)
			cout << "����3�ı���" << endl;
	}
	while (1) {
		cout << "������߶�(" << MIN_HEIGHT << '-' << MAX_HEIGHT << ")" << endl;
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