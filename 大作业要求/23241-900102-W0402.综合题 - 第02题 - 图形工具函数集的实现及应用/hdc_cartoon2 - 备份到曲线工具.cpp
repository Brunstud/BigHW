/* 2252201 胡世成 计科2班 */
#include <Windows.h>
#include <cmath>
#include "../include/cmd_hdc_tools.h"

#define LIST_INIT_SIZE 10  //顺序表初始空间单元数
#define LISTINCREMENT 5    //顺序表递增空间单元数
#define END INT_MAX
#define PI          3.1415926	        //圆周率
#define SKIN_COLOR  RGB(253, 244, 226)  //默认肤色
#define HAIR_COLOR  RGB(68, 67, 75)     //默认发色
static HDC hdc = NULL;

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
	void calCurve(Curve *curve,const Point point1, const Point pointm, const Point point2)
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
		curve->center.x = (int)((cMidBC - cMidAB) / (mMidAB - mMidBC));
		curve->center.y = (int)(mMidAB * curve->center.x + cMidAB);

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
				curve->topY = curve->center.y + (int)curve->radius;
				curve->botY = curve->center.y - (int)curve->radius;
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
				curve->topY = curve->center.y + (int)curve->radius;
			if (!curve->above && (botX < curve->center.x && topX > botX || botX > curve->center.x && topX < botX))
				curve->botY = curve->center.y - (int)curve->radius;
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
			const int x0 = (int)(curve->point1.x + (y - curve->point1.y) * (curve->point2.x - curve->point1.x) * 1.0 / (curve->point2.y - curve->point1.y));
			ret->x = x0;
			return 1;
		}
		else {
			const int dx = (int)(sqrt(curve->radius * curve->radius - (y - curve->center.y) * (y - curve->center.y)));
			const int xLeft = curve->center.x - dx;
			const int xRight = curve->center.x + dx;
			if (curve->point1.x == curve->point2.x) {
				if (curve->above && xLeft > curve->point1.x || !curve->above && xLeft < curve->point1.x)
					ret->x = xLeft;
				if (curve->above && xRight > curve->point1.x || !curve->above && xRight < curve->point1.x)
					ret->y = xRight;
			}
			else {
				const int yLeft = (int)(curve->point1.y + (xLeft - curve->point1.x) * (curve->point2.y - curve->point1.y) * 1.0 / (curve->point2.x - curve->point1.x));
				const int yRight = (int)(curve->point1.y + (xRight - curve->point1.x) * (curve->point2.y - curve->point1.y) * 1.0 / (curve->point2.x - curve->point1.x));
				if (curve->above && y > yLeft || !curve->above && y <  yLeft)
					ret->x = xLeft;
				if (curve->above && y > yRight || !curve->above && y < yRight)
					ret->y = xRight;
			}
			return 1;
		}
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
				hdc_arc(pcur->center.x, pcur->center.y, (int)pcur->radius, pcur->angle_beg - 1, pcur->angle_end + 1, thickness, RGB_value);
			else
				hdc_line(pcur->point1.x, pcur->point1.y, pcur->point2.x, pcur->point2.y, thickness, RGB_value);
		}
		return 0;
	}
	//作用：填充闭合曲线
	//参数：RGB_value：曲线颜色
	//返回：填充失败返回-1；曲线不足/曲线不闭合返回-2；正常返回0
	int fill(const int RGB_value = INVALID_RGB) {
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
			//按从小到大重排列
			for (int* pAuxINT = (int*)auxiliary; pAuxINT - (int*)auxiliary < length * 2; pAuxINT++) {
				int* pMIN = NULL;
				for (int* pA = pAuxINT; pA - (int*)auxiliary < length * 2; pA++)
					if (!*pA)
						continue;
					else if (!pMIN || *pA > *pMIN)
						pMIN = pA;
				if (!pMIN)
					break;
				else {
					int tmp = *pAuxINT;
					*pAuxINT = *pMIN;
					*pMIN = tmp;
				}
			}
			//划线
			for (paux = auxiliary; paux->x && paux->y; paux++) {
				hdc_line(paux->x, y, paux->y, y, 2, RGB_value);
			}
		}
		//完成填充，清理现场
		free(auxiliary);
		return 0;
	}
	//释放空间
	void freeCurve() { if (curvelist) free(curvelist); };
};


//调整图像大小的类
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
		hdc_sector(base_x + point_x, base_y + point_y, ad(radius), filled, ad(thickness), RGB_value);
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
		for (; point->x != END; point++) {
			rotation_relative(point->x, point->y);
			point->x = base_x + ad(point->x);
			point->y = base_y + ad(point->y);
		}
		point->x = point->y = NULL;
		return curve.init(firstp);
	}
};

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
	S.ad_ellipse(5, -8, 54, 64, rotation + 30, true, 2, RGB(17, 11, 6));//黑
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

