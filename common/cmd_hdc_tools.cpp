/* 2252201 ������ �ƿ�2�� */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//δ���������²Ŵ���������

/* ���������Ҫ��ͷ�ļ� */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS�д˴����в����ߣ���Ӱ������ʹ��

/* �Ѷ���ľ�̬ȫ�ֱ��� */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* �����ʶ�ʹ�þ�̬ȫ�ֱ�����������static�����ⲿ���ɼ� */

/* �˴���������Զ��庯����������static�����ⲿ���ɼ� */
//�Ƕ�ת����
static inline double deg2rad(const double angle)
{
	return angle * PI / 180;
}

static inline double arc2sys_ang(const double angle)
{
	return angle;
}

static inline int find_line_x(const int x1, const int y1, const int x2, const int y2, const int y)
{
	if (y < y1 && y < y2 || y > y1 && y > y2 || y1 == y2)
		return 0; //�Ҳ���
	int x = (int)(x1 + (y - y1) * (x2 - x1) * 1.0 / (y2 - y1));
	return (x ? x : 1);
}

static inline int find_arc_left_x(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int y)
{
	int angleBEG = ((angle_begin % 360) + 360) % 360;
	int angleEND = ((angle_end % 360) + 360) % 360;
	int y_min, y_max;
	if (angleBEG == angleEND) {
		y_min = point_y - radius;
		y_max = point_y + radius;
	}
	else if (angleBEG <= 180) {
		if (angleEND > 180) {
			y_min = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angleEND))));
			y_max = point_y + radius;
		}
		else if (angleEND <= angleBEG) {
			y_min = point_y - radius;
			y_max = point_y + radius;
		}
		else return 0;
	}
	else {
		if (angleEND > 180) {
			y_min = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angleEND))));
			y_max = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angleBEG))));
		}
		else {
			y_min = point_y - radius;
			y_max = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angleBEG))));
		}
	}
	if (y > point_y + radius || y < point_y - radius) return 0;
	else if (y_min >= y_max) { if (y < y_min && y > y_max) return 0; }
	else if (y >= y_max || y <= y_min) return 0;

	int left_x = point_x - (int)(sqrt(radius * radius - (y - point_y) * (y - point_y)));
	return (left_x ? left_x : 1);
}

static inline int find_arc_right_x(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int y)
{
	int const angleBEG = ((angle_begin % 360) + 360) % 360;
	int const angleEND = ((angle_end % 360) + 360) % 360;
	int y_min, y_max;
	if (angleBEG == angleEND) {
		y_min = point_y - radius;
		y_max = point_y + radius;
	}
	else if (angleBEG <= 180) {
		if (angleEND >= 180) {
			y_min = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angleBEG))));
			y_max = point_y + radius;
		}
		else {
			y_min = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angleBEG))));
			y_max = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angleEND))));
		}
	}
	else {
		if (!angleEND) return 0;
		if (angleEND <= 180) {
			y_min = point_y - radius;
			y_max = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angleEND))));
		}
		else if (angleEND <= angleBEG) {
			y_min = point_y - radius;
			y_max = point_y + radius;
		}
		else return 0;
	}
	if (y > point_y + radius || y < point_y - radius) return 0;
	else if (y_min >= y_max) { if (y < y_min && y > y_max) return 0; }
	else if (y >= y_max || y <= y_min) return 0;

	int right_x = point_x + (int)(sqrt(radius * radius - (y - point_y) * (y - point_y)));
	return (right_x ? right_x : 1);
}

static inline double ellipse_relative_x(const int radius_a, const int radius_b, const double relative_y)
{
	if (relative_y >= radius_b)
		return 0;
	double relative_x = radius_a * sqrt(1 - relative_y * relative_y / radius_b / radius_b);
	return relative_x;
}

static inline void rotation_relative(const int rotation_angles, double & relative_x, double & relative_y)
{
	const double x = relative_x, y = relative_y;
	const double rotation_rads = deg2rad(rotation_angles);
	relative_x = x * cos(rotation_rads) - y * sin(rotation_rads);
	relative_y = x * sin(rotation_rads) + y * cos(rotation_rads);
	return;
}

/* ��������˼�����������������ʵ�֣���Ҫ�Ķ� */
/***************************************************************************
  �������ƣ�
  ��    �ܣ���ʼ��
  ���������const int bgcolor������ɫ
            const int fgcolor��ǰ��ɫ
			const int width  ����Ļ��ȣ�����
			const int high   ����Ļ�߶ȣ�����
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* ���ͷţ���ֹ��release���ٴ�init��hdc_release�����룩 */
	hdc_release();

	/* ����init����һ����̬ȫ������¼������hdc_cls()���õ� */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("��������", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //������Ŀ�ȼ��߶�ת��Ϊ�ض��������������!!�ĺ��壺�������8/16�ı��������ж�+1
	cct_cls();
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��ͷŻ�ͼ��Դ
  ���������
  �� �� ֵ��
  ˵    ����������
***************************************************************************/
void hdc_release()
{
	if (hdc) {
		ReleaseDC(hWnd, hdc);
		hdc = NULL;
		cct_setcursor(CURSOR_VISIBLE_NORMAL);
	}
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGBֵ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����û�����ɫ������RGB��ɫ��ֵ0-255
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ������Ļ�����е�ͼ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_cls()
{
    /* ����һ��Ҫ��һ����ɫ��ʼ������������ص㣬�ҵ�����㷽����ͬѧ����֪ͨ�� */
	hdc_init(_BgColor_, (_FgColor_  + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* ���ֻ���������demoʱ�������hdc_cls()���һ������ȱ�ߵ����������ʱ����
	   ������ֻ������л������⣬���ߴ���ʱֵ */
	Sleep(30);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ����һ�����ص�
  ���������const int x��x���꣬���Ͻ�Ϊ(0,0)
            const int y��y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨�������ڵ��ô�ָ��
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x1,y1)-(x2,y2)֮�仭��һ�����ص������
  ���������const int x1�����x���꣬���Ͻ�Ϊ(0,0)
			const int y1�����y���꣬���Ͻ�Ϊ(0,0)
			const int x2���յ�y���꣬���Ͻ�Ϊ(0,0)
			const int y2���յ�y���꣬���Ͻ�Ϊ(0,0)
  �� �� ֵ��
  ˵    ������ɫֱ���õ�ǰ�趨
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ���(x,y)λ�ô���ָ����ɫ��һ��ָ����ϸ�ĵ�(�û�ʵ��Բ��ģ��)
  ���������const int x			��x���꣬���Ͻ�Ϊ(0,0)
            const int y			��y���꣬���Ͻ�Ϊ(0,0)
			const int thickness	����Ĵ�ϸ������λ1�����޲���(�����ᵼ�²���ȫ���)����ȱʡֵ��
			const int RGB_value	�������ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �����Ľ�����Ч��
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
	return;
}

/* -------------------------------------------------------------------------
	�������к�����ʵ�֣���������������׼�Ķ�
	1������Ҫ����ϵͳ�������й�ͼ�β����ĺ���
	2�����к�����ʵ�ֶ�����hdc_base_point/hdc_base_line���������
	3����һ�룬�ǲ������к�������Ҫ����������ʵ�ֹ��̣�Ӧ����ʵ����Щ������
	4�������ʵ������ӵĲ���
	5��ϵͳ�ĽǶ��뺯�������еĽǶȺ������180��
   ------------------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ����߶�
  ���������const int x1		������x
            const int y1		������y
			const int x2		���յ��x
			const int y2		���յ��y
			const int thickness	���߶εĴ�ϸ����ȱʡֵ��
			const int RGB_value	���߶ε���ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ���������������û���hdc_point����ϣ��ٶ�����
                  Ҳ������hdc_base_point/hdc_base_line����ϣ��ٶȿ죩
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

#if 1 //�������߷�
	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle0 = (x1 == x2 ? 90 : (int)(atan((y2 - y1) * 1.0 / (x2 - x1)) / PI * 180));
	int angle, level;
	int old_x = INT_MAX, old_y = INT_MAX, dx, dy;
	int count = 0;

	/* �û�Բ�ķ�ʽģ��ֵ� */
	for (level = 1; level <= tn_end; level++) {
		for (angle = angle0; angle <= angle0 + 180; angle++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			dx = + (int)(level * sin(angle * PI / 180));
			dy = - (int)(level * cos(angle * PI / 180));
		    
			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (dx != old_x || dy != old_y) {
				old_x = dx;
				old_y = dy;

				hdc_base_line(x1 + dx, y1 + dy, x2 + dx, y2 + dy);
				++count;
			}
		}
	}
#else //��ƽ��ֱ��
	double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	if (0 == distance) {
		hdc_base_point(x1, y1);
		return;
	}
	double dx = (x2 - x1) / distance;//��(x1,y1)��ʼ
	double dy = (y2 - y1) / distance;//��(x1,y1)��ʼ
	int wx = (int)(y2 - y1 ? tn / dy / 2 : 0);
	int wy = (int)(x2 - x1 ? - tn / dx / 2 : 0);

	double x, y;
	int x_int, y_int;
	int old_x = INT_MAX, old_y = INT_MAX;
	int count = 0;

	/* ���߶��ϱ���ÿ���������ص� */
	for (x = y = 0; x * x + y * y <= distance * distance; x += dx, y += dy) {
		/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
		x_int = x1 + (int)(x);
		y_int = y1 + (int)(y);
		
		/* ��dx��dy��Сʱ��x��y�仯��������int�͵�x_int/y_int�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
		if (x_int != old_x || y_int != old_y) {
			if (wx < x && wx < x2 - x1 - x || -wx < x && -wx < x2 - x1 - x)
			    hdc_base_line(x_int - wx, y_int, x_int + wx, y_int);
			if (wy < y && wy < y2 - y1 - y || -wy < y && -wy < y2 - y1 - y)
			    hdc_base_line(x_int, y_int - wy, x_int, y_int + wy);
			old_x = x_int;
			old_y = y_int;
			++count;
			++count;
		}
	}
#endif

#if 0
	/* �ſ���ע�ͣ����Կ���һ���������˼������������ */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�������������꣬��һ��������
  ���������const int x1		����1�����x
            const int y1		����1�����y
			const int x2		����2�����x
			const int y2		����2�����y
			const int x3		����3�����x
			const int y3		����3�����y
			bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness	���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value	����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    �������ж������Ƿ��ߣ�������ߣ�����һ��ֱ�߼���
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	//������
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	hdc_line(x3, y3, x1, y1, thickness, RGB_value);
	if (!filled)
		return;
	//�ڲ����
	//if (RGB_value != INVALID_RGB)
	//	hdc_set_pencolor(RGB_value);
#if 1  //��ʽ��䷨
	int y_min = y1 < y2 ? y1 : y2;
	y_min = y_min < y3 ? y_min : y3;
	int y_max = y1 > y2 ? y1 : y2;
	y_max = y_max > y3 ? y_max : y3;

	for (int y = y_min; y < y_max; y++) {
		int xf1 = find_line_x(x1, y1, x2, y2, y);
		int xf2 = find_line_x(x2, y2, x3, y3, y);
		int xf3 = find_line_x(x3, y3, x1, y1, y);
		if (!xf1 && !xf2 && !xf3) continue;
		else if (xf1 && xf2 && xf1 != xf2) hdc_base_line(xf1, y, xf2, y);
		else if (xf1 && xf3 && xf1 != xf3) hdc_base_line(xf1, y, xf3, y);
		else if (xf2 && xf3 && xf2 != xf3) hdc_base_line(xf2, y, xf3, y);
	}
#else  //�ֻ���䷨
	int X[4] = { 0, x1, x2, x3 }, Y[4] = { 0, y1, y2, y3 };
	int count = 0;
	//�ֻ�����
	for (int i = 0; i < 3; i++) {
		double distance = sqrt((X[1] - X[2]) * (X[1] - X[2]) + (Y[1] - Y[2]) * (Y[1] - Y[2]));
		if (distance <= thickness) return;//�����غϣ�ֱ���˳�
		double dx = (X[2] - X[1]) / distance;//��(x1,y1)��ʼ
		double dy = (Y[2] - Y[1]) / distance;//��(x1,y1)��ʼ

		double x, y;
		int x_int, y_int;
		int old_x = INT_MAX, old_y = INT_MAX;

		/* ���߶��ϱ���ÿ���������ص� */
		for (x = y = 0; x * x + y * y <= distance * distance; x += dx, y += dy) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x_int = X[1] + (int)(x);
			y_int = Y[1] + (int)(y);

			/* ��dx��dy��Сʱ��x��y�仯��������int�͵�x_int/y_int�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x_int != old_x || y_int != old_y) {
				hdc_base_line(x_int, y_int, X[3], Y[3]);
				old_x = x_int;
				old_y = y_int;
				++count;
			}
		}
		//��ʼ�ֻ�
		for (int j = 0; j < 4; j++) {
			X[j] = X[(j + 1) % 4];
			Y[j] = Y[(j + 1) % 4];
		}
	}
#endif
#if 0
	/* �ſ���ע�ͣ����Կ���������������˼������������ */
	cct_gotoxy(0, 42);
	printf("count=%d           ", count);
	getchar();
#endif
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰��ȡ��߶ȣ�����һ��������
  ���������const int left_up_x			�����Ͻ�x
            const int left_up_y			�����Ͻ�y
			const int width				�����
			const int high				���߶�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ����
			bool filled					���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_rectangle(const int left_up_x, const int left_up_y, const int width, const int high, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	const int width_x = (int)(width * cos(deg2rad(rotation_angles)));
	const int width_y = (int)(width * sin(deg2rad(rotation_angles)));
	const int high_x = (int)(high * cos(deg2rad(rotation_angles + 90)));
	const int high_y = (int)(high * sin(deg2rad(rotation_angles + 90)));
	const int right_up_x = left_up_x + width_x;
	const int right_up_y = left_up_y + width_y;
	const int left_down_x = left_up_x + high_x;
	const int left_down_y = left_up_y + high_y;
	const int right_down_x = right_up_x + high_x;
	const int right_down_y = right_up_y + high_y;
	//������
	hdc_line(left_up_x, left_up_y, right_up_x, right_up_y, thickness, RGB_value);
	hdc_line(left_up_x, left_up_y, left_down_x, left_down_y, thickness, RGB_value);
	hdc_line(right_up_x, right_up_y, right_down_x, right_down_y, thickness, RGB_value);
	hdc_line(left_down_x, left_down_y, right_down_x, right_down_y, thickness, RGB_value);
	if (!filled)
		return;
	//��ʼ���
#if 1  //��ʽ��䷨
	int y_min = left_up_y < right_up_y ? left_up_y : right_up_y;
	y_min = y_min < left_down_y ? y_min : left_down_y;
	y_min = y_min < right_down_y ? y_min : right_down_y;
	int y_max = left_up_y > right_up_y ? left_up_y : right_up_y;
	y_max = y_max > left_down_y ? y_max : left_down_y;
	y_max = y_max > right_down_y ? y_max : right_down_y;

	for (int y = y_min; y < y_max; y++) {
		int x1 = find_line_x(left_up_x, left_up_y, right_up_x, right_up_y, y);
		int x2 = find_line_x(left_up_x, left_up_y, left_down_x, left_down_y, y);
		int x3 = find_line_x(right_up_x, right_up_y, right_down_x, right_down_y, y);
		int x4 = find_line_x(left_down_x, left_down_y, right_down_x, right_down_y, y);
		if (!x1 && !x2 && !x3 && !x4) continue;
		else if (x1 && x2 && x1 != x2) hdc_base_line(x1, y, x2, y);
		else if (x1 && x3 && x1 != x3) hdc_base_line(x1, y, x3, y);
		else if (x1 && x4 && x1 != x4) hdc_base_line(x1, y, x4, y);
		else if (x2 && x3 && x2 != x3) hdc_base_line(x2, y, x3, y);
		else if (x2 && x4 && x2 != x4) hdc_base_line(x2, y, x4, y);
		else if (x3 && x4 && x3 != x4) hdc_base_line(x3, y, x4, y);
	}
#else  //ԭʼ��䷨
#if 0   //б����䷨
	int X[] = { 0, left_up_x, left_down_x }, Y[] = { 0, left_up_x, left_down_y };
	int dx = width_x, dy = width_y;
	for (int i = 0; i < 2; ++i) {
		double distance = sqrt((X[1] - X[2]) * (X[1] - X[2]) + (Y[1] - Y[2]) * (Y[1] - Y[2]));
		if (distance <= thickness) return;//�����غϣ�ֱ���˳�
		double dx = (X[2] - X[1]) / distance;//��(x1,y1)��ʼ
		double dy = (Y[2] - Y[1]) / distance;//��(x1,y1)��ʼ

		double x, y;
		int x_int, y_int;
		int old_x = INT_MAX, old_y = INT_MAX;
		int count = 0;

		/* ���߶��ϱ���ÿ���������ص� */
		for (x = y = 0; x * x + y * y <= distance * distance; x += dx, y += dy) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x_int = X[1] + (int)(x);
			y_int = Y[1] + (int)(y);

			/* ��dx��dy��Сʱ��x��y�仯��������int�͵�x_int/y_int�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x_int != old_x || y_int != old_y) {
				hdc_base_line(x_int, y_int, x_int + width_x, y_int + width_y);
				old_x = x_int;
				old_y = y_int;
				++count;
			}
		}

		X[2] = right_up_x, Y[2] = right_up_y;
		dx = high_x, dy = width_y;
	}
#else    //��������䷨
	hdc_triangle(left_up_x, left_up_y, right_up_x, right_up_y, right_down_x, right_down_y, true, thickness, RGB_value);
	hdc_triangle(left_up_x, left_up_y, left_down_x, left_down_y, right_down_x, right_down_y, true, thickness, RGB_value);
#endif
#endif
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������Ͻ����꼰�߳�������һ��������
  ���������const int left_up_x			�����Ͻ�x
			const int left_up_y			�����Ͻ�y
			const int length			���߳�
			const int rotation_angles	�������Ͻ�Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			���ߵĴ�ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ��
  ���������const int point_x		��Բ�ĵ�x
            const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	int tn = thickness;
	/* ������ֱ�������ޣ����ǵ����޳���һ����С�ǣ�������Բ����λ��δʵ�� */

	const int tn_end = (tn <= 1) ? 0 : tn / 2;
	int angleBEG = ((angle_begin % 360) + 360) % 360;
	int angleEND = ((angle_end % 360) + 360) % 360;
	if (angleEND <= angleBEG) angleEND += 360;

	double angle;
	int level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	for (angle = angleBEG ; angle <= angleEND ; angle += 0.1) {
		for (level = radius - tn_end; level <= radius + tn_end; level++) {
			/* ע�⣬ϵͳ�������ᣬ0����Բ�����·� */
			x1 = point_x + (int)(level * sin(deg2rad(arc2sys_ang(angle))));
			y1 = point_y - (int)(level * cos(deg2rad(arc2sys_ang(angle))));

			/* ���뾶��Сʱ���Ƕȱ仯��������int�͵�x1/y1�仯����˼��ж���䣬�����ظ���ͬһ���ص� */
			if (x1 != old_x1 || y1 != old_y1) {
				hdc_base_point(x1, y1);
				old_x1 = x1;
				old_y1 = y1;
				++count;
			}
		}
	}
#if 0
	/* �ſ���ע�ͣ����Կ���һ���ֵ����˼������ص���� */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ������
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const int angle_begin	����ʼ�Ƕȣ���Բ�����Ϸ�Ϊ0�㣬˳ʱ��Ϊ����ע�⣺��ϵͳ�������180��!!!������ȱʡֵ��
			const int angle_end		�������Ƕȣ�ͬ�ϣ�����ȱʡֵ��
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  ���������
  �� �� ֵ��
  ˵    ��������ʼ/�����ǶȲ�ֵΪ360�ı���ʱ������������
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	//������
	const int begin_x = point_x + (int)(radius * sin(deg2rad(arc2sys_ang(angle_begin))));
	const int begin_y = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angle_begin))));
	const int end_x = point_x + (int)(radius * sin(deg2rad(arc2sys_ang(angle_end))));
	const int end_y = point_y - (int)(radius * cos(deg2rad(arc2sys_ang(angle_end))));
	if ((angle_begin - angle_end) % 360) {
		hdc_line(point_x, point_y, begin_x, begin_y, thickness, RGB_value);
		hdc_line(point_x, point_y, end_x, end_y, thickness, RGB_value);
	}
	hdc_arc(point_x, point_y, radius, angle_begin, angle_end, thickness, RGB_value);
	if (!filled)
		return;
	//��ʼ���
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
#if 1  //��ʽ��䷨
	for (int y = point_y - radius; y < point_y + radius; y++) {
		int x3 = find_arc_left_x(point_x, point_y, radius, angle_begin, angle_end, y);
		int x4 = find_arc_right_x(point_x, point_y, radius, angle_begin, angle_end, y);
		if (!((angle_begin - angle_end) % 360) && x3 && x4) hdc_base_line(x3, y, x4, y);

		int x1 = find_line_x(point_x, point_y, begin_x, begin_y, y);
		int x2 = find_line_x(point_x, point_y, end_x, end_y, y);
		if (!x1 && !x2 && !x3 && !x4) continue;
		else if (x1 && x2 && x3 && x4) {
			hdc_base_line(x3, y, (x1 <= x2 ? x1 : x2), y);
			hdc_base_line(x4, y, (x1 >= x2 ? x1 : x2), y);
		}
		else if (x3 && x4 && x3 != x4) hdc_base_line(x3, y, x4, y);
		else if (x1 && x2 && x1 != x2) hdc_base_line(x1, y, x2, y);
		else if (x1 && x3 && x1 != x3) hdc_base_line(x1, y, x3, y);
		else if (x1 && x4 && x1 != x4) hdc_base_line(x1, y, x4, y);
		else if (x2 && x3 && x2 != x3) hdc_base_line(x2, y, x3, y);
		else if (x2 && x4 && x2 != x4) hdc_base_line(x2, y, x4, y);
	}
#else  //ԭʼ��䷨
	for (int ra = 0; ra < radius; ra++)
		hdc_arc(point_x, point_y, ra, angle_begin, angle_end, 0, RGB_value);
#endif
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ��Բ
  ���������const int point_x		��Բ�ĵ�x
			const int point_y		��Բ�ĵ�y
			const int radius		���뾶
			const bool filled		���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness		����ϸ����ȱʡֵ��
			const int RGB_value		����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 0, filled, thickness, RGB_value);
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ����ո����Ĳ�����һ����Բ
  ���������const int point_x			��Բ�ĵ�x
			const int point_y			��Բ�ĵ�y
			const int radius_a			��ƽ����X��İ뾶
			const int radius_b			��ƽ����Y��İ뾶
			const int rotation_angles	����Բ��Ϊ֧�㣬��x�����ת��ǣ�����תΪ��������ȱʡֵ��
			const bool filled			���Ƿ���Ҫ��䣨��ȱʡֵ��
			const int thickness			����ϸ����ȱʡֵ��
			const int RGB_value			����ɫ����ȱʡֵ��
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//��������

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	double relative_y;
	double pos_x, neg_x, pos_y, neg_y;
	int old_neg_x = 0, old_neg_y = 0, old_pos_x = 0, old_pos_y = 0;
	for (relative_y = radius_b; relative_y >= -radius_b; relative_y -= 1) {
		neg_y = pos_y = relative_y;
		pos_x = ellipse_relative_x(radius_a, radius_b, relative_y);
		neg_x = -pos_x;
		rotation_relative(rotation_angles, pos_x, pos_y);
		rotation_relative(rotation_angles, neg_x, neg_y);
		if (filled)
		    hdc_line(point_x + (int)neg_x, point_y + (int)neg_y, point_x + (int)pos_x, point_y + (int)pos_y, 4);
		else {  //��ֱ����
			if (old_neg_x && old_neg_y && old_pos_x && old_pos_y) {
				hdc_line(old_pos_x, old_pos_y, point_x + (int)pos_x, point_y + (int)pos_y, thickness);
				hdc_line(old_neg_x, old_neg_y, point_x + (int)neg_x, point_y + (int)neg_y, thickness);
			}
			//hdc_point(point_x + (int)neg_x, point_y + (int)neg_y, thickness);
			//hdc_point(point_x + (int)pos_x, point_y + (int)pos_y, thickness);
			old_neg_x = point_x + (int)neg_x;
			old_neg_y = point_y + (int)neg_y;
			old_pos_x = point_x + (int)pos_x;
			old_pos_y = point_y + (int)pos_y;
		}
	}
	return;
}

#endif !HDC_SERIES_BY_TEACHER
