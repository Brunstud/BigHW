//2252201 胡世成 计科2班
//绝对原创 北部玄驹（但图是照着画的）
//::“今天的坚宝不是我的对手”
#include <iostream>
#include <Windows.h>
#include <cmath>
#include <ctime>
#include "../include/cmd_hdc_tools.h"

#define LIST_INIT_SIZE 10  //顺序表初始空间单元数
#define LISTINCREMENT 5    //顺序表递增空间单元数
#define END INT_MAX
#define MAX_P (INT_MAX / 2)
#define WAIT 1000          //演示时间间隔
#define PI          3.1415926	        //圆周率
#define SKIN_COLOR  RGB(253, 244, 226)  //默认肤色
#define HAIR_COLOR  RGB(71, 69, 78)     //默认发色

//中心点位置
static int base_x = 400;
static int base_y = 400;

/****************************************************************以下是辅助函数及类*******************************************************************************************/
//角度转弧度
static inline double deg2rad(const double angle)
{
	return angle * PI / 180;
}

//相对位置转画弧角度
static inline double pos2deg(const int x0, const int y0, const int x1, const int y1)
{
	const int dx = x1 - x0;
	const int dy = y1 - y0;
	if (!dx)
		if (!dy) return NULL;
		else if (dy > 0) return 180;
		else return 0;
	const double rad = atan(dy * 1.0 / dx);
	double angle = rad / PI * 180 + 90;
	if (dx < 0) angle += 180;
	return angle;
}

//根据端点及弧度画曲线——》被舍弃的方案
static void hdc_rcurve(const int x1, const int y1, const int x2, const int y2, const int dline, const int thickness = 2, const int RGB_value = INVALID_RGB)//radius为曲率半径可正可负
{
	const double distance = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
	if (!distance) return;
	const int radius = (int)sqrt(dline * dline + distance * distance / 4);
	const double dx = (x2 - x1) / distance;
	const double dy = (y2 - y1) / distance;
	const int point_x = (int)((x1 + x2) / 2 + dline * dy);
	const int point_y = (int)((y1 + y2) / 2 - dline * dx);
	const int angle1 = (int)pos2deg(point_x, point_y, x1, y1);
	const int angle2 = (int)pos2deg(point_x, point_y, x2, y2);
	hdc_arc(point_x, point_y, radius, (dline < 0 ? angle1 : angle2), (dline < 0 ? angle2 : angle1), thickness, RGB_value);
}

//根据三个点画曲线——》经过改进后可以被CurveList类完全取代
static void hdc_pcurve(const int x1, const int y1, const int x3, const int y3, const int x2, const int y2, const int thickness = 2, const int RGB_value = INVALID_RGB)
{
	hdc_point(x1, y1, thickness, RGB_value);
	hdc_point(x2, y2, thickness, RGB_value);

	if (x1 == x2 || y1 == y2 || x2 == x3 || y2 == y3) {
		hdc_line(x1, y1, x2, y2, thickness, RGB_value);
		hdc_line(x2, y2, x3, y3, thickness, RGB_value);
		return;
	}

	// 计算中点和斜率
	double xAB = (x1 + x2) * 1.0 / 2;
	double yAB = (y1 + y2) * 1.0 / 2;
	double mAB = (y2 - y1) * 1.0 / (x2 - x1);
	double mMidAB = -1 / mAB;

	double xBC = (x2 + x3) * 1.0 / 2;
	double yBC = (y2 + y3) * 1.0 / 2;
	double mBC = (y3 - y2) * 1.0 / (x3 - x2);
	double mMidBC = -1 / mBC;

	if (mMidAB == mMidBC) {
		hdc_line(x1, y1, x2, y2, thickness, RGB_value);
		hdc_line(x2, y2, x3, y3, thickness, RGB_value);
		return;
	}

	// 计算中垂线方程
	double cMidAB = yAB - mMidAB * xAB;
	double cMidBC = yBC - mMidBC * xBC;

	// 解方程得到外心坐标
	int xCenter = (int)((cMidBC - cMidAB) / (mMidAB - mMidBC));
	int yCenter = (int)(mMidAB * xCenter + cMidAB);

	// 计算外接圆半径
	int radius = (int)sqrt(pow(x1 - xCenter, 2) + pow(y1 - yCenter, 2));
    
	int angle_beg, angle_end;
	const int angle1 = (int)pos2deg(xCenter, yCenter, x1, y1);
	const int angle2 = (int)pos2deg(xCenter, yCenter, x2, y2);

	const int yc = (int)(y1 + (x3 - x1) * (y2 - y1) * 1.0 / (x2 - x1));
	if (yc == y3 || radius > 400) {
		hdc_line(x1, y1, x2, y2, thickness * 2, RGB_value);
		hdc_line(x2, y2, x3, y3, thickness * 2, RGB_value);
		return;
	}
	if (yc < y3 && x1 > x2 || yc > y3 && x1 < x2) {  //???
		angle_beg = angle1;
		angle_end = angle2;
	}
	else {
		angle_beg = angle2;
		angle_end = angle1;
	}

	hdc_arc(xCenter, yCenter, radius, angle_beg - 1, angle_end + 1, thickness, RGB_value);
}

//储存点坐标的结构体,不可再增删元素或修改数据类型
struct Point {
	int x;
	int y;
};

//储存简单圆弧信息的结构体
struct Curve {
	Point center;
	double radius;
	Point point1;
	Point point2;
	int topY = 0;
	int botY = 0;
	int angle_beg = 0;
	int angle_end = 0;
	int above = 0;//当圆弧在两点连线上方时为1，否则为0
};

/*************************************************************************
类名：CurveList
作用：储存任意曲线簇信息的顺序表
初始：传入结构体数组格式为{端点1, 中间点1, 端点2, 中间点2, ...}
备注：初始化时至少传两个点：奇数个点视作不闭合曲线；偶数个点视作闭合曲线
**************************************************************************/
class CurveList {
	Curve* curvelist = NULL;
	int length = 0;
	int listsize = 0;
	int closed = 0;//判断是否闭合 

