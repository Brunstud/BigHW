/* 2252201 ������ �ƿ�2�� */
#include <iostream>
#include <cmath>
#include <conio.h>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

//Բ����
static const double PI = 3.1415926;

//����λ�õĺ�������
static const int point_x = 400;
static const int point_y = 400;

//��ɫ
#define WHITE_COLOR RGB(255,255,255)	//ѩ����Ҫ�õ��İ�ɫ
static const int color[] = { 0x00FF0000, 0x0000FF00, 0x000000FF }; //R G B

//��������
static const int size0 = 800;
static const int width = 4;

//�����ܴ���
static const int total_num = 10;

static inline void rotation_relative(const int rotation_angles, double& relative_x, double& relative_y)
{
	const double x = relative_x, y = relative_y;
	const double rotation_rads = rotation_angles / 180 * PI;
	relative_x = x * cos(rotation_rads) - y * sin(rotation_rads);
	relative_y = x * sin(rotation_rads) + y * cos(rotation_rads);
	return;
}

static void fractal(int n, const double x, const double y, const double size, const int rotation_degrees, const int RGB_value)
{
	const double dx = size * cos(rotation_degrees * PI / 180);
	const double dy = size * sin(rotation_degrees * PI / 180);
	if (0 == n) {
		hdc_line((int)x, (int)y, int(x + dx), int(y + dy), width, RGB_value);
		return;
	}
	fractal(n - 1, x, y, size / 3, rotation_degrees, RGB_value);//1
	fractal(n - 1, x + dx / 3, y + dy / 3, size / 3, rotation_degrees - 60, RGB_value);//2
	fractal(n - 1, x + dx / 2 + dy / 2 / sqrt(3), y + dy / 2 - dx / 2 / sqrt(3), size / 3, rotation_degrees + 60, RGB_value);//3
	fractal(n - 1, x + dx * 2 / 3, y + dy * 2 / 3, size / 3, rotation_degrees, RGB_value);//4
	return;
}

static void snowflake(int n, const double x, const double y, const double size, const int RGB_value)
{
	fractal(n, x - size / 4, y - size / 4, size / 2, 0, RGB_value);
	fractal(n, x + size / 4, y - size / 4, size / 2, 90, RGB_value);
	fractal(n, x + size / 4, y + size / 4, size / 2, 180, RGB_value);
	fractal(n, x - size / 4, y + size / 4, size / 2, 270, RGB_value);
	return;
}

//������Ҫ������������������Ƚ�С��ϣ���ܿ��õ���
void hdc_draw_Mathematical_curve_2252201()
{
	//���뺯����ִ�еĵ�һ���������� hdc_cls();
	hdc_cls();

	//������ʾ
	int bg_color, fg_color;
	cct_getcolor(bg_color, fg_color);
	cct_setcolor(bg_color, 0);
	cout << "���Σ�������ͼ�η��εĴ���(0-10)�� ";
	int num;		//��Ҫ���Ƶĺ������
	cin >> num;
	if (cin.fail() || num < 0 || num > total_num)	//�������򳬷�Χֱ��������
		num = total_num;

	cout << "���� " << num << " ��" << endl;;

	//��ͼ����
	fractal(num, point_x - size0 / 2, point_y + 100, size0, 0, color[rand() % 3]);
	cout << "���س�������...";
	while (_getch() != '\r')
		;

	//��������ѩ��
	hdc_cls();
	cout << "��������Сѩ��";
	for (int size = 40; size < size0; size += 40)
	    snowflake(6, point_x, point_y, size, WHITE_COLOR);

	cct_setcolor(bg_color, fg_color);//�ָ�ԭ��ɫ
	return;
}