//画面部——尚不可旋转
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
	S.ad_sector(-20, 15, 60, -166, -120, true, 1, SKIN_COLOR);//浅肤色
	S.ad_sector(1, -15, 110, 90, 144, true, 1, SKIN_COLOR);//浅肤色
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
	S.ad_fun(&left_eye, -54, -30, 0.5, 0);
	S.ad_fun(&right_eye, 54, -56, 1.07 / 2, -5);
	S.ad_fun(&nose, 0, 0, 0.6, 0);
	S.ad_fun(&mouse, 20, 36, 1, -15);
}

//画发丝
/*static void hairline(adaptor & S, const int x1, const int y1, const int x2, const int y2, const int x3, const int y3, const int deline1, const int deline2, const int RGB_value)
{
	const double distance_sq = (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
	if (!distance_sq)
		S.ad_rcurve(x1, y1, x3, y3, (deline1 + deline2) / 2, 2, RGB_value);
	double dx = (x2 - x1) / sqrt(distance_sq);//从(x1,y1)开始
	double dy = (y2 - y1) / sqrt(distance_sq);//从(x1,y1)开始
	double dr = (deline2 - deline1) / sqrt(distance_sq);
	double x, y, r;
	int x_int, y_int, r_int;

	// 在线段上遍历每个相邻像素点
	for (x = y = r = 0; x * x + y * y <= distance_sq; x += 2 * dx, y += 2 * dy, r += 2 * dr) {
		x_int = x1 + (int)(x);
		y_int = y1 + (int)(y);
		r_int = deline1 + (int)(r);
		S.ad_rcurve(x_int, y_int, x3, y3, r_int, 4, RGB_value);
	}
}
//第一措毛
static void hair1(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//第一措毛
	hairline(S, -66, -65, -35, -60, 33, 96, 180, 200, HAIR_COLOR);
	hairline(S, 7, -59, -35, -60, 92, 98, 180, 180, HAIR_COLOR);
	hairline(S, 7, -59, 20, -92, 95, 63, 180, 300, HAIR_COLOR);
    //发根
	hairline(S, -66, -60, 7 , -56, -23, -171, -180, -300, HAIR_COLOR);
	hairline(S, 7  , -56, 20, -92, -23, -171, -180, -300, HAIR_COLOR);
	//上光
}
//第二措毛
static void hair2(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//第三根毛
	hairline(S, -21, 146, 9, 128, -5, 226, 180, 300, HAIR_COLOR);
	//第二根毛
	hairline(S, 35, -46, 74, -44, -66, 160, -200, -100, HAIR_COLOR);
	//第一根毛
	hairline(S, 74, -44, 117, -43, 60 , 164, 300, 500, HAIR_COLOR);
	hairline(S, 56, 36 , 68 , 114, 60 , 164, 180, 400, HAIR_COLOR);
	hairline(S, 48, 100, 68 , 164, 78 , 196, 100, 300, HAIR_COLOR);
	//发根
	hairline(S, 35, -46, 112, -32, 144, -236, -300, -400, HAIR_COLOR);
	hairline(S, 35, -46, 112, -60, 144, -236, -300, -400, HAIR_COLOR);
	//上光
}*/

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

//画头发——尚不可旋转
static void hair(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//头发轮廓
	hairOutline(base_x, base_y, size, rotation_angles);
	/*//曲线绘制使用示例
	Point curveTest[] = {{0,0},{20,-40},{80,0}, {60,20}, {50,-20}, {30,-20}, {END, END}};
	CurveList curve;
	S.ad_initCurve(curve, curveTest);
	curve.draw(2, HAIR_COLOR);
	curve.fill(HAIR_COLOR);
	curve.freeCurve();*/
}

//画头饰
static void headdress(const int base_x, const int base_y, const double size, const double rotation_angles);

//画北部玄驹的头
static void head(const int base_x, const int base_y, const double size, const double rotation_angles);

//画衣服
static void cloth(const int base_x, const int base_y, const double size, const double rotation_angles);

//画身体
static void body(const int base_x, const int base_y, const double size, const double rotation_angles);

void hdc_draw_cartoon_2252201(const int base_x, const int base_y)
{
	//进入函数后执行的第一个语句必须是 hdc_cls();
	hdc_cls();

	//初始化下背景，我要的是天空色背景
	background(800, 800);
	adaptor S(base_x, base_y, 1, 0);

	//S.ad_fun(left_eye, 0, 0, 2, 0);
	//S.ad_fun(right_eye, 0, 0, 2, 0);
	//S.ad_fun(nose, 0, 0, 2, 0);
	//S.ad_fun(mouse, 0, 0, 2, 0);
	S.ad_fun(face, 0, 0, 1, 0);
	S.ad_fun(hair, 0, - 40, 1.4, 0);
	//S.ad_fun(headdress, 0, 0, 1, 0);
    //S.ad_fun(head, 0, 0, 1, 0);
	//S.ad_fun(cloth, 0, 0, 1, 0);
	//S.ad_fun(body, 0, 0, 1, 0);

	return;
}