	//内置函数，用散点计算拟合曲线信息
	void calCurve(Curve *curve, Point point1, const Point pointm, const Point point2)
	{
		//读取端点，默认直线设置
		curve->point1 = point1;
		curve->point2 = point2;
		curve->center.x = (point1.x + point2.x) / 2;
		curve->center.y = (point1.y + point2.y) / 2;
		curve->radius = NULL;//直线时设置radius为NULL
		curve->angle_beg = curve->angle_end = curve->above = 0;//默认置零
		int topX, botX;
		if (point1.y > point2.y) {
			curve->topY = point1.y;
			curve->botY = point2.y;
			topX = point1.x;
			botX = point2.x;
		}
		else {
			curve->topY = point2.y;
			curve->botY = point1.y;
			topX = point2.x;
			botX = point1.x;
		}
		
		//判断是否为直线
		if ((point1.x - pointm.x) * (point2.y - pointm.y) == (point2.x - pointm.x) * (point1.y - pointm.y))
			return;

		// 计算中点和斜率
		Point p[4] = { point1, point2, pointm, point1 };
		Point* pp = p;
		if (pp->y == (pp + 1)->y)
			++pp;
		//风险判断
		if (pp->y == (pp + 1)->y)
			return;
		double xAB = (pp->x + (pp + 1)->x) * 1.0 / 2;
		double yAB = (pp->y + (pp + 1)->y) * 1.0 / 2;
		double mMidAB = -1.0 * ((pp + 1)->x - pp->x) / ((pp + 1)->y - pp->y);
		
		pp++;//计算下一组中点及斜率
		if (pp->y == (pp + 1)->y)
			++pp;
		//风险判断
		if (pp - p >= 3 || pp->y == (pp + 1)->y)
			return;
		double xBC = (pp->x + (pp + 1)->x) * 1.0 / 2;
		double yBC = (pp->y + (pp + 1)->y) * 1.0 / 2;
		double mMidBC = -1.0 * ((pp + 1)->x - pp->x) / ((pp + 1)->y - pp->y);

		// 计算中垂线方程
		double cMidAB = yAB - mMidAB * xAB;
		double cMidBC = yBC - mMidBC * xBC;

		//风险判断
		if (!(mMidAB - mMidBC))
			return;
		// 解方程得到外心坐标
		curve->center.x = (int)round((cMidBC - cMidAB) / (mMidAB - mMidBC));
		curve->center.y = (int)round(mMidAB * curve->center.x + cMidAB);

		// 计算外接圆半径
		const double radi = sqrt(pow(point1.x - curve->center.x, 2) + pow(point1.y - curve->center.y, 2));
		if (radi > 600)//直线判断
			return;
		curve->radius = radi;

		//设置angle及above
		//设置topY及botY，用于填充
		const int angle1 = (int)pos2deg(curve->center.x, curve->center.y, point1.x, point1.y);
		const int angle2 = (int)pos2deg(curve->center.x, curve->center.y, point2.x, point2.y);
		if (point1.x == point2.x) {
			const int xc = point1.x;
			if (xc == pointm.x)//直线判断
				return;
			if (pointm.x > xc)
				curve->above = 1;
			if (pointm.x > xc && point1.y < point2.y || pointm.x < xc && point1.y > point2.y) {
				curve->angle_beg = angle1;
				curve->angle_end = angle2;
			}
			else {
				curve->angle_beg = angle2;
				curve->angle_end = angle1;
			}
			if (curve->above && xc < curve->center.x || !curve->above && xc > curve->center.x) {
				curve->topY = curve->center.y + (int)round(curve->radius);
				curve->botY = curve->center.y - (int)round(curve->radius);
			}
		}
		else {
			const int yc = (int)(point1.y + (pointm.x - point1.x) * (point2.y - point1.y) * 1.0 / (point2.x - point1.x));
			if (yc == pointm.y)//直线判断
				return;
			if (pointm.y > yc)
				curve->above = 1;
			if (pointm.y > yc && point1.x > point2.x || pointm.y < yc && point1.x < point2.x) {  //???
				curve->angle_beg = angle1;
				curve->angle_end = angle2;
			}
			else {
				curve->angle_beg = angle2;
				curve->angle_end = angle1;
			}
			if (curve->above && (topX < curve->center.x && botX > topX || topX > curve->center.x && botX < topX))
				curve->topY = curve->center.y + (int)round(curve->radius);
			if (!curve->above && (botX < curve->center.x && topX > botX || botX > curve->center.x && topX < botX))
				curve->botY = curve->center.y - (int)round(curve->radius);
		}

		//结束拟合
		return;
	}
	//内置函数，辅助填充函数
	// Point& ret中返回两个解
	//如果没找到，返回NULL
	int find_curve_x(const Curve* curve, Point* ret, const int y)
	{
		ret->x = ret->y = 0;
		if (y > curve->topY || y < curve->botY || curve->topY == curve->botY)
			return 0;//找不到
		if (!curve->radius) {
			const int x0 = (int)round(curve->point1.x + (y - curve->point1.y) * (curve->point2.x - curve->point1.x) * 1.0 / (curve->point2.y - curve->point1.y)) + (y == curve->topY ? MAX_P : 0);
			ret->x = (x0 ? x0 : 1);
			return 1;
		}
		else {
			//保证端点一定有值返回
			if (abs(y - curve->point1.y) <= 2 && abs(y - curve->point2.y) > 2 && 
				((curve->above && (curve->center.x - curve->point1.x) * (curve->point2.x - curve->point1.x) > 0 ||
				  !curve->above && (curve->center.x - curve->point1.x) * (curve->point2.x - curve->point1.x) <  0) ? y > curve->point1.y : y < curve->point1.y))
				if (curve->point1.x != curve->center.x)
					if (curve->point1.x < curve->center.x) ret->x = (curve->point1.x ? curve->point1.x : 1); 
					else ret->y = (curve->point1.x ? curve->point1.x : 1);
			if (abs(y - curve->point2.y) <= 2 && abs(y - curve->point1.y) > 2 &&
				((curve->above && (curve->center.x - curve->point2.x) * (curve->point1.x - curve->point2.x) > 0 ||
					!curve->above && (curve->center.x - curve->point2.x) * (curve->point1.x - curve->point2.x) < 0) ? y > curve->point2.y : y < curve->point2.y))
				if (curve->point2.x != curve->center.x)
					if (curve->point2.x <= curve->center.x) ret->x = (curve->point2.x ? curve->point2.x : 1); 
					else ret->y = (curve->point2.x ? curve->point2.x : 1);
			//正常情况计算
			const int dx = (int)round(sqrt(curve->radius * curve->radius - (y - curve->center.y) * (y - curve->center.y)));
			const int xLeft = curve->center.x - dx;
			const int xRight = curve->center.x + dx;
			if (curve->point1.x == curve->point2.x) {
				if (curve->above && xLeft >= curve->point1.x || !curve->above && xLeft <= curve->point1.x)
					ret->x = (xLeft ? xLeft : 1);
				if (curve->above && xRight >= curve->point1.x || !curve->above && xRight <= curve->point1.x)
					ret->y = (xRight ? xRight : 1);
			}
			else {
				const int yLeft = (int)round(curve->point1.y + (xLeft - curve->point1.x) * (curve->point2.y - curve->point1.y) * 1.0 / (curve->point2.x - curve->point1.x));
				const int yRight = (int)round(curve->point1.y + (xRight - curve->point1.x) * (curve->point2.y - curve->point1.y) * 1.0 / (curve->point2.x - curve->point1.x));
				if (curve->above && y >= yLeft || !curve->above && y <=  yLeft)
					ret->x = (xLeft ? xLeft : 1);
				if (curve->above && y >= yRight || !curve->above && y <= yRight)
					ret->y = (xRight ? xRight : 1);
			}
			//保证端点一定有值返回
			if (y == curve->point1.y)
				if (curve->point1.x <= curve->center.x) ret->x = (curve->point1.x ? curve->point1.x : 1) + (y == curve->topY || !curve->above ? MAX_P : 0);
				else ret->y = (curve->point1.x ? curve->point1.x : 1) + (y == curve->topY || !curve->above ? MAX_P : 0);
			if (y == curve->point2.y)
				if (curve->point2.x <= curve->center.x) ret->x = (curve->point2.x ? curve->point2.x : 1) + (y == curve->topY || !curve->above ? MAX_P : 0);
				else ret->y = (curve->point2.x ? curve->point2.x : 1) + (y == curve->topY || !curve->above ? MAX_P : 0);
			return 1;
		}
	}
	//渐变色彩
	//ratio = 0时返回RGB_color1，ratio = 1时返回RGB_color2
	int RGB_shade(const double ratio, const int RGB_value1, const int RGB_value2 = INVALID_RGB)
	{
		if (RGB_value1 == RGB_value2 || RGB_value2 == INVALID_RGB)
			return RGB_value1;
		if (ratio <= 0)
			return RGB_value1;
		if (ratio >= 1)
			return RGB_value2;
		const int R1 = ((BYTE)RGB_value1);
		const int G1 = ((BYTE)(((WORD)(RGB_value1))>>8));
		const int B1 = ((BYTE)((RGB_value1)>>16));
		const int R2 = ((BYTE)RGB_value2);
		const int G2 = ((BYTE)(((WORD)(RGB_value2)) >> 8));
		const int B2 = ((BYTE)((RGB_value2) >> 16));
		const int RGB_final = RGB(int(R1 * (1 - ratio) + R2 * ratio), int(G1 * (1 - ratio) + G2 * ratio), int(B1 * (1 - ratio) + B2 * ratio));
		return RGB_final;
	}
public:
	//判断是否闭合
	int close() { return (length > 0 ? closed : -1); };
	//作用：用离散点初始拟合一个曲线簇
	//参数：传入结构体数组格式为{端点1, 中间点1, 端点2, 中间点2, ...}
	//返回：空间分配失败返回-1；点不组构成拟合返回-2；正常拟合返回0
	int init(Point* point)
	{
		//分配初始空间
		curvelist = (Curve*)malloc(LIST_INIT_SIZE * sizeof(Curve));
		if (!curvelist) return -1;
		length = 0;
		listsize = LIST_INIT_SIZE;
		closed = 0;//默认不闭合

		//开始拟合曲线
		const Point* firstp = point;
		while ((point->x || point->y) && ((point+1)->x || (point+1)->y)) {
			//增加分配内存
			if (length >= listsize) {
				Curve* newbase = (Curve*)realloc(curvelist, (listsize + LISTINCREMENT) * sizeof(Curve));
				if (!newbase) return -1;
				curvelist = newbase;
				listsize += LISTINCREMENT;
			}
			//判断是否闭合
			if (!((point + 2)->x) && !((point + 2)->y) && length > 0) {
				closed = 1;//判断为闭合曲线
				calCurve(curvelist + length, *point, *(point + 1), *firstp);
				++length;
				break;
			}
			//拟合赋值
			calCurve(curvelist + length, *point, *(point + 1), *(point + 2));
			++length;
			point += 2;//向后移动两个点
		}

		if (!length)//点数不足以初始拟合曲线
			return -2;
		return 0;
	}
	//作用：曲线描边
	//参数：thickness：曲线厚度；RGB_value：曲线颜色
	//返回：描边失败返回-1；曲线不足返回-2；正常返回0
	int draw(const int thickness = 2, const int RGB_value = INVALID_RGB) {
		if (!curvelist || !length)
			return -2;
		for (Curve* pcur = curvelist; pcur - curvelist < length; pcur++) {
			if (pcur->radius != NULL)
				hdc_arc(pcur->center.x, pcur->center.y, (int)pcur->radius, pcur->angle_beg, pcur->angle_end + 1, thickness, RGB_value);
			else
				hdc_line(pcur->point1.x, pcur->point1.y, pcur->point2.x, pcur->point2.y, thickness, RGB_value);
		}
		return 0;
	}
	//作用：填充闭合曲线
	//参数：RGB_value：曲线颜色(支持渐变填充)
	//返回：填充失败返回-1；曲线不足/曲线不闭合返回-2；正常返回0
	//注释：一定情况下会有漏点，略修改数据即可
	int fill(const int RGB_value1 = INVALID_RGB, const int RGB_value2 = INVALID_RGB) {
		if (!curvelist || !length || !closed)
			return -2;
		//寻找曲线上下确界
		int topY = INT_MIN, botY = INT_MAX;
		Curve* pcur = curvelist;
		for (; pcur - curvelist < length; pcur++) {
			if (pcur->topY > topY)
				topY = pcur->topY;
			if (pcur->botY < botY)
				botY = pcur->botY;
		}
		//开始填充
		Point* auxiliary = (Point*)malloc((length + 1) * sizeof(Point));
		if (!auxiliary)
			return -1;

		Point* paux = auxiliary;
		for (int y = botY; y <= topY; y++) {
     		for (pcur = curvelist, paux = auxiliary; pcur - curvelist < length; pcur++, paux++)
				find_curve_x(pcur, paux, y);
			//统计被返回的有效x值的个数
			int count = 0;
			for (int* pAuxINT = (int*)auxiliary; pAuxINT - (int*)auxiliary < length * 2; pAuxINT++)
				if (*pAuxINT)
					count++;
			//按从小到大重排列
			for (int* pAuxINT = (int*)auxiliary; pAuxINT - (int*)auxiliary < length * 2; pAuxINT++) {
				int* pMIN = NULL;
				for (int* pA = pAuxINT; pA - (int*)auxiliary < length * 2; pA++)
					if (!*pA)
						continue;
					else if (!pMIN || *pA % MAX_P < *pMIN % MAX_P)
						pMIN = pA;
				if (!pMIN)
					break;
				else if (pAuxINT > (int*)auxiliary && *pMIN % MAX_P == *(pAuxINT - 1) % MAX_P && *pMIN != *(pAuxINT - 1)) {//(count % 2 && pAuxINT > (int*)auxiliary && *pMIN == *(pAuxINT - 1)) {
					*pMIN = 0;
					pAuxINT--;
					count--;
					continue;
				}
				else if (pMIN == pAuxINT)
					continue;
				else {
					int tmp = *pAuxINT;
					*pAuxINT = *pMIN;
					*pMIN = tmp;
				}
			}
			//划线
			for (paux = auxiliary; paux->x && paux->y; paux++) {
				if (paux->x == paux->y)
						continue;
				hdc_line(paux->x % MAX_P, y, paux->y % MAX_P, y, 2, RGB_shade((y - botY) * 1.0 / (topY - botY), RGB_value1, RGB_value2));
			}
		}
		//完成填充，清理现场
		free(auxiliary);
		return 0;
	}
	//析构函数，释放空间
	~CurveList() { if (curvelist) free(curvelist); };
};

