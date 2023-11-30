/* 2252201 胡世成 计科2班 */

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <Windows.h>
#include "../include/cmd_hdc_tools.h"
using namespace std;

#if !HDC_SERIES_BY_TEACHER	//未定义的情况下才打开条件编译

/* 允许添加需要的头文件 */

extern "C" WINBASEAPI HWND WINAPI GetConsoleWindow(); //VS中此处会有波浪线，不影响正常使用

/* 已定义的静态全局变量 */
static HWND hWnd = GetConsoleWindow();
static HDC hdc = NULL;
static const double PI = 3.14159;
static int _BgColor_, _FgColor_, _Width_, _High;
static int _Delay_ms;

/* 允许适度使用静态全局变量，但仅限static，即外部不可见 */

/* 此处允许添加自定义函数，但仅限static，即外部不可见 */
//角度转弧度
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
		return 0; //找不到
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

/* 下面给出了几个基本函数的完整实现，不要改动 */
/***************************************************************************
  函数名称：
  功    能：初始化
  输入参数：const int bgcolor：背景色
            const int fgcolor：前景色
			const int width  ：屏幕宽度（点阵）
			const int high   ：屏幕高度（点阵）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_init(const int bgcolor, const int fgcolor, const int width, const int high)
{
	/* 先释放，防止不release而再次init（hdc_release可重入） */
	hdc_release();

	/* 窗口init后，用一个静态全局量记录，后续hdc_cls()会用到 */
	_BgColor_ = bgcolor;
	_FgColor_ = fgcolor;
	_Width_ = width;
	_High = high;

	hdc = GetDC(hWnd);

	cct_setcursor(CURSOR_INVISIBLE);
	cct_setcolor(bgcolor, fgcolor);
	cct_setfontsize("点阵字体", 16);
	cct_setconsoleborder(width / 8 + !!(width % 8), high / 16 + !!(high % 16)); //将点阵的宽度及高度转换为特定字体的行列数，!!的含义：如果不是8/16的倍数，行列多+1
	cct_cls();
}

/***************************************************************************
  函数名称：
  功    能：释放画图资源
  输入参数：
  返 回 值：
  说    明：可重入
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
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_delay(int ms)
{
	if (ms > 0)
		_Delay_ms = ms;
	else
		_Delay_ms = 0;
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB值
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const int RGB_value)
{
	SelectObject(hdc, GetStockObject(DC_PEN));
	SetDCPenColor(hdc, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：设置画笔颜色，传入RGB三色，值0-255
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_set_pencolor(const unsigned char red, const unsigned char green, const unsigned char blue)
{
	hdc_set_pencolor(RGB(red, green, blue));
}

/***************************************************************************
  函数名称：
  功    能：清除屏幕上现有的图形
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_cls()
{
    /* 发现一定要换一种颜色初始化才能清除像素点，找到更简便方法的同学可以通知我 */
	hdc_init(_BgColor_, (_FgColor_  + 1) % 16, _Width_, _High);

	hdc_init(_BgColor_, _FgColor_, _Width_, _High);

	/* 部分机器上运行demo时，会出现hdc_cls()后第一根线有缺线的情况，加延时即可
	   如果部分机器运行还有问题，调高此延时值 */
	Sleep(30);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画出一个像素点
  输入参数：const int x：x坐标，左上角为(0,0)
            const int y：y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定，可以在调用处指定
***************************************************************************/
static inline void hdc_base_point(const int x, const int y)
{
	MoveToEx(hdc, x - 1, y - 1, NULL);
	LineTo(hdc, x, y);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x1,y1)-(x2,y2)之间画出一个像素点的连线
  输入参数：const int x1：起点x坐标，左上角为(0,0)
			const int y1：起点y坐标，左上角为(0,0)
			const int x2：终点y坐标，左上角为(0,0)
			const int y2：终点y坐标，左上角为(0,0)
  返 回 值：
  说    明：颜色直接用当前设定
***************************************************************************/
static inline void hdc_base_line(const int x1, const int y1, const int x2, const int y2)
{
	MoveToEx(hdc, x1, y1, NULL);
	LineTo(hdc, x2, y2);
	if (_Delay_ms > 0)
		Sleep(_Delay_ms);
}

/***************************************************************************
  函数名称：
  功    能：在(x,y)位置处用指定颜色画一个指定粗细的点(用画实心圆来模拟)
  输入参数：const int x			：x坐标，左上角为(0,0)
            const int y			：y坐标，左上角为(0,0)
			const int thickness	：点的粗细，下限位1，上限不限(如过大会导致不完全填充)（有缺省值）
			const int RGB_value	：点的颜色（有缺省值）
  返 回 值：
  说    明：改进运行效率
***************************************************************************/
void hdc_point(const int x, const int y, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

	const int tn_end = (tn <= 1) ? 1 : tn/2;

	int angle, level;
	int old_x1 = INT_MAX, old_y1 = INT_MAX, x1, y1;
	int count = 0;

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = 0; angle <= 360; angle ++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = x + (int)(level * sin(angle * PI / 180));
			y1 = y - (int)(level * cos(angle * PI / 180));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				old_x1 = x1;
				old_y1 = y1;
				hdc_base_point(x1, y1);
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
	return;
}

/* -------------------------------------------------------------------------
	给出下列函数的实现，函数名及参数表不准改动
	1、不需要调用系统的其他有关图形操作的函数
	2、下列函数的实现都基于hdc_base_point/hdc_base_line函数的组合
	3、想一想，是不是所有函数都需要给出独立的实现过程？应该先实现哪些函数？
	4、填充是实现中最复杂的部分
	5、系统的角度与函数参数中的角度含义相差180°
   ------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：画线段
  输入参数：const int x1		：起点的x
            const int y1		：起点的y
			const int x2		：终点的x
			const int y2		：终点的y
			const int thickness	：线段的粗细（有缺省值）
			const int RGB_value	：线段的颜色（有缺省值）
  返 回 值：
  说    明：本函数可以用基于hdc_point的组合（速度慢）
                  也可以用hdc_base_point/hdc_base_line的组合（速度快）
***************************************************************************/
void hdc_line(const int x1, const int y1, const int x2, const int y2, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限

	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);

#if 1 //两端连线法
	const int tn_end = (tn <= 1) ? 1 : tn / 2;

	int angle0 = (x1 == x2 ? 90 : (int)(atan((y2 - y1) * 1.0 / (x2 - x1)) / PI * 180));
	int angle, level;
	int old_x = INT_MAX, old_y = INT_MAX, dx, dy;
	int count = 0;

	/* 用画圆的方式模拟粗点 */
	for (level = 1; level <= tn_end; level++) {
		for (angle = angle0; angle <= angle0 + 180; angle++) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			dx = + (int)(level * sin(angle * PI / 180));
			dy = - (int)(level * cos(angle * PI / 180));
		    
			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (dx != old_x || dy != old_y) {
				old_x = dx;
				old_y = dy;

				hdc_base_line(x1 + dx, y1 + dy, x2 + dx, y2 + dy);
				++count;
			}
		}
	}
#else //横平竖直法
	double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	if (0 == distance) {
		hdc_base_point(x1, y1);
		return;
	}
	double dx = (x2 - x1) / distance;//从(x1,y1)开始
	double dy = (y2 - y1) / distance;//从(x1,y1)开始
	int wx = (int)(y2 - y1 ? tn / dy / 2 : 0);
	int wy = (int)(x2 - x1 ? - tn / dx / 2 : 0);

	double x, y;
	int x_int, y_int;
	int old_x = INT_MAX, old_y = INT_MAX;
	int count = 0;

	/* 在线段上遍历每个相邻像素点 */
	for (x = y = 0; x * x + y * y <= distance * distance; x += dx, y += dy) {
		/* 注意，系统的坐标轴，0°在圆的最下方 */
		x_int = x1 + (int)(x);
		y_int = y1 + (int)(y);
		
		/* 当dx或dy很小时，x或y变化不会引起int型的x_int/y_int变化，因此加判断语句，避免重复画同一像素点 */
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
	/* 放开此注释，可以看到一个粗线用了几条像素线组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
	return;
}

/***************************************************************************
  函数名称：
  功    能：给出三点的坐标，画一个三角形
  输入参数：const int x1		：第1个点的x
            const int y1		：第1个点的y
			const int x2		：第2个点的x
			const int y2		：第2个点的y
			const int x3		：第3个点的x
			const int y3		：第3个点的y
			bool filled			：是否需要填充（有缺省值）
			const int thickness	：边的粗细（有缺省值）
			const int RGB_value	：颜色（有缺省值）
  返 回 值：
  说    明：不判断三点是否共线，如果共线，划出一条直线即可
***************************************************************************/
void hdc_triangle(const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, bool filled, const int thickness, const int RGB_value)
{
	//画边线
	hdc_line(x1, y1, x2, y2, thickness, RGB_value);
	hdc_line(x2, y2, x3, y3, thickness, RGB_value);
	hdc_line(x3, y3, x1, y1, thickness, RGB_value);
	if (!filled)
		return;
	//内部填充
	//if (RGB_value != INVALID_RGB)
	//	hdc_set_pencolor(RGB_value);
#if 1  //坚式填充法
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
#else  //轮换填充法
	int X[4] = { 0, x1, x2, x3 }, Y[4] = { 0, y1, y2, y3 };
	int count = 0;
	//轮换三次
	for (int i = 0; i < 3; i++) {
		double distance = sqrt((X[1] - X[2]) * (X[1] - X[2]) + (Y[1] - Y[2]) * (Y[1] - Y[2]));
		if (distance <= thickness) return;//两点重合，直接退出
		double dx = (X[2] - X[1]) / distance;//从(x1,y1)开始
		double dy = (Y[2] - Y[1]) / distance;//从(x1,y1)开始

		double x, y;
		int x_int, y_int;
		int old_x = INT_MAX, old_y = INT_MAX;

		/* 在线段上遍历每个相邻像素点 */
		for (x = y = 0; x * x + y * y <= distance * distance; x += dx, y += dy) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x_int = X[1] + (int)(x);
			y_int = Y[1] + (int)(y);

			/* 当dx或dy很小时，x或y变化不会引起int型的x_int/y_int变化，因此加判断语句，避免重复画同一像素点 */
			if (x_int != old_x || y_int != old_y) {
				hdc_base_line(x_int, y_int, X[3], Y[3]);
				old_x = x_int;
				old_y = y_int;
				++count;
			}
		}
		//开始轮换
		for (int j = 0; j < 4; j++) {
			X[j] = X[(j + 1) % 4];
			Y[j] = Y[(j + 1) % 4];
		}
	}
#endif
#if 0
	/* 放开此注释，可以看到填充三角形用了几条像素线组成 */
	cct_gotoxy(0, 42);
	printf("count=%d           ", count);
	getchar();
#endif
	return;
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及宽度、高度，画出一个长方形
  输入参数：const int left_up_x			：左上角x
            const int left_up_y			：左上角y
			const int width				：宽度
			const int high				：高度
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）
			bool filled					：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
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
	//画边线
	hdc_line(left_up_x, left_up_y, right_up_x, right_up_y, thickness, RGB_value);
	hdc_line(left_up_x, left_up_y, left_down_x, left_down_y, thickness, RGB_value);
	hdc_line(right_up_x, right_up_y, right_down_x, right_down_y, thickness, RGB_value);
	hdc_line(left_down_x, left_down_y, right_down_x, right_down_y, thickness, RGB_value);
	if (!filled)
		return;
	//开始填充
#if 1  //坚式填充法
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
#else  //原始填充法
#if 0   //斜线填充法
	int X[] = { 0, left_up_x, left_down_x }, Y[] = { 0, left_up_x, left_down_y };
	int dx = width_x, dy = width_y;
	for (int i = 0; i < 2; ++i) {
		double distance = sqrt((X[1] - X[2]) * (X[1] - X[2]) + (Y[1] - Y[2]) * (Y[1] - Y[2]));
		if (distance <= thickness) return;//两点重合，直接退出
		double dx = (X[2] - X[1]) / distance;//从(x1,y1)开始
		double dy = (Y[2] - Y[1]) / distance;//从(x1,y1)开始

		double x, y;
		int x_int, y_int;
		int old_x = INT_MAX, old_y = INT_MAX;
		int count = 0;

		/* 在线段上遍历每个相邻像素点 */
		for (x = y = 0; x * x + y * y <= distance * distance; x += dx, y += dy) {
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x_int = X[1] + (int)(x);
			y_int = Y[1] + (int)(y);

			/* 当dx或dy很小时，x或y变化不会引起int型的x_int/y_int变化，因此加判断语句，避免重复画同一像素点 */
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
#else    //三角形填充法
	hdc_triangle(left_up_x, left_up_y, right_up_x, right_up_y, right_down_x, right_down_y, true, thickness, RGB_value);
	hdc_triangle(left_up_x, left_up_y, left_down_x, left_down_y, right_down_x, right_down_y, true, thickness, RGB_value);
#endif
#endif
	return;
}

/***************************************************************************
  函数名称：
  功    能：给出左上角坐标及边长，画出一个正方形
  输入参数：const int left_up_x			：左上角x
			const int left_up_y			：左上角y
			const int length			：边长
			const int rotation_angles	：以左上角为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：边的粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hdc_square(const int left_up_x, const int left_up_y, const int length, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	hdc_rectangle(left_up_x, left_up_y, length, length, rotation_angles, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一段圆弧
  输入参数：const int point_x		：圆心的x
            const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_arc(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const int thickness, const int RGB_value)
{
	int tn = thickness;
	/* 不卡点直径的上限，但是单上限超过一定大小是，画出的圆部分位置未实心 */

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
			/* 注意，系统的坐标轴，0°在圆的最下方 */
			x1 = point_x + (int)(level * sin(deg2rad(arc2sys_ang(angle))));
			y1 = point_y - (int)(level * cos(deg2rad(arc2sys_ang(angle))));

			/* 当半径很小时，角度变化不会引起int型的x1/y1变化，因此加判断语句，避免重复画同一像素点 */
			if (x1 != old_x1 || y1 != old_y1) {
				hdc_base_point(x1, y1);
				old_x1 = x1;
				old_y1 = y1;
				++count;
			}
		}
	}
#if 0
	/* 放开此注释，可以看到一个粗点用了几个像素点组成 */
	cct_gotoxy(0, 41);
	printf("count=%d           ", count);
	getchar();
#endif
	return;
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个扇形
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const int angle_begin	：起始角度（以圆的最上方为0°，顺时针为正，注意：与系统坐标差了180°!!!）（有缺省值）
			const int angle_end		：结束角度（同上）（有缺省值）
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  输入参数：
  返 回 值：
  说    明：当起始/结束角度差值为360的倍数时，不画两条边
***************************************************************************/
void hdc_sector(const int point_x, const int point_y, const int radius, const int angle_begin, const int angle_end, const bool filled, const int thickness, const int RGB_value)
{
	//画边线
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
	//开始填充
	if (RGB_value != INVALID_RGB)
		hdc_set_pencolor(RGB_value);
#if 1  //坚式填充法
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
#else  //原始填充法
	for (int ra = 0; ra < radius; ra++)
		hdc_arc(point_x, point_y, ra, angle_begin, angle_end, 0, RGB_value);
#endif
	return;
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个圆
  输入参数：const int point_x		：圆心的x
			const int point_y		：圆心的y
			const int radius		：半径
			const bool filled		：是否需要填充（有缺省值）
			const int thickness		：粗细（有缺省值）
			const int RGB_value		：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_circle(const int point_x, const int point_y, const int radius, const bool filled, const int thickness, const int RGB_value)
{
	hdc_sector(point_x, point_y, radius, 0, 0, filled, thickness, RGB_value);
}

/***************************************************************************
  函数名称：
  功    能：按照给出的参数画一个椭圆
  输入参数：const int point_x			：圆心的x
			const int point_y			：圆心的y
			const int radius_a			：平行于X轴的半径
			const int radius_b			：平行于Y轴的半径
			const int rotation_angles	：以圆心为支点，与x轴的旋转倾角（向下转为正）（有缺省值）
			const bool filled			：是否需要填充（有缺省值）
			const int thickness			：粗细（有缺省值）
			const int RGB_value			：颜色（有缺省值）
  返 回 值：
  说    明：
***************************************************************************/
void hdc_ellipse(const int point_x, const int point_y, const int radius_a, const int radius_b, const int rotation_angles, const bool filled, const int thickness, const int RGB_value)
{
	int tn = thickness;
	if (tn < 1)
		tn = 1;		//处理下限

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
		else {  //以直代曲
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
