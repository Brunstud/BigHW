/*2252201 ������ ��18 */
#include<iostream>
#include<conio.h>
#include<ctime>
#include<windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
#include"90-01-b2.h"
using namespace std;

void input(int* row, int* line);//��������������

int main()
{
	srand(unsigned int(time(0)));
	int cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines;
	char cur_title[256];
	cct_getconsoletitle(cur_title, sizeof(cur_title));
	cct_getconsoleborder(cur_cols, cur_lines, cur_buffer_cols, cur_buffer_lines);
	const char* instru[] = { "���������ǡ�", "A.�������ҳ����������ʶ", "B.���������һ���������ֲ�����ʾ��", "C.���������һ�أ��ֲ�����ʾ��", "D.αͼ�ν����������ѡ��һ��ɫ�飨�޷ָ��ߣ�",
		"E.αͼ������������ѡ��һ��ɫ�飨�зָ��ߣ�", "F.αͼ�ν������һ���������ֲ��裩", "G.αͼ�ν���������", "H.�Զ��ѻ����Ų���(������*�в�����40)", "Q.�˳�", NULL};
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
			cout << "������*�в�����40" << endl << "(ǧ��Ҫ��ͼ��10*10������û�Ż���˵)" << endl;
		input(&row, &line);
		ori_set_star(star, row, line);
		operate_by_choice(star, row, line, choice);
		end();
		cct_cls();
		cct_gotoxy(0, 0);
	}
	return 0;
}

void input(int* row, int* line)//��������������
{
	while (1) {
		cout << "����������(" << MIN << '-' << ROW << ")" << endl;
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
		cout << "����������(" << MIN << '-' << LINE << ")" << endl;
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

int score_by(int found)//ÿ�ֵ÷ּ���
{
	return found * found * 5;
}
int score_in_end(int num)//�þֽ����������
{
	if (num >= 10)
		return 10;
	return (10 - num) * 180 + 10;
}