/*************************************************************************
类名：adaptor
作用：调节图形位置，大小及方向的类
**************************************************************************/
class adaptor {
	const int base_x;
	const int base_y;
	const double size;
	const double ad_rotation_ang;
	//内置函数，处理放缩及旋转
	template<typename T>
	inline T ad(T ori)
	{
		return T(ori * size);
	}
	inline void rotation_relative(int& x, int& y, const int relative_x, const int relative_y)
	{
		const double rotation_rads = deg2rad(ad_rotation_ang);
		x = (int)(relative_x * cos(rotation_rads) - relative_y * sin(rotation_rads));
		y = (int)(relative_x * sin(rotation_rads) + relative_y * cos(rotation_rads));
		return;
	}
	inline void rotation_relative(int& rel_x, int& rel_y)
	{
		const int x = rel_x, y = rel_y;
		const double rotation_rads = deg2rad(ad_rotation_ang);
		rel_x = (int)(x * cos(rotation_rads) - y * sin(rotation_rads));
		rel_y = (int)(x * sin(rotation_rads) + y * cos(rotation_rads));
		return;
	}
public:
	//初始化adaptor
	adaptor(int x, int y, double s, double ro) : base_x(x), base_y(y), size(s), ad_rotation_ang(ro) {}
	//经过修正后的绘图函数
	void ad_point(int rel_x, int rel_y, const int thickness = 3, const int RGB_value = INVALID_RGB)
	{
		int x, y;
		rotation_relative(x, y, ad(rel_x), ad(rel_y));
		hdc_point(base_x + x, base_y + y, ad(thickness), RGB_value);
	}
	void ad_line(const int rel_x1, const int rel_y1, const int rel_x2, const int rel_y2, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int x1, y1, x2, y2;
		rotation_relative(x1, y1, ad(rel_x1), ad(rel_y1));
		rotation_relative(x2, y2, ad(rel_x2), ad(rel_y2));
		hdc_line(base_x + x1, base_y + y1, base_x + x2, base_y + y2, ad(thickness), RGB_value);
	}
	void ad_triangle(const int rel_x1, const int rel_y1, const int rel_x2, const int rel_y2, const int rel_x3, const int rel_y3, bool filled = false, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int x1, y1, x2, y2, x3, y3;
		rotation_relative(x1, y1, ad(rel_x1), ad(rel_y1));
		rotation_relative(x2, y2, ad(rel_x2), ad(rel_y2));
		rotation_relative(x3, y3, ad(rel_x3), ad(rel_y3));
		hdc_triangle(base_x + x1, base_y + y1, base_x + x2, base_y + y2, base_x + x3, base_y + y3, filled, ad(thickness), RGB_value);
	}
	void ad_rectangle(const int rel_left_up_x, const int rel_left_up_y, const int width, const int high, const int rotation_angles = 0, const bool filled = false, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int left_up_x, left_up_y;
		rotation_relative(left_up_x, left_up_y, ad(rel_left_up_x), ad(rel_left_up_y));
		hdc_rectangle(base_x + left_up_x, base_y + left_up_y, ad(width), ad(high), rotation_angles + (int)ad_rotation_ang , filled, ad(thickness), RGB_value);
	}
	void ad_square(const int rel_left_up_x, const int rel_left_up_y, const int length, const int rotation_angles = 0, const bool filled = false, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int left_up_x, left_up_y;
		rotation_relative(left_up_x, left_up_y, ad(rel_left_up_x), ad(rel_left_up_y));
		hdc_rectangle(base_x + left_up_x, base_y + left_up_y, ad(length), rotation_angles + (int)ad_rotation_ang, filled, ad(thickness), RGB_value);
	}
	void ad_arc(const int rel_point_x, const int rel_point_y, const int radius, const int angle_begin = 0, const int angle_end = 360, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int point_x, point_y;
		rotation_relative(point_x, point_y, ad(rel_point_x), ad(rel_point_y));
		hdc_arc(base_x + point_x, base_y + point_y, ad(radius), angle_begin + (int)ad_rotation_ang, angle_end + (int)ad_rotation_ang, ad(thickness), RGB_value);
	}
	void ad_sector(const int rel_point_x, const int rel_point_y, const int radius, const int angle_begin = 0, const int angle_end = 360, const bool filled = false, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int point_x, point_y;
		rotation_relative(point_x, point_y, ad(rel_point_x), ad(rel_point_y));
		hdc_sector(base_x + point_x, base_y + point_y, ad(radius), angle_begin + (int)ad_rotation_ang, angle_end + (int)ad_rotation_ang, filled, ad(thickness), RGB_value);
	}
	void ad_circle(const int rel_point_x, const int rel_point_y, const int radius, const bool filled = false, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int point_x, point_y;
		rotation_relative(point_x, point_y, ad(rel_point_x), ad(rel_point_y));
		hdc_circle(base_x + point_x, base_y + point_y, ad(radius), filled, ad(thickness), RGB_value);
	}
	void ad_ellipse(const int rel_point_x, const int rel_point_y, const int radius_a, const int radius_b, const int rotation_angles = 0, const bool filled = false, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int point_x, point_y;
		rotation_relative(point_x, point_y, ad(rel_point_x), ad(rel_point_y));
		hdc_ellipse(base_x + point_x, base_y + point_y, ad(radius_a), ad(radius_b), rotation_angles + (int)ad_rotation_ang, filled, ad(thickness), RGB_value);
	}
	void ad_rcurve(const int rel_x1, const int rel_y1, const int rel_x2, const int rel_y2, const int deline, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int x1, y1, x2, y2;
		rotation_relative(x1, y1, ad(rel_x1), ad(rel_y1));
		rotation_relative(x2, y2, ad(rel_x2), ad(rel_y2));
		hdc_rcurve(base_x + x1, base_y + y1, base_x + x2, base_y + y2, ad(deline), ad(thickness), RGB_value);
	}
	void ad_pcurve(const int rel_x1, const int rel_y1, const int rel_xm, const int rel_ym, const int rel_x2, const int rel_y2, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		int x1, y1, x2, y2, xm, ym;
		rotation_relative(x1, y1, ad(rel_x1), ad(rel_y1));
		rotation_relative(x2, y2, ad(rel_x2), ad(rel_y2));
		rotation_relative(xm, ym, ad(rel_xm), ad(rel_ym));
		hdc_pcurve(base_x + x1, base_y + y1, base_x + xm, base_y + ym, base_x + x2, base_y + y2, ad(thickness), RGB_value);
	}
	void ad_fun(void fun(const int, const int, const double, const double), const int rel_x, const int rel_y, const double size, const double rotation_angles)
	{
		int x, y;
		rotation_relative(x, y, ad(rel_x), ad(rel_y));
		fun(base_x + x, base_y + y, ad(size), rotation_angles + ad_rotation_ang);
	}
	int ad_initCurve(CurveList& curve, Point* point)
	{
		Point* firstp = point;
		for (; point && point->x != END; point++) {
			rotation_relative(point->x, point->y);
			point->x = base_x + ad(point->x);
			point->y = base_y + ad(point->y);
		}
		if (point) point->x = point->y = 0;
		return curve.init(firstp);
	}
	int ad_drawCurveSheet(Point** curveSheet, const int thickness = 2, const int RGB_value = INVALID_RGB)
	{
		Point** pCSheet = curveSheet;
		for (; *pCSheet; pCSheet++) {
			CurveList curve;
			if (ad_initCurve(curve, *pCSheet))
				return pCSheet - curveSheet + 1;//返回第n个曲线初始化错误
			curve.draw(thickness, RGB_value);
			//curve.freeCurve();
		}
		return 0;
	}
	int ad_fillCurveSheet(Point** curveSheet, const int thickness = 2, const int RGB_value1 = INVALID_RGB, const int RGB_value2 = INVALID_RGB, int RGB_line = INVALID_RGB)
	{
		Point** pCSheet = curveSheet;
		for (; *pCSheet; pCSheet++) {
			CurveList curve;
			if (ad_initCurve(curve, *pCSheet))
				return pCSheet - curveSheet + 1;//返回第n个曲线初始化错误
			curve.fill(RGB_value1, RGB_value2);
			if (RGB_value2 == INVALID_RGB) {
				if (RGB_line == INVALID_RGB)
					RGB_line = RGB_value1;
				curve.draw(thickness, RGB_line);
			}
			//curve.freeCurve();
		}
		return 0;
	}
};

//测试曲线
static void testCurve(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//测试曲线
	Point curveTest[] = { {-49, -81},  {-45, -77},  {-40, -77},  {-39, -67},  {-35, -50},  {-39, -51},  {-43, -55},  {-48, -69}, {END, END} };// 8 points(closed curve)
	CurveList curve;
	S.ad_initCurve(curve, curveTest);
	curve.draw(2, HAIR_COLOR);
	curve.fill(RGB(168, 138, 126), HAIR_COLOR);
	//curve.freeCurve();
}

/****************************************************************以下开始绘画*******************************************************************************************/
//背景填充
static void background(const int width, const int height)
{
	const int BACKGROUND = RGB(202, 255, 255);
	hdc_rectangle(0, 0, width, height, 0, true, 2, BACKGROUND);
}

//画瞳孔
static void pupil(const int base_x, const int base_y, const double size, const double rotation_angles) 
{
	adaptor S(base_x, base_y, size, rotation_angles);
	const int rotation = -15;
	//瞳孔
	S.ad_ellipse(16, 0, 40, 56, rotation, true, 2, RGB(75, 30, 40));//深红
	S.ad_ellipse(20, 18, 34, 36, rotation, true, 2, RGB(215, 78, 96));//亮红
	//瞳心
	S.ad_ellipse(16, 0, 20, 28, rotation, true, 2, RGB(96, 29, 42));//褐
	S.ad_ellipse(16, 0, 3, 4, rotation, true, 2, RGB(185, 149, 157));//白
	//上光
	S.ad_ellipse(28, -34, 12, 18, rotation - 30, true, 2, RGB(143, 115, 236));//紫
	S.ad_ellipse(-9, -31, 8, 12, rotation - 60, true, 2, RGB(255, 255, 255));//白
	S.ad_ellipse(25, 34, 27, 20, rotation, true, 2, RGB(255, 195, 145));//橙
	S.ad_ellipse(21, 19, 15, 5, rotation, true, 2, RGB(254, 39, 46));//红
	S.ad_ellipse(23, 23, 9, 3, rotation, true, 2, RGB(254, 39, 46));//红
	//瞳底点缀
	S.ad_rectangle(42, 26, 4, 3, rotation - 30, true, 2, RGB(255, 255, 213));//白偏黄点缀
	S.ad_rectangle(32, 34, 8, 3, rotation - 20, true, 2, RGB(255, 255, 213));//白偏黄点缀
	S.ad_rectangle(23, 37, 6, 3, rotation, true, 2, RGB(255, 255, 213));//白偏黄点缀
	S.ad_rectangle(15, 37, 4, 3, rotation + 10, true, 2, RGB(255, 255, 213));//白偏黄点缀
	S.ad_rectangle(2, 32, 8, 3, rotation + 36, true, 2, RGB(255, 255, 213));//白偏黄点缀
}

//画左眼
static void left_eye(const int base_x, const int base_y ,const double size, const double rotation_angles) 
{
	adaptor S(base_x, base_y, size, rotation_angles);
	const int rotation = -15;
	//背景肤色
	//S.ad_ellipse(0, 0, 80, 100, rotation, true, 2, SKIN_COLOR);//肤色
	//眼廓
    S.ad_arc(-16, -14, 76, rotation - 45, rotation + 60, 2, RGB(11, 12, 6));//黑
	S.ad_triangle(-36, -70, -28, -80, -42, -97, true, 2, RGB(11, 12, 6));//黑
	S.ad_ellipse(-6, -16, 58, 70, rotation - 30, true, 2, RGB(11, 12, 6));//黑
	//上睫毛
	S.ad_triangle(-68, -4, -60, -38, -86, -42, true, 2, RGB(87, 44, 38));//褐
	S.ad_triangle(-60, -38, -46, -66, -77, -40, true, 2, RGB(59, 31, 26));//褐
	S.ad_ellipse(-6, -10, 60, 70, rotation - 30, true, 2, RGB(136, 24, 47));//褐
	S.ad_ellipse(-2, -4, 58, 70, rotation - 30, true, 2, RGB(17, 11, 6));//黑
	//下睫毛
	S.ad_line(-33, 57, 0, 0, 3, RGB(120, 55, 51));//褐
	S.ad_line(-27, 59, 5, 0, 3, RGB(120, 55, 51));//褐
	//瞳白
	S.ad_ellipse(0, 0, 68, 56, rotation + 55, true, 2, RGB(227, 208, 208));//灰白
	S.ad_ellipse(4, 11, 58, 48, rotation + 25, true, 2, RGB(255, 246, 243));//白
	//瞳孔
	S.ad_fun(&pupil, 0, 0, 1, 0);
	//下眼廓
	S.ad_arc(10, 3, 60, rotation + 147, rotation + 192, 4, RGB(135, 24, 47));//褐
}

