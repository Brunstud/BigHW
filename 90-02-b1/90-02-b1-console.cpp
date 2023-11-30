/* 2252201 ������ �ƿ�2�� */
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
  �������ƣ�ori_paint
  ��    �ܣ�������Ϸ������
  ���������int instru �Ƿ���ʾ˵��
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void num::ori_paint(int instru)
{
	cct_cls();
	const int width = boundary.x;
	const int height = boundary.y;
	const int WID = width * STAR_X + 4 + X_L0;//�����ܿ�
	const int HEI = height * STAR_Y + 2 + 4;//�����ܸ�
	cct_setconsoleborder(WID, HEI);
	cct_gotoxy(2, 2);
	bg_paint(18, 38 + instru * 16, COLOR_HBLACK);
	cct_gotoxy(0, 0);
	cout << "��Ļ��ǰ����Ϊ��" << HEI << "��" << WID << "��";
	cct_setcolor(COLOR_HBLACK, COLOR_HWHITE);
	cct_gotoxy(2, NEX_Y - 8);
	cout << "��һ�����֣�";
	cct_gotoxy(2, SCORE_Y - 2);
	cout << "  ��  ��  ��";
	cct_gotoxy(NEX_X, SCORE_Y);
	cout << "0  ";
	cct_gotoxy(2, LINE_Y - 2);
	cout << "������������";
	cct_gotoxy(NEX_X, LINE_Y);
	cout << "0  ";
	cct_gotoxy(2, NUMBER_Y - 2);
	cout << "��  ��  ����";
	cct_gotoxy(NEX_X, NUMBER_Y);
	cout << "1  ";
	cct_gotoxy(2, TIME_Y - 2);
	cout << "  ʱ  ��  ��";
	cct_gotoxy(NEX_X, TIME_Y);
	cout << "0  ";
	cct_gotoxy(2, SPEED_Y - 2);
	cout << "�ٶ�  �ȼ���";
	cct_gotoxy(NEX_X, SPEED_Y);
	cout << "1  ";
	if (instru) {
		cct_gotoxy(2, INSTRU_Y);
		cout << "����˵����";
		cct_gotoxy(2, INSTRU_Y + 1);
		cout << "�����ƣ�    A/����";
		cct_gotoxy(2, INSTRU_Y + 2);
		cout << "�����ƣ�    D/����";
		cct_gotoxy(2, INSTRU_Y + 3);
		cout << "��ʱ����ת��W/����";
		cct_gotoxy(2, INSTRU_Y + 4);
		cout << "�������ƣ�  S/����";
		cct_gotoxy(2, INSTRU_Y + 5);
		cout << "ֱ��׹��:�ո�/�س�";
		cct_gotoxy(2, INSTRU_Y + 6);
		cout << "������Ϸ:Q/ESC";
		cct_gotoxy(2, INSTRU_Y + 8);
		cout << "�Ƿֹ���";
		cct_gotoxy(2, INSTRU_Y + 9);
		cout << "��n������һ�м�n��";
		cct_gotoxy(2, INSTRU_Y + 10);
		cout << "ͬʱ����������ÿ��";
		cct_gotoxy(2, INSTRU_Y + 11);
		cout << "��n�֡�ÿ15���ٶ�";
		cct_gotoxy(2, INSTRU_Y + 12);
		cout << "����һ���ȼ�";
	}
	cct_setcolor();
	cct_gotoxy(X_L0, Y_R0);
	bg_paint(width * STAR_X, height * STAR_Y);
	return;
}

/***************************************************************************
  �������ƣ�paint_num
  ��    �ܣ���������
  �����������
  �� �� ֵ����
  ˵    ��������λ��position������num_shape��������
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
  �������ƣ�erase_num
  ��    �ܣ���������
  �����������
  �� �� ֵ����
  ˵    ��������λ��position������num_shape��������
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
  �������ƣ�show_nex_num
  ��    �ܣ�����ʾ����ʾ��һ������
  �����������
  �� �� ֵ����
  ˵    ��������λ��position������num_shape��������
***************************************************************************/
void num::show_nex_num(int num)
{
	if (nex_num >=0 && nex_num < 10) {
		set_num(nex_num);//����һ���������ǰ��
		for (point* ppoint = num_shape; ppoint->x != end_num; ++ppoint) {
			int x = ppoint->x;
			int y = ppoint->y;
			erase_star(NEX_X + x * STAR_X, NEX_Y + y * STAR_Y, COLOR_HBLACK);
		}
	}
	int cur = cur_num;//��¼��ǰ��
	nex_num = num;
	set_num(nex_num);
	for (point* ppoint = num_shape; ppoint->x != end_num; ++ppoint) {
		int x = ppoint->x;
		int y = ppoint->y;
		paint_star(NEX_X + x * STAR_X, NEX_Y + y * STAR_Y, cur_num + 9, COLOR_BLACK + cur_num);
	}
	set_num(cur);//�ָ���ǰ��
	return;
}

/***************************************************************************
  �������ƣ�num::eliminate
  ��    �ܣ������У���ʵ��ͼ������
  ���������int line : �ô�������line��
  �� �� ֵ����
  ˵    ������num���ú�������check_line()��ʹ��
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
  �������ƣ�num::renew_score
  ��    �ܣ����������У����·�����������ʾ
  ���������int lines : �ô���������(0-5)
  �� �� ֵ����
  ˵    ������num���ú�������end_a_num()��ʹ��
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
  �������ƣ�num::move
  ��    �ܣ����ݼ������룬�������ֿ��λ��
  �����������
  �� �� ֵ�������׷���1,�˳�����-1�����򷵻�0
  ˵    ������drop_num()һͬʹ��
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
					int x = position.x + ppoint->x - 1;//Ԥ������֮������
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
					int x = position.x + ppoint->x + 1;//Ԥ������֮������
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
  �������ƣ�num::time_fly(int t)
  ��    �ܣ���ͣt����(�����ٶ�speed����),������ʱ��
  ����������ô����ŵ�ʱ�䣨��λ��ms��
  �� �� ֵ����ǰʱ�䣨��λ��ms��
  ˵    ��������ʱ�������1.0-0.3sʱ,tһ��ȡ100���ظ�10��Ϊ1��
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