//画右眼
static void right_eye(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	const int rotation = -5;
	//背景肤色
	//S.ad_ellipse(0, 0, 80, 100, rotation, true, 2, SKIN_COLOR);//肤色
	//眼廓
	S.ad_arc(16, -14, 76, rotation - 60, rotation + 45, 2, RGB(11, 12, 6));//黑
	S.ad_triangle(36, -70, 28, -76, 42, -97, true, 2, RGB(11, 12, 6));//黑
	S.ad_ellipse(6, -16, 56, 66, rotation + 30, true, 2, RGB(11, 12, 6));//黑
	//上睫毛
	S.ad_triangle(66, -4, 60, -38, 86, -42, true, 2, RGB(87, 44, 38));//褐
	S.ad_triangle(60, -38, 46, -66, 77, -40, true, 2, RGB(59, 31, 26));//褐
	S.ad_ellipse(6, -10, 56, 66, rotation + 30, true, 2, RGB(136, 24, 47));//褐
	S.ad_ellipse(5, -8, 54, 62, rotation + 30, true, 2, RGB(17, 11, 6));//黑
	//下睫毛
	S.ad_line(52, 41, 0, 0, 3, RGB(120, 55, 51));//褐
	S.ad_line(47, 44, -5, 0, 3, RGB(120, 55, 51));//褐
	//瞳白
	S.ad_ellipse(4, -4, 60, 54, rotation - 55, true, 2, RGB(227, 208, 208));//灰白
	S.ad_ellipse(6, 9, 54, 44, rotation - 25, true, 2, RGB(255, 246, 243));//亮白
	//瞳孔
	S.ad_fun(&pupil, -10, -4, 1, 15);
	//下眼廓
	S.ad_arc(5, 0, 56, rotation + 156, rotation + 202, 4, RGB(135, 24, 47));//褐
}

//画鼻子
static void nose(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//腮红
	S.ad_ellipse(4, -2, 18, 24, 0, true, 2, RGB(254, 234, 220));//腮红1
	S.ad_ellipse(4, -2, 12, 16, 0, true, 2, RGB(254, 222, 213));//腮红2
	S.ad_ellipse(4, -2, 6, 8, 0, true, 2, RGB(254, 217, 208));//腮红3
	//鼻尖
	S.ad_line(-2, 0, -2, 10, 4, RGB(191, 133, 117));//浅褐
	//鼻身
	S.ad_sector(0, -2, 8, 0, 180, true, 2, RGB(255, 255, 255));//纯白
}

//画嘴巴
static void mouse(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//上唇
    //下唇
	S.ad_arc(0, 10, 29, 165, 195, 4, RGB(253, 247, 246));//亮白
	S.ad_arc(0, 10, 35, 176, 184, 2, RGB(253, 200, 194));//深肤色
	//口
	S.ad_sector(0, 0, 30, 90, -90, true, 2, RGB(149, 71, 58));//褐
	S.ad_arc(0, 90, 93, -20, 20, 4, RGB(149, 71, 58));//褐
	S.ad_arc(30, 0, 60, -110, -90, 4, RGB(149, 71, 58));//褐
	S.ad_arc(-30, 0, 60, 90, 110, 4, RGB(149, 71, 58));//褐
	S.ad_arc(-3, 10, 26, -180, -90, 4, RGB(149, 71, 58));//褐
	S.ad_arc(3, 10, 26, 90, 180, 4, RGB(149, 71, 58));//褐
	//舌
	S.ad_sector(0, 10, 28, 90, -90, true, 2, RGB(253, 165, 157));//红舌
	S.ad_sector(-16, 10, 12, -90, 0, true, 2, RGB(253, 165, 157));//红舌
	S.ad_sector(16, 10, 12, 0, 90, true, 2, RGB(253, 165, 157));//红舌
	S.ad_arc(0, 91, 93, -14, 14, 4, RGB(253, 165, 157));//红舌
	S.ad_rectangle(-16, 0, 32, 10, 0, true, 2, RGB(253, 165, 157));//红舌
}

//描脸部阴影
static void faceShadow(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	const int faceShadow_color = RGB(254, 202, 212);
	adaptor S(base_x, base_y, size, rotation_angles);
	// Source: faceShadowCurve1.txt
	Point faceShadowCurve1[] = { {-54, 49},  {-67, 19},  {-70, 11},  {-84, -24},  {-72, -89},  {-34, -120},  {42, -100},  {69, -64},  {77, -27},  {78, -14},  {78, 6},  {77, -3},  {75, -12},  {46, -33},  {3, -66},  {10, -46},  {22, -19},  {8, -26},  {0, -32},  {16, -4},  {23, 3},  {-2, -12},  {-20, -29},  {-14, -8},  {-8, 0},  {-35, -26},  {-53, -56},  {-58, -30},  {-64, 17},  {-58, 32}, {END, END} };// 30 points(closed curve)
	// Source: faceShadowCurve2.txt
	Point faceShadowCurve2[] = { {79, 22},  {77, 33},  {76, 43},  {68, 59},  {53, 79},  {68, 56}, {END, END} };// 6 points(closed curve)

	// faceShadowCurveSheet
	Point* faceShadowCurveSheet[] = { faceShadowCurve1, faceShadowCurve2, NULL };//all 2 faceShadow curves

	S.ad_fillCurveSheet(faceShadowCurveSheet, 2, faceShadow_color);


}

//画面部
static void face(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//肤色填充
	//S.ad_rectangle(-120, -120, 300, 300, -5, true, 2, SKIN_COLOR);//浅肤色
	S.ad_rectangle(105, -90, 218, 100, 170, true, 2, SKIN_COLOR);//浅肤色
	S.ad_triangle(-72, 44, -110, -50, 105, -90, true, 2, SKIN_COLOR);//浅肤色
	S.ad_triangle(-72, 44, 110, -10, 105, -90, true, 2, SKIN_COLOR);//浅肤色
	S.ad_triangle(-36, 72, 34, 92, 105, -90, true, 2, SKIN_COLOR);//浅肤色
	S.ad_triangle(65, 73, 34, 92, -110, -50, true, 2, SKIN_COLOR);//浅肤色
	S.ad_triangle(-36, 72, 105, -90, -110, -50, true, 2, SKIN_COLOR);//浅肤色
	S.ad_triangle(65, 73, 105, -90, -110, -50, true, 2, SKIN_COLOR);//浅肤色
	S.ad_line(-72, 44, 0, 44, 6, SKIN_COLOR);//补填充
	S.ad_sector(-20, 15, 60, -166, -120, true, 1, SKIN_COLOR);//浅肤色
	S.ad_sector(1, -15, 110, 90, 144, true, 1, SKIN_COLOR);//浅肤色
	//脸部阴影
	if (size <= 1.6)
		S.ad_fun(faceShadow, -2, -44, 1.4, 0);
	//左眼腮红
	S.ad_ellipse(-38, 2, 24, 12, -18, true, 2, RGB(254, 234, 220));//腮红1
	S.ad_ellipse(-38, 2, 16, 8, -18, true, 2, RGB(254, 222, 213));//腮红2
	S.ad_ellipse(-38, 2, 8, 4, -18, true, 2, RGB(254, 217, 208));//腮红3
	//右眼腮红
	S.ad_ellipse(64, -24, 24, 12, -10, true, 2, RGB(254, 234, 220));//腮红1
	S.ad_ellipse(64, -24, 16, 8, -10, true, 2, RGB(254, 222, 213));//腮红2
	S.ad_ellipse(64, -24, 8, 4, -10, true, 2, RGB(254, 217, 208));//腮红3
	//脸边线
	S.ad_line(-72, 44, -110, -50, 2, RGB(80, 36, 23));//轮廓褐色
	S.ad_line(-36, 72, 34, 92, 2, RGB(80, 36, 23));//轮廓褐色
	S.ad_arc(-20, 15, 60, -166, -120, 1, RGB(80, 36, 23));//轮廓褐色
	S.ad_line(65, 73, 34, 92, 2, RGB(80, 36, 23));//轮廓褐色
	S.ad_line(110, -10, 105, -90, 2, RGB(80, 36, 23));//轮廓褐色
	S.ad_arc(1, -15, 110, 90, 144, 1, RGB(80, 36, 23));//轮廓褐色
	//五官
	S.ad_fun(left_eye, -54, -30, 0.5, 0);
	S.ad_fun(right_eye, 54, -56, 1.07 / 2, -5);
	S.ad_fun(nose, 0, 0, 0.6, 0);
	S.ad_fun(mouse, 20, 36, 1, -15);
}

//画头发轮廓
static void hairOutline(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//头发轮廓
	S.ad_pcurve(-54, -76, -39, -39, -5, -2, 2, HAIR_COLOR);//头顶三措毛
	S.ad_pcurve(-5, -2, -16, -19, -24, -36, 2, HAIR_COLOR);
	S.ad_pcurve(-24, -36, -6, -19, 24, 0, 2, HAIR_COLOR);
	S.ad_pcurve(24, 0, 7, -23, -5, -47, 2, HAIR_COLOR);
	S.ad_pcurve(-5, -47, 7, -32, 25, -17, 2, HAIR_COLOR);
	S.ad_pcurve(25, -17, 9, -46, -4, -79, 2, HAIR_COLOR);

	S.ad_pcurve(-4, -79, 36, -48, 69, -27, 2, HAIR_COLOR);//右边最飘逸的一撮毛
	S.ad_pcurve(69, -27, 90, -13, 107, 3, 2, HAIR_COLOR);
	S.ad_pcurve(107, 3, 123, 31, 119, 70, 2, HAIR_COLOR);
	S.ad_pcurve(119, 70, 132, 56, 137, 39, 2, HAIR_COLOR);
	S.ad_pcurve(137, 39, 132, -1, 119, -28, 2, HAIR_COLOR);
	S.ad_pcurve(119, -28, 104, -48, 79, -74, 2, HAIR_COLOR);

	S.ad_pcurve(132, 56, 145, 44, 150, 32, 2, HAIR_COLOR);//curve2
	S.ad_pcurve(158, 40, 150, 32, 138, 19, 2, HAIR_COLOR);
	S.ad_pcurve(158, 40, 169, 61, 159, 86, 2, HAIR_COLOR);
	S.ad_pcurve(159, 86, 179, 67, 174, 38, 2, HAIR_COLOR);
	S.ad_pcurve(174, 38, 184, 55, 187, 71, 2, HAIR_COLOR);
	S.ad_pcurve(184, 75, 190, 63, 189, 52, 2, HAIR_COLOR);
	S.ad_pcurve(189, 52, 178, 32, 161, 13, 2, HAIR_COLOR);
	S.ad_pcurve(161, 13, 149, 0, 142, -11, 2, HAIR_COLOR);
	S.ad_pcurve(142, -11, 123, -51, 105, -92, 2, HAIR_COLOR);
	S.ad_pcurve(105, -92, 92, -111, 76, -128, 2, HAIR_COLOR);
	S.ad_pcurve(76, -128, 51, -146, 22, -157, 2, HAIR_COLOR);
	S.ad_pcurve(22, -157, 0, -151, -14, -143, 2, HAIR_COLOR);

	S.ad_pcurve(-26, -143, -31, -136, -29, -125, 2, HAIR_COLOR);
	S.ad_pcurve(-29, -125, -21, -109, -2, -94, 2, HAIR_COLOR);
	S.ad_pcurve(-2, -94, 45, -77, 113, -82, 2, HAIR_COLOR);
	S.ad_pcurve(113, -82, 75, -88, 43, -102, 2, HAIR_COLOR);

	S.ad_pcurve(134, -25, 148, -12, 164, -2, 2, HAIR_COLOR);
	S.ad_pcurve(164, -2, 183, 3, 201, 4, 2, HAIR_COLOR);
	S.ad_pcurve(212, 0, 204, 7, 188, 11, 2, HAIR_COLOR);
	S.ad_pcurve(188, 11, 168, 7, 147, -3, 2, HAIR_COLOR);

	S.ad_pcurve(81, -130, 100, -106, 124, -87, 2, HAIR_COLOR);//curve3
	S.ad_pcurve(124, -87, 144, -80, 167, -79, 2, HAIR_COLOR);
	S.ad_pcurve(167, -79, 191, -80, 212, -76, 2, HAIR_COLOR);
	S.ad_pcurve(212, -76, 220, -70, 220, -63, 2, HAIR_COLOR);
	S.ad_pcurve(220, -63, 225, -72, 218, -82, 2, HAIR_COLOR);
	S.ad_pcurve(218, -82, 236, -76, 247, -68, 2, HAIR_COLOR);
	S.ad_pcurve(247, -68, 249, -58, 239, -53, 2, HAIR_COLOR);
	S.ad_pcurve(239, -53, 256, -58, 259, -77, 2, HAIR_COLOR);
	S.ad_pcurve(259, -77, 257, -87, 245, -101, 2, HAIR_COLOR);
	S.ad_pcurve(257, -87, 255, -96, 245, -101, 2, HAIR_COLOR);
	S.ad_pcurve(259, -82, 260, -102, 244, -113, 2, HAIR_COLOR);
	S.ad_pcurve(244, -113, 235, -116, 214, -115, 2, HAIR_COLOR);
	S.ad_pcurve(214, -115, 182, -112, 161, -117, 2, HAIR_COLOR);
	S.ad_pcurve(245, -101, 230, -108, 202, -111, 2, HAIR_COLOR);
	S.ad_pcurve(244, -113, 236, -130, 215, -133, 2, HAIR_COLOR);
	S.ad_pcurve(215, -133, 191, -128, 171, -125, 2, HAIR_COLOR);
	S.ad_pcurve(171, -125, 153, -127, 137, -132, 2, HAIR_COLOR);
	S.ad_pcurve(137, -132, 119, -143, 106, -147, 2, HAIR_COLOR);
	S.ad_pcurve(106, -147, 95, -147, 86, -145, 2, HAIR_COLOR);

	S.ad_pcurve(-54, -78, -58, -41, -73, 15, 2, HAIR_COLOR);//curve4左边第一措毛
	S.ad_pcurve(-73, 15, -82, 40, -88, 63, 2, HAIR_COLOR);
	S.ad_pcurve(-88, 63, -88, 82, -82, 99, 2, HAIR_COLOR);
	S.ad_pcurve(-82, 99, -97, 81, -101, 58, 2, HAIR_COLOR);
	S.ad_pcurve(-101, 58, -117, 73, -154, 80, 2, HAIR_COLOR);
	S.ad_pcurve(-117, 73, -122, 91, -123, 112, 2, HAIR_COLOR);
	S.ad_pcurve(-123, 112, -132, 96, -132, 82, 2, HAIR_COLOR);
	S.ad_pcurve(-154, 80, -135, 72, -124, 55, 2, HAIR_COLOR);
	S.ad_pcurve(-124, 55, -117, 33, -108, -73, 2, HAIR_COLOR);
	S.ad_pcurve(-108, -73, -88, -120, -34, -140, 2, HAIR_COLOR);

	S.ad_pcurve(-54, -78, -56, -98, -50, -123, 2, HAIR_COLOR);//curve5头顶
	S.ad_pcurve(-14, -139, 0, -126, 56, -104, 2, HAIR_COLOR);
	S.ad_pcurve(-92, -116, -119, -132, -143, -181, 2, HAIR_COLOR);
	S.ad_pcurve(-143, -181, -145, -199, -145, -218, 2, HAIR_COLOR);
	S.ad_pcurve(-103, -182, -83, -162, -69, -136, 2, HAIR_COLOR);
	S.ad_pcurve(-69, -136, -121, -199, -145, -218, 2, HAIR_COLOR);
	S.ad_pcurve(-104, -121, -110, -103, -108, -73, 2, HAIR_COLOR);
	S.ad_pcurve(-78, -154, -59, -167, -38, -175, 2, HAIR_COLOR);
	S.ad_pcurve(-18, -180, 2, -183, 22, -181, 2, HAIR_COLOR);
	S.ad_pcurve(69, -164, 84, -155, 96, -147, 2, HAIR_COLOR);
	S.ad_pcurve(-20, -143, -32, -163, -40, -182, 2, HAIR_COLOR);
	S.ad_pcurve(-40, -182, -41, -200, -20, -220, 2, HAIR_COLOR);
	S.ad_pcurve(-20, -143, -21, -164, -18, -182, 2, HAIR_COLOR);
	S.ad_pcurve(-18, -182, -15, -200, -20, -220, 2, HAIR_COLOR);
	S.ad_pcurve(24, -158, 23, -170, 22, -181, 2, HAIR_COLOR);
	S.ad_pcurve(22, -181, 32, -200, 51, -226, 2, HAIR_COLOR);
	S.ad_pcurve(51, -146, 69, -165, 73, -182, 2, HAIR_COLOR);
	S.ad_pcurve(73, -182, 72, -200, 51, -226, 2, HAIR_COLOR);

	S.ad_pcurve(-109, -73, -127, -33, -166, -15, 2, HAIR_COLOR);//curve6左毛
	S.ad_pcurve(-166, -15, -191, -5, -178, 19, 2, HAIR_COLOR);
	S.ad_pcurve(-178, 19, -200, 13, -211, -1, 2, HAIR_COLOR);
	S.ad_pcurve(-211, -1, -210, -19, -205, -32, 2, HAIR_COLOR);
	S.ad_pcurve(-205, -32, -191, -45, -173, -53, 2, HAIR_COLOR);
	S.ad_pcurve(-173, -53, -154, -70, -146, -84, 2, HAIR_COLOR);
	S.ad_pcurve(-146, -84, -129, -100, -111, -106, 2, HAIR_COLOR);

}

//画头发基层填充
static void hairBase(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	// Source: hairBaseCurve1.txt
	Point hairBaseCurve1[] = { {-50, -123},  {-55, -98},  {-54, -78},  {-40, -41},  {-5, -2},  {-17, -22},  {-25, -39},  {-4, -17},  {24, -1},  {5, -26},  {-6, -50},  {7, -31},  {26, -18},  {4, -55},  {-13, -101},  {-27, -120},  {-26, -143},  {-39, -135}, {END, END} };// 18 points(closed curve)
	// Source: hairBaseCurve2.txt
	Point hairBaseCurve2[] = { {-26, -143},  {-27, -120},  {-13, -101},  {13, -87},  {45, -77},  {74, -75},  {113, -82},  {75, -89},  {43, -102},  {12, -110},  {-16, -125},  {-26, -135}, {END, END} };// 12 points(closed curve)
	// Source: hairBaseCurve3.txt
	Point hairBaseCurve3[] = { {-13, -100},  {-4, -80},  {17, -61},  {56, -35},  {88, -15},  {110, 7},  {123, 30},  {126, 50},  {119, 70},  {135, 47},  {138, 19},  {158, 39},  {169, 61},  {169, 73},  {159, 85},  {179, 66},  {174, 38},  {184, 54},  {187, 71},  {189, 52},  {178, 32},  {161, 13},  {147, -3},  {179, 10},  {211, -1},  {201, 4},  {183, 2},  {158, -6},  {135, -24},  {115, -69},  {92, -111},  {76, -128},  {37, -153},  {24, -157},  {-3, -153},  {-15, -149},  {-25, -143},  {-21, -109}, {END, END} };// 38 points(closed curve)
	// Source: hairBaseCurve4.txt
	Point hairBaseCurve4[] = { {82, -130},  {100, -106},  {124, -88},  {144, -80},  {167, -79},  {191, -80},  {212, -76},  {220, -71},  {219, -63},  {225, -72},  {218, -82},  {236, -76},  {247, -68},  {249, -58},  {239, -53},  {256, -59},  {259, -77},  {257, -87},  {245, -101},  {255, -97},  {259, -82},  {260, -102},  {244, -113},  {236, -116},  {214, -115},  {182, -113},  {161, -117},  {137, -132},  {118, -143},  {106, -147},  {95, -147},  {86, -145}, {END, END} };// 32 points(closed curve)
	// Source: hairBaseCurve5.txt
	Point hairBaseCurve5[] = { {-30, -140},  {-50, -123},  {-56, -98},  {-54, -78},  {-60, -35},  {-73, 15},  {-84, 47},  {-89, 66},  {-82, 99},  {-97, 78},  {-101, 57},  {-108, 66},  {-117, 73},  {-122, 90},  {-123, 112},  {-132, 96},  {-132, 81},  {-141, 82},  {-154, 80},  {-135, 71},  {-125, 55},  {-117, 33},  {-112, -10},  {-111, -38},  {-110, -62},  {-108, -73},  {-88, -120},  {-70, -136}, {END, END} };// 28 points(closed curve)
	// Source: hairBaseCurve6.txt
	Point hairBaseCurve6[] = { {-102, -81},  {-119, -43},  {-137, -24},  {-146, -20},  {-171, -15},  {-191, -5},  {-178, 19},  {-200, 12},  {-211, -11},  {-216, -4},  {-213, 5},  {-220, -8},  {-208, -26},  {-200, -37},  {-191, -45},  {-202, -41},  {-208, -30},  {-208, -48},  {-192, -56},  {-178, -59},  {-157, -66},  {-140, -90},  {-116, -108},  {-98, -100}, {END, END} };// 24 points(closed curve)
	// Source: hairBaseCurve7.txt
	Point hairBaseCurve7[] = { {-27, -141},  {-50, -141},  {-69, -136},  {-88, -120},  {-105, -92},  {-108, -107},  {-103, -121},  {-77, -154},  {-38, -176},  {2, -183},  {22, -180},  {69, -164},  {95, -147},  {85, -144},  {81, -127},  {51, -146},  {37, -153},  {24, -157},  {-3, -154},  {-18, -146}, {END, END} };// 20 points(closed curve)
	
	// HairBaseCurveSheet
	Point* HairBaseCurveSheet[] = { hairBaseCurve1, hairBaseCurve2, hairBaseCurve3, hairBaseCurve4, hairBaseCurve5, hairBaseCurve6, hairBaseCurve7, NULL };//all 10 hair curves

	S.ad_fillCurveSheet(HairBaseCurveSheet, 2, HAIR_COLOR);
}

//画头发背景填充（脸后方）
static void hairBH(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	// Source: hairBackCurve1.txt
	Point hairBackCurve1[] = { {77, -22},  {92, -12},  {102, -2},  {102, 15},  {102, 40},  {98, 62},  {87, 87},  {76, 101},  {58, 112},  {71, 92},  {75, 76},  {70, 83},  {60, 90},  {73, 66},  {77, 40},  {79, 10}, {END, END} };// 16 points(closed curve)
	// Source: hairBackCurve2.txt
	Point hairBackCurve2[] = { {-84, 47},  {-82, 53},  {-77, 58},  {-70, 82},  {-54, 105},  {-47, 114},  {-30, 125},  {-49, 87},  {-56, 53},  {-46, 74},  {-23, 89},  {-35, 75},  {-42, 63},  {-49, 35},  {-71, 9},  {-81, 26}, {END, END} };// 16 points(closed curve)
	// Source: hairBackCurve3.txt
	Point hairBackCurve3[] = { {72, -130},  {82, -121},  {92, -109},  {106, -77},  {116, -56},  {126, -32},  {135, 8},  {138, 29},  {132, 54},  {150, 32},  {155, -3},  {160, 24},  {143, 52},  {161, 42},  {169, 21},  {169, -1},  {160, -25},  {178, -4},  {169, 30},  {185, 22},  {192, 9},  {199, -14},  {192, -40},  {210, -29},  {227, -12},  {227, -4},  {220, 3},  {234, -8},  {233, -28},  {207, -57},  {158, -78},  {124, -91},  {95, -119},  {86, -129}, {END, END} };// 34 points(closed curve)
	// Source: hairBackCurve4.txt
	Point hairBackCurve4[] = { {-126, -34},  {-136, -17},  {-152, -2},  {-166, 0},  {-179, -6},  {-169, 5},  {-151, 5},  {-160, 24},  {-164, 42},  {-159, 57},  {-146, 64},  {-155, 53},  {-144, 28},  {-140, 50},  {-117, 66},  {-131, 42},  {-122, 18},  {-109, 0},  {-96, -22},  {-92, -51},  {-110, -63},  {-120, -52}, {END, END} };// 22 points(closed curve)

	// HairBackCurveSheet
	Point* HairBackCurveSheet[] = { hairBackCurve1, hairBackCurve2, hairBackCurve3, hairBackCurve4, NULL };//all 4 hair curves
	
	S.ad_fillCurveSheet(HairBackCurveSheet, 3, RGB(66, 54, 73));//黑偏蓝
}

//画耳朵和呆毛
static void hairEar(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	const int EARBASE_COLOR = RGB(93, 92, 108);
	const int EARSHADOW_COLOR = RGB(54, 50, 68);
	const int EARSHADOW_DCOLOR = RGB(30, 28, 38);
	adaptor S(base_x, base_y, size, rotation_angles);
	//耳朵形状
	// Source: hairBaseCurve8.txt
	Point hairBaseCurve8[] = { {-91, -116},  {-104, -121},  {-119, -132},  {-135, -156},  {-145, -181},  {-149, -200},  {-143, -218},  {-117, -196},  {-109, -189},  {-78, -154},  {-69, -136},  {-82, -127}, {END, END} };// 12 points(closed curve)
	// Source: hairBaseCurve9.txt
	Point hairBaseCurve9[] = { {24, -153},  {23, -170},  {23, -182},  {32, -200},  {56, -227},  {71, -200},  {74, -187},  {69, -165},  {54, -144},  {37, -153}, {END, END} };// 10 points(closed curve)
	// Source: hairBaseCurve10.txt
	Point hairBaseCurve10[] = { {-20, -142},  {-32, -163},  {-40, -181},  {-41, -200},  {0, -230},  {-16, -200},  {-18, -182},  {-21, -164}, {END, END} };// 8 points(closed curve)

	// HairBaseCurveSheet
	Point* HairBaseCurveSheet[] = { hairBaseCurve8, hairBaseCurve9, hairBaseCurve10, NULL };//all 10 hair curves

	S.ad_fillCurveSheet(HairBaseCurveSheet, 2, RGB(93, 92, 108));

	//耳朵阴影
	// Source: earShadowCurve1.txt
	Point earShadowCurve1[] = { {-130, -198},  {-112, -181},  {-95, -165},  {-88, -148},  {-80, -129},  {-86, -122},  {-91, -117},  {-104, -121},  {-125, -140},  {-109, -130},  {-102, -128},  {-109, -134},  {-122, -147},  {-112, -141},  {-104, -137},  {-105, -151},  {-104, -166},  {-106, -170},  {-112, -171},  {-129, -182}, {END, END} };// 20 points(closed curve)
	// Source: earShadowCurve2.txt
	Point earShadowCurve2[] = { {23, -182},  {23, -166},  {26, -156},  {37, -153},  {54, -144},  {63, -154},  {70, -171},  {64, -164},  {58, -162},  {66, -176},  {64, -199},  {61, -182},  {56, -172},  {55, -183},  {43, -204},  {32, -185},  {35, -163},  {29, -170}, {END, END} };// 18 points(closed curve)
	// Source: earShadowCurve3.txt
	Point earShadowCurve3[] = { {-21, -142},  {-40, -182},  {0, -230},  {-25, -182}, {END, END} };// 4 points(closed curve)
	// Source: earShadowCurve4.txt
	Point earShadowCurve4[] = { {-21, -164},  {-15, -168},  {-1, -171},  {-15, -164},  {-21, -157},  {-21, -161}, {END, END} };// 6 points(closed curve)
	// Source: earShadowCurve5.txt
	Point earShadowCurve5[] = { {-89, -152},  {-85, -139},  {-80, -129},  {-84, -125},  {-87, -121},  {-92, -137}, {END, END} };// 6 points(closed curve)
	// Source: earShadowCurve6.txt
	Point earShadowCurve6[] = { {32, -182},  {34, -168},  {37, -153},  {41, -151},  {47, -148},  {47, -164}, {END, END} };// 6 points(closed curve)
	// Source: earShadowCurve7.txt
	Point earShadowCurve7[] = { {-20, -142},  {-23, -149},  {-29, -155},  {-42, -156},  {-56, -155},  {-45, -155}, {END, END} };// 6 points(closed curve)
	// Source: earShadowCurve8.txt
	Point earShadowCurve8[] = { {-14, -143},  {-5, -153},  {24, -157},  {0, -151}, {END, END} };// 4 points(closed curve)
	// Source: earShadowCurve9.txt
	Point earShadowCurve9[] = { {-72, -142},  {-67, -148},  {-56, -155},  {-63, -153},  {-67, -152},  {-59, -161},  {-37, -171},  {-56, -167},  {-76, -151},  {-74, -147}, {END, END} };// 10 points(closed curve)
	// Source: earShadowCurve10.txt
	Point earShadowCurve10[] = { {-32, -135},  {-48, -133},  {-64, -121},  {-56, -135},  {-27, -143},  {-22, -142},  {-17, -140},  {-24, -132},  {-25, -94},  {-32, -106},  {-35, -110},  {-35, -124}, {END, END} };// 12 points(closed curve)
	// Source: earShadowCurve11.txt
	Point earShadowCurve11[] = { {-14, -139},  {-12, -132},  {2, -126},  {-8, -134},  {-12, -139},  {14, -143},  {56, -134},  {30, -147},  {9, -150},  {19, -153},  {32, -152},  {10, -155}, {END, END} };// 12 points(closed curve)
	// Source: earShadowCurve12.txt
	Point earShadowCurve12[] = { {21, -131},  {78, -94},  {116, -41},  {67, -90}, {END, END} };// 4 points(closed curve)


	// earShadowCurveSheet
	Point* earShadowCurveSheet1[] = { earShadowCurve1, earShadowCurve2, earShadowCurve3, earShadowCurve4, earShadowCurve9, earShadowCurve10, earShadowCurve11, earShadowCurve12, NULL };// 4 hair curves
	Point* earShadowCurveSheet2[] = { earShadowCurve5, earShadowCurve6, earShadowCurve7, earShadowCurve8, NULL };// 4 hair curves

	S.ad_fillCurveSheet(earShadowCurveSheet1, 2, EARSHADOW_COLOR);
	S.ad_fillCurveSheet(earShadowCurveSheet2, 2, EARSHADOW_DCOLOR);
}

//发丝边缘上光
static void hairLight(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	const int HAIRLIGHT_COLOR = RGB(254, 254, 254);//cur1-cur24
	const int HAIRLIGHT_SCOLOR = RGB(195, 214, 247);//cur25+
	adaptor S(base_x, base_y, size, rotation_angles);
	//边缘上光
	// Source: hairLightCurve1.txt
	Point hairLightCurve1[] = { {-78, -154},  {-59, -167},  {-38, -175},  {-38, -174},  {-37, -172},  {-41, -172},  {-46, -171},  {-67, -161}, {END, END} };// 8 points(closed curve)
	// Source: hairLightCurve2.txt
	Point hairLightCurve2[] = { {-18, -182},  {2, -183},  {22, -180},  {22, -178},  {22, -176},  {9, -180}, {END, END} };// 6 points(closed curve)
	// Source: hairLightCurve3.txt
	Point hairLightCurve3[] = { {-71, -136},  {-73, -143},  {-80, -154},  {-74, -142},  {-72, -135},  {-71, -136}, {END, END} };// 6 points(closed curve)
	// Source: hairLightCurve4.txt
	Point hairLightCurve4[] = { {-85, -164},  {-94, -175},  {-102, -183},  {-121, -200},  {-143, -218},  {-124, -198},  {-101, -177},  {-94, -172}, {END, END} };// 8 points(closed curve)
	// Source: hairLightCurve5.txt
	Point hairLightCurve5[] = { {-85, -164},  {-94, -175},  {-102, -183},  {-121, -200},  {-143, -218},  {-124, -198},  {-101, -177},  {-94, -172}, {END, END} };// 8 points(closed curve)
	// Source: hairLightCurve6.txt
	Point hairLightCurve6[] = { {-21, -150},  {-19, -182},  {-15, -200},  {-21, -182}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve7.txt
	Point hairLightCurve7[] = { {25, -160},  {23, -178},  {26, -191},  {25, -182}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve8.txt
	Point hairLightCurve8[] = { {69, -168},  {74, -183},  {56, -227},  {68, -199},  {70, -190},  {70, -182}, {END, END} };// 6 points(closed curve)
	// Source: hairLightCurve9.txt
	Point hairLightCurve9[] = { {68, -163},  {83, -155},  {95, -147},  {86, -150},  {68, -161},  {68, -162}, {END, END} };// 6 points(closed curve)
	// Source: hairLightCurve10.txt
	Point hairLightCurve10[] = { {86, -144},  {106, -147},  {137, -132},  {161, -117},  {188, -113},  {214, -115},  {240, -115},  {253, -109},  {260, -93},  {253, -108},  {238, -113},  {226, -114},  {204, -111},  {230, -108},  {245, -100},  {229, -106},  {194, -108},  {161, -113},  {132, -132},  {117, -140}, {END, END} };// 20 points(closed curve)
	// Source: hairLightCurve11.txt
	Point hairLightCurve11[] = { {161, -76},  {192, -65},  {225, -41},  {234, -21},  {232, -4},  {233, -21},  {226, -37},  {209, -52},  {194, -61},  {178, -69}, {END, END} };// 10 points(closed curve)
	// Source: hairLightCurve12.txt
	Point hairLightCurve12[] = { {176, -56},  {198, -28},  {195, 2},  {195, -27}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve13.txt
	Point hairLightCurve13[] = { {18, -156},  {38, -152},  {54, -144},  {76, -128},  {92, -112},  {105, -92},  {122, -50},  {106, -84},  {93, -106},  {72, -128},  {53, -142},  {35, -151}, {END, END} };// 12 points(closed curve)
	// Source: hairLightCurve14.txt
	Point hairLightCurve14[] = { {165, -2},  {190, 4},  {212, -3},  {206, 2},  {193, 7},  {179, 3}, {END, END} };// 6 points(closed curve)
	// Source: hairLightCurve15.txt
	Point hairLightCurve15[] = { {161, 14},  {180, 32},  {191, 52},  {190, 59},  {188, 69},  {188, 54},  {185, 46},  {178, 34}, {END, END} };// 8 points(closed curve)
	// Source: hairLightCurve16.txt
	Point hairLightCurve16[] = { {161, 85},  {175, 76},  {177, 53},  {174, 69},  {166, 79},  {163, 83}, {END, END} };// 6 points(closed curve)
	// Source: hairLightCurve17.txt
	Point hairLightCurve17[] = { {126, 63},  {137, 38},  {132, 0},  {135, 35}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve18.txt
	Point hairLightCurve18[] = { {62, 110},  {76, 101},  {90, 81},  {85, 88},  {80, 95},  {83, 87},  {85, 77},  {76, 97}, {END, END} };// 8 points(closed curve)
	// Source: hairLightCurve19.txt
	Point hairLightCurve19[] = { {-145, -84},  {-157, -66},  {-172, -53},  {-153, -67}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve20.txt
	Point hairLightCurve20[] = { {-158, -66},  {-168, -63},  {-176, -60},  {-192, -57},  {-208, -48},  {-192, -55},  {-178, -58},  {-166, -60}, {END, END} };// 8 points(closed curve)
	// Source: hairLightCurve21.txt
	Point hairLightCurve21[] = { {-154, 80},  {-141, 82},  {-117, 72},  {-138, 80}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve22.txt
	Point hairLightCurve22[] = { {-124, 109},  {-122, 91},  {-119, 78},  {-125, 92}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve23.txt
	Point hairLightCurve23[] = { {-86, 93},  {-88, 82},  {-85, 48},  {-91, 77}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve24.txt
	Point hairLightCurve24[] = { {-34, 122},  {-47, 96},  {-54, 67},  {-49, 97}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve25.txt
	Point hairLightCurve25[] = { {-190, -6},  {-193, 3},  {-184, 15},  {-189, 8},  {-190, 4},  {-186, 6},  {-179, 4},  {-187, 3}, {END, END} };// 8 points(closed curve)
	// Source: hairLightCurve26.txt
	Point hairLightCurve26[] = { {-176, -3},  {-167, 1},  {-160, 1},  {-152, -2},  {-137, -15},  {-145, -5},  {-159, 2},  {-166, 3}, {END, END} };// 8 points(closed curve)
	// Source: hairLightCurve27.txt
	Point hairLightCurve27[] = { {-154, 57},  {-151, 61},  {-146, 64},  {-151, 63}, {END, END} };// 4 points(closed curve)
	// Source: hairLightCurve28.txt
	Point hairLightCurve28[] = { {-131, 43},  {-129, 51},  {-126, 56},  {-131, 51}, {END, END} };// 4 points(closed curve)

	// hairLightCurveSheet
	Point* hairLightCurveSheet1[] = { hairLightCurve1, hairLightCurve2, hairLightCurve3, hairLightCurve4, hairLightCurve5, hairLightCurve6, hairLightCurve7, hairLightCurve8, hairLightCurve9, hairLightCurve10, hairLightCurve11, hairLightCurve12, hairLightCurve13, hairLightCurve14, hairLightCurve15, hairLightCurve16, hairLightCurve17, hairLightCurve18, hairLightCurve19, hairLightCurve20, hairLightCurve21, hairLightCurve22, hairLightCurve23, hairLightCurve24, NULL };//all 24 hairLight curves
	S.ad_fillCurveSheet(hairLightCurveSheet1, 2, HAIRLIGHT_COLOR, HAIRLIGHT_COLOR);
	// hairLightCurveSheet
	Point* hairLightCurveSheet2[] = { hairLightCurve25, hairLightCurve26, hairLightCurve27, hairLightCurve28, NULL };//all 4 hairLight curves
	S.ad_fillCurveSheet(hairLightCurveSheet2, 2, HAIRLIGHT_COLOR, HAIRLIGHT_SCOLOR);
}

//头发上光
static void hairPolish(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	const int HAIRPOLISH_COLOR1 = RGB(168, 138, 126);
	const int HAIRPOLISH_COLOR2 = HAIR_COLOR;
	adaptor S(base_x, base_y, size, rotation_angles);
	//抛光
	// Source: hairPolishCurve1.txt
	Point hairPolishCurve1[] = { {-49, -81},  {-45, -77},  {-40, -77},  {-39, -67},  {-35, -50},  {-39, -51},  {-43, -55},  {-48, -69}, {END, END} };// 8 points(closed curve)
	// Source: hairPolishCurve2.txt
	Point hairPolishCurve2[] = { {-36, -81},  {-35, -75},  {-32, -71},  {-32, -72},  {-32, -75},  {-20, -76},  {-10, -80},  {-6, -69},  {1, -48},  {-10, -64},  {-16, -75},  {-14, -64},  {-11, -50},  {-20, -51},  {-27, -50},  {-35, -64}, {END, END} };// 16 points(closed curve)
	// Source: hairPolishCurve3.txt
	Point hairPolishCurve3[] = { {4, -81},  {15, -75},  {31, -60},  {33, -61},  {29, -65},  {22, -71},  {14, -79},  {18, -78},  {20, -79},  {39, -71},  {55, -67},  {44, -61},  {35, -56},  {14, -70}, {END, END} };// 14 points(closed curve)
	// Source: hairPolishCurve4.txt
	Point hairPolishCurve4[] = { {51, -111},  {61, -114},  {62, -126},  {74, -118},  {84, -108},  {80, -102},  {81, -91},  {65, -103}, {END, END} };// 8 points(closed curve)
	// Source: hairPolishCurve5.txt
	Point hairPolishCurve5[] = { {65, -127},  {70, -128},  {72, -128},  {79, -120},  {93, -104},  {76, -119}, {END, END} };// 6 points(closed curve)
	// Source: hairPolishCurve6.txt
	Point hairPolishCurve6[] = { {99, -13},  {105, -8},  {114, 7},  {104, -3}, {END, END} };// 4 points(closed curve)
	// Source: hairPolishCurve7.txt
	Point hairPolishCurve7[] = { {107, -16},  {113, -23},  {109, -36},  {119, -24},  {129, -7},  {124, -14},  {119, -19},  {122, -15},  {124, -8},  {119, -3},  {118, 1},  {113, -8}, {END, END} };// 12 points(closed curve)
	// Source: hairPolishCurve8.txt
	Point hairPolishCurve8[] = { {118, -34},  {122, -38},  {121, -44},  {127, -38},  {129, -33},  {126, -30},  {128, -21},  {122, -29}, {END, END} };// 8 points(closed curve)
	// Source: hairPolishCurve9.txt
	Point hairPolishCurve9[] = { {98, -115},  {101, -116},  {102, -118},  {106, -115},  {112, -110},  {110, -107},  {107, -104},  {101, -110}, {END, END} };// 8 points(closed curve)
	// Source: hairPolishCurve10.txt
	Point hairPolishCurve10[] = { {105, -129},  {111, -126},  {119, -117},  {118, -120},  {117, -124},  {113, -127},  {109, -131},  {111, -134},  {112, -137},  {113, -136},  {116, -135},  {113, -139},  {109, -141},  {118, -138},  {130, -129},  {125, -123},  {120, -114},  {109, -123}, {END, END} };// 18 points(closed curve)
	// Source: hairPolishCurve11.txt
	Point hairPolishCurve11[] = { {83, 26},  {80, 34},  {81, 42},  {84, 33}, {END, END} };// 4 points(closed curve)
	// Source: hairPolishCurve12.txt
	Point hairPolishCurve12[] = { {87, 27},  {91, 29},  {95, 26},  {95, 34},  {93, 42},  {90, 41},  {86, 45},  {86, 34}, {END, END} };// 8 points(closed curve)
	// Source: hairPolishCurve13.txt
	Point hairPolishCurve13[] = { {-79, -96},  {-75, -87},  {-67, -85},  {-69, -73},  {-71, -54},  {-81, -62},  {-88, -66},  {-87, -72},  {-86, -80},  {-90, -73},  {-92, -66},  {-94, -71},  {-92, -76},  {-89, -78},  {-86, -83},  {-83, -87}, {END, END} };// 16 points(closed curve)
	// Source: hairPolishCurve14.txt
	Point hairPolishCurve14[] = { {-62, -82},  {-61, -68},  {-65, -50},  {-66, -66}, {END, END} };// 4 points(closed curve)
	// Source: hairPolishCurve15.txt
	Point hairPolishCurve15[] = { {-100, -12},  {-101, -7},  {-101, -2},  {-100, -4},  {-99, -7},  {-96, -2},  {-91, 0},  {-92, 4},  {-95, 10},  {-98, 5},  {-102, 4},  {-103, 3},  {-105, 5},  {-104, -4}, {END, END} };// 14 points(closed curve)
	// Source: hairPolishCurve16.txt
	Point hairPolishCurve16[] = { {-88, 2},  {-80, 5},  {-72, 0},  {-75, 12},  {-81, 24},  {-87, 18},  {-92, 14},  {-90, 7}, {END, END} };// 8 points(closed curve)
	// Source: hairPolishCurve17.txt
	Point hairPolishCurve17[] = { {-143, -77},  {-143, -69},  {-138, -64},  {-140, -59},  {-146, -53},  {-150, -59},  {-152, -62},  {-149, -65},  {-147, -69},  {-150, -67},  {-155, -63},  {-148, -72}, {END, END} };// 12 points(closed curve)
	// Source: hairPolishCurve18.txt
	Point hairPolishCurve18[] = { {-129, -62},  {-134, -54},  {-144, -48},  {-137, -56}, {END, END} };// 4 points(closed curve)
	// Source: hairPolishCurve19.txt
	Point hairPolishCurve19[] = { {-70, 58},  {-65, 63},  {-60, 62},  {-59, 69},  {-56, 80},  {-62, 76},  {-66, 79},  {-69, 68}, {END, END} };// 8 points(closed curve)

	// HairPolishCurveSheet
	Point* HairPolishCurveSheet[] = { hairPolishCurve1, hairPolishCurve2, hairPolishCurve3, hairPolishCurve4, hairPolishCurve5, hairPolishCurve6, hairPolishCurve7, hairPolishCurve8, hairPolishCurve9, hairPolishCurve10, hairPolishCurve11, hairPolishCurve12, hairPolishCurve13, hairPolishCurve14, hairPolishCurve15, hairPolishCurve16, hairPolishCurve17, hairPolishCurve18, hairPolishCurve19, NULL };//all 19 hair curves

	S.ad_fillCurveSheet(HairPolishCurveSheet, 2, HAIRPOLISH_COLOR1, HAIRPOLISH_COLOR2);

	// 白毛
	Point hairCurve2[] = { {-26, -143},  {-27, -120},  {-13, -101},  {13, -87},  {45, -77},  {74, -75},  {113, -82},  {75, -89},  {43, -102},  {58, -95},  {72, -91},  {62, -97},  {56, -104},  {36, -111},  {7, -124},  {16, -115},  {28, -104},  {12, -110},  {-16, -125},  {-26, -135}, {END, END} };// 20 points(closed curve)
	// HairCurveSheet
	Point* HairCurveSheet[] = { hairCurve2, NULL };//all 10 hair curves
	S.ad_fillCurveSheet(HairCurveSheet, 2, RGB(242,243,233), RGB(242, 243, 233));
}

//点眉之笔
static void eyebow(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//填充+划线
	// Source: eyebowCurve1.txt
	Point eyebowCurve1[] = { {-80, -34},  {-65, -44},  {-50, -48},  {-43, -48},  {-35, -46},  {-51, -44}, {END, END} };// 6 points(closed curve)
	// Source: eyebowCurve2.txt
	Point eyebowCurve2[] = { {16, -54},  {25, -61},  {32, -63},  {51, -63},  {72, -58},  {43, -60}, {END, END} };// 6 points(closed curve)

	// eyebowCurveSheet
	Point* eyebowCurveSheet[] = { eyebowCurve1, eyebowCurve2, NULL };//all 2 hair curves
	S.ad_fillCurveSheet(eyebowCurveSheet, 2, HAIR_COLOR, INVALID_RGB, RGB(62, 52, 45));
}

//画头发
static void hair(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//头发轮廓
	//S.ad_fun(hairOutline, 0, 0, 1, 0);
	S.ad_fun(hairBase, 0, 0, 1, 0);
	S.ad_fun(hairEar, 0, 0, 1, 0);
	S.ad_fun(hairLight, 0, 0, 1, 0);
	S.ad_fun(hairPolish, 0, 0, 1, 0);
	S.ad_fun(eyebow, 0, 0, 1, 0);
}

//画头饰
static void headdress(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	const int RBG_blackline = RGB(58, 54, 59);
	int RBG_headdress = RGB(248, 175, 161);
	int he = 25, wd = 13;
	if (rotation_angles > 20)
		RBG_headdress = RGB(255, 255, 255);
	if (rotation_angles < 0) {
		he = 22;
		wd = 16;
	}
	adaptor S(base_x, base_y, size, rotation_angles);
	//菱形黑边
	S.ad_triangle(0,       1, wd+2,  3+he, -wd-2,  3+he, true, 2, RBG_blackline);//黑边
	S.ad_triangle(0,  5+2*he, wd+2,  3+he, -wd-2,  3+he, true, 2, RBG_blackline);//黑边
	S.ad_triangle(0,      -1, wd+2, -3-he, -wd-2, -3+he, true, 2, RBG_blackline);//黑边
	S.ad_triangle(0, -5-2*he, wd+2, -3-he, -wd-2, -3+he, true, 2, RBG_blackline);//黑边
	S.ad_triangle(      1, 0,  wd+3, -2-he,  wd+3, -2-he, true, 2, RBG_blackline);//黑边
	S.ad_triangle( 5+2*wd, 0,  wd+3, -2-he,  wd+3, -2-he, true, 2, RBG_blackline);//黑边
	S.ad_triangle(     -1, 0, -wd-3, -2-he, -wd-3, -2-he, true, 2, RBG_blackline);//黑边
	S.ad_triangle(-5-2*wd, 0, -wd-3, -2-he, -wd-3, -2-he, true, 2, RBG_blackline);//黑边
	//菱形实心
	S.ad_triangle(0,       3, wd,  3+he, -wd,  3+he, true, 2, RBG_headdress);//橙
	S.ad_triangle(0,  3+2*he, wd,  3+he, -wd,  3+he, true, 2, RBG_headdress);//橙
	S.ad_triangle(0,      -3, wd, -3-he, -wd, -3-he, true, 2, RBG_headdress);//橙
	S.ad_triangle(0, -3-2*he, wd, -3-he, -wd, -3-he, true, 2, RBG_headdress);//橙
	S.ad_triangle(      3, 0,  3+wd, -he,  3+wd, he, true, 2, RBG_headdress);//橙
	S.ad_triangle( 3+2*wd, 0,  3+wd, -he,  3+wd, he, true, 2, RBG_headdress);//橙
	S.ad_triangle(     -3, 0, -3-wd, -he, -3-wd, he, true, 2, RBG_headdress);//橙
	S.ad_triangle(-3-2*wd, 0, -3-wd, -he, -3-wd, he, true, 2, RBG_headdress);//橙
	//中间图案
	S.ad_circle(0, 0, 5, false, 1, RBG_blackline);//黑边
	S.ad_line(0, -21, 0, 21, 4, RBG_blackline);//黑边
	S.ad_line(-14, 0, 14, 0, 4, RBG_blackline);//黑边
	S.ad_point(0, 0, 10, RGB(253, 223, 174));//黄
	S.ad_line(0, -20, 0, 20, 3, RGB(253, 223, 174));//黄
	S.ad_line(-13, 0, 13, 0, 3, RGB(253, 223, 174));//黄
}

//画头绳
static void headstring(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	const int headString_color = RGB(232, 16, 44);
	const int blackOutline_color = RGB(62, 52, 45);
	adaptor S(base_x, base_y, size, rotation_angles);
	{
		// Source: headStringCurve1.txt
		Point headStringCurve1[] = { {-113, -102},  {-114, -110},  {-116, -119},  {-124, -127},  {-134, -128},  {-144, -116},  {-147, -99},  {-132, -94},  {-113, -99},  {-116, -104},  {-124, -104},  {-131, -101},  {-140, -102},  {-139, -114},  {-130, -122},  {-125, -118},  {-117, -106},  {-116, -104}, {END, END} };// 18 points(closed curve)
		// Source: headStringCurve2.txt
		Point headStringCurve2[] = { {-117, -108},  {-99, -115},  {-84, -117},  {-77, -112},  {-77, -106},  {-82, -91},  {-93, -82},  {-110, -83},  {-120, -96},  {-115, -101},  {-111, -93},  {-102, -88},  {-94, -89},  {-87, -96},  {-85, -104},  {-87, -110},  {-91, -110},  {-99, -108},  {-107, -105},  {-114, -104}, {END, END} };// 20 points(closed curve)
		// Source: headStringCurve3.txt
		Point headStringCurve3[] = { {-122, -95},  {-136, -86},  {-147, -84},  {-162, -91},  {-177, -103},  {-185, -106},  {-187, -106},  {-202, -103},  {-210, -87},  {-213, -88},  {-214, -87},  {-213, -92},  {-210, -99},  {-216, -93},  {-217, -90},  {-224, -93},  {-229, -100},  {-220, -109},  {-209, -112},  {-222, -111},  {-233, -103},  {-234, -105},  {-235, -106},  {-215, -116},  {-198, -113},  {-182, -114},  {-169, -106},  {-160, -99},  {-147, -92},  {-139, -93},  {-129, -99},  {-124, -100}, {END, END} };// 32 points(closed curve)
		// Source: headStringCurve4.txt
		Point headStringCurve4[] = { {-188, -114},  {-190, -110},  {-187, -106},  {-192, -104},  {-197, -105},  {-200, -111},  {-196, -115},  {-192, -116}, {END, END} };// 8 points(closed curve)
		// Source: headStringCurve5.txt
		Point headStringCurve5[] = { {-112, -83},  {-114, -70},  {-120, -58},  {-133, -48},  {-143, -43},  {-153, -41},  {-161, -41},  {-170, -44},  {-177, -45},  {-180, -39},  {-181, -33},  {-175, -31},  {-166, -29},  {-172, -27},  {-179, -28},  {-167, -25},  {-157, -31},  {-151, -34},  {-142, -36},  {-115, -51},  {-104, -81},  {-108, -87}, {END, END} };// 22 points(closed curve)
		// Source: headStringCurve6.txt
		Point headStringCurve6[] = { {-143, -43},  {-144, -39},  {-141, -36},  {-146, -33},  {-152, -34},  {-152, -38},  {-153, -41},  {-149, -45}, {END, END} };// 8 points(closed curve)

		// headStringCurveSheet
		Point* headStringCurveSheet[] = { headStringCurve3, headStringCurve4, headStringCurve5, headStringCurve6, headStringCurve1, headStringCurve2, NULL };//all 6 headString curves

		S.ad_fillCurveSheet(headStringCurveSheet, 1, headString_color, INVALID_RGB, blackOutline_color);
	}
}

//画北部玄驹的头
static void head(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	S.ad_fun(hairBH, -2, -44, 1.4, 0);
	S.ad_fun(face, 0, 0, 1, 0);
	S.ad_fun(hair, -2, -44, 1.4, 0);
	S.ad_fun(headstring, -2, -44, 1.4, 0);
	S.ad_fun(headdress, -146, -174, 0.6, 22);
	S.ad_fun(headdress, -164, -200, 0.8, 18);
	S.ad_fun(headdress, 99, -242, 0.8, -46);
}

//随机出现的闪烁四芒星
static void rand_star(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	srand((unsigned int)(time(0)));
	const int STAR_COLOR = RGB(255, 255, 255);
	const int SHADE_COLOR = RGB(255, 236, 211);
	int num = rand() % 6 + 4;
	for (int i = 0; i < num; i++) {
		adaptor S(base_x + rand() % (base_x * 2 / 3) - base_x * 2 / 3, base_y + rand() % (base_y * 2 / 3) - base_y * 2 / 3, size * (rand() % 100 / 100 + 1), rotation_angles);
		S.ad_point(0, 0, 8, SHADE_COLOR);
		S.ad_point(0, 0, 6, STAR_COLOR);
		S.ad_line(-14, 0, 14, 0, 2, SHADE_COLOR);
		S.ad_line(0, -14, 0, 14, 2, SHADE_COLOR);
		S.ad_line(-12, 0, 12, 0, 3, STAR_COLOR);
		S.ad_line(0, -12, 0, 12, 3, STAR_COLOR);
	}
}

//“今天的坚宝不是我的对手”
static void words(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	S.ad_line(- 130, - 286, - 100,  - 336, 5, HAIR_COLOR);
	S.ad_line( - 110, - 270, - 70,  - 300, 5, HAIR_COLOR);
	//打印汉字
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cct_gotoxy(base_x / 10 + !!(base_x % 10) - 5, base_y / 20 + !!(base_y % 20) - 21);
	std::cout << "今天的渣哥";
	cct_gotoxy(base_x / 10 + !!(base_x % 10) - 8, base_y / 20 + !!(base_y % 20) - 20);
	std::cout << "不是我北黑的对手!";
	cct_setcolor();
}

//画衣服——这里空间太小，写不下
static void cloth(const int base_x, const int base_y, const double size, const double rotation_angles);

//画身体——这里空间太小，写不下
static void body(const int base_x, const int base_y, const double size, const double rotation_angles);

//主函数
void hdc_draw_cartoon_2252201(const int base_x, const int base_y)
{
	//进入函数后执行的第一个语句必须是 hdc_cls();
	hdc_cls();

	//初始化下背景，我要的是天空色背景
	background(1000, 1000);
	//控制位置，大小及方向的类
	adaptor S(base_x, base_y + 40, 1, 0);

	//轮流演示左眼、右眼、鼻子、嘴巴、脸
	/*void (*fun[])(const int, const int, const double, const double) = {left_eye , right_eye , nose , mouse , face , NULL};
	for (auto pfun = fun; *pfun; pfun++) {
		S.ad_fun(*pfun, 0, 0, 2, 0);
		Sleep(WAIT);
		background(1000, 1000);
	}*/

	//测试曲线
	//S.ad_fun(testCurve, 0, 0, 1.6, 0);

	//演示头部
	//S.ad_fun(face, 0, 0, 2, 0);
	//S.ad_fun(hair, -2, -44, 1.4, 0);
	S.ad_fun(head, 0, 60, 1.2, 0);
	S.ad_fun(rand_star, 0, 0, 1, 0);
	//S.ad_fun(cloth, 0, 0, 1, 0);
	//S.ad_fun(body, 0, 0, 1, 0);

	words(base_x, base_y, 1.2, 0);
	return;
}