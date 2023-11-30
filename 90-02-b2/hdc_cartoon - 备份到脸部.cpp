/* 2252201 ºúÊÀ³É ¼Æ¿Æ2°à */
#include <Windows.h>
#include <cmath>
#include "../include/cmd_hdc_tools.h"

#define PI          3.1415926	        //Ô²ÖÜÂÊ
#define SKIN_COLOR  RGB(253, 244, 226)  //Ä¬ÈÏ·ôÉ«

//ÖÐÐÄµãÎ»ÖÃ
static int base_x = 400;
static int base_y = 400;

//½Ç¶È×ª»¡¶È
static inline double deg2rad(const double angle)
{
	return angle * PI / 180;
}


//µ÷ÕûÍ¼Ïñ´óÐ¡µÄÀà
class adaptor {
	const int base_x;
	const int base_y;
	const double size;
	const double ad_rotation_ang;
	inline void rotation_relative(int& x, int& y, const int relative_x, const int relative_y)
	{
		const double rotation_rads = deg2rad(ad_rotation_ang);
		x = (int)(relative_x * cos(rotation_rads) - relative_y * sin(rotation_rads));
		y = (int)(relative_x * sin(rotation_rads) + relative_y * cos(rotation_rads));
		return;
	}
public:
	adaptor(int x, int y, double s, double ro) : base_x(x), base_y(y), size(s), ad_rotation_ang(ro) {}
	inline int ad(int ori)
	{
		return int(ori * size);
	}
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
};

//±³¾°Ìî³ä
static void background(const int width, const int height)
{
	const int BACKGROUND = RGB(202, 255, 255);
	hdc_rectangle(0, 0, width, height, 0, true, 2, BACKGROUND);
}

//»­Í«¿×
static void pupil(const int base_x, const int base_y, const double size, const double rotation_angles) 
{
	adaptor S(base_x, base_y, size, rotation_angles);
	const int rotation = -15;
	//Í«¿×
	S.ad_ellipse(16, 0, 40, 56, rotation, true, 2, RGB(75, 30, 40));//Éîºì
	S.ad_ellipse(20, 18, 34, 36, rotation, true, 2, RGB(215, 78, 96));//ÁÁºì
	//Í«ÐÄ
	S.ad_ellipse(16, 0, 20, 28, rotation, true, 2, RGB(96, 29, 42));//ºÖ
	S.ad_ellipse(16, 0, 3, 4, rotation, true, 2, RGB(185, 149, 157));//°×
	//ÉÏ¹â
	S.ad_ellipse(28, -34, 12, 18, rotation - 30, true, 2, RGB(143, 115, 236));//×Ï
	S.ad_ellipse(-9, -31, 8, 12, rotation - 60, true, 2, RGB(255, 255, 255));//°×
	S.ad_ellipse(25, 34, 27, 20, rotation, true, 2, RGB(255, 195, 145));//³È
	S.ad_ellipse(21, 19, 15, 5, rotation, true, 2, RGB(254, 39, 46));//ºì
	S.ad_ellipse(23, 23, 9, 3, rotation, true, 2, RGB(254, 39, 46));//ºì
	//Í«µ×µã×º
	S.ad_rectangle(42, 26, 4, 3, rotation - 30, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º
	S.ad_rectangle(32, 34, 8, 3, rotation - 20, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º
	S.ad_rectangle(23, 37, 6, 3, rotation, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º
	S.ad_rectangle(15, 37, 4, 3, rotation + 10, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º
	S.ad_rectangle(2, 32, 8, 3, rotation + 36, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º
}

//»­×óÑÛ
static void left_eye(const int base_x, const int base_y ,const double size, const double rotation_angles) 
{
	adaptor S(base_x, base_y, size, rotation_angles);
	const int rotation = -15;
	//±³¾°·ôÉ«
	//S.ad_ellipse(0, 0, 80, 100, rotation, true, 2, SKIN_COLOR);//·ôÉ«
	//ÑÛÀª
    S.ad_arc(-16, -14, 76, rotation - 45, rotation + 60, 2, RGB(11, 12, 6));//ºÚ
	S.ad_triangle(-36, -70, -28, -80, -42, -97, true, 2, RGB(11, 12, 6));//ºÚ
	S.ad_ellipse(-6, -16, 58, 70, rotation - 30, true, 2, RGB(11, 12, 6));//ºÚ
	//ÉÏ½ÞÃ«
	S.ad_triangle(-68, -4, -60, -38, -86, -42, true, 2, RGB(87, 44, 38));//ºÖ
	S.ad_triangle(-60, -38, -46, -66, -77, -40, true, 2, RGB(59, 31, 26));//ºÖ
	S.ad_ellipse(-6, -10, 60, 70, rotation - 30, true, 2, RGB(136, 24, 47));//ºÖ
	S.ad_ellipse(-2, -4, 58, 70, rotation - 30, true, 2, RGB(17, 11, 6));//ºÚ
	//ÏÂ½ÞÃ«
	S.ad_line(-33, 57, 0, 0, 3, RGB(120, 55, 51));//ºÖ
	S.ad_line(-27, 59, 5, 0, 3, RGB(120, 55, 51));//ºÖ
	//Í«°×
	S.ad_ellipse(0, 0, 68, 56, rotation + 55, true, 2, RGB(227, 208, 208));//»Ò°×
	S.ad_ellipse(4, 11, 58, 48, rotation + 25, true, 2, RGB(255, 246, 243));//°×
	pupil(base_x, base_y, size, 0);
	/*//Í«¿×
	S.ad_ellipse(16, 0, 40, 56, rotation, true, 2, RGB(75, 30, 40));//Éîºì
	S.ad_ellipse(20, 18, 34, 36, rotation, true, 2, RGB(215, 78, 96));//ÁÁºì
	//Í«ÐÄ
	S.ad_ellipse(16, 0, 20, 28, rotation, true, 2, RGB(96, 29, 42));//ºÖ
	S.ad_ellipse(16, 0, 3, 4, rotation, true, 2, RGB(185, 149, 157));//°×
	//ÉÏ¹â
	S.ad_ellipse(28, -34, 12, 18, rotation - 30, true, 2, RGB(143, 115, 236));//×Ï
	S.ad_ellipse(-9, -31, 8, 12, rotation - 60, true, 2, RGB(255, 255, 255));//°×
	S.ad_ellipse(25, 34, 27, 20, rotation, true, 2, RGB(255, 195, 145));//³È
	S.ad_ellipse(21, 19, 15, 5 , rotation, true, 2, RGB(254, 39, 46));//ºì
	S.ad_ellipse(23, 23, 9 , 3 , rotation, true, 2, RGB(254, 39, 46));//ºì
	//Í«µ×µã×º
	S.ad_rectangle(42, 26, 4, 3, rotation - 30, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º
	S.ad_rectangle(32, 34, 8, 3, rotation - 20, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º
	S.ad_rectangle(23, 37, 6, 3, rotation, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º
	S.ad_rectangle(15, 37, 4, 3, rotation + 10, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º
	S.ad_rectangle(2, 32, 8, 3, rotation + 36, true, 2, RGB(255, 255, 213));//°×Æ«»Æµã×º*/
	//ÏÂÑÛÀª
	S.ad_arc(10, 3, 60, rotation + 147, rotation + 192, 4, RGB(135, 24, 47));//ºÖ
}

//»­ÓÒÑÛ
static void right_eye(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	const int rotation = -5;
	//±³¾°·ôÉ«
	//S.ad_ellipse(0, 0, 80, 100, rotation, true, 2, SKIN_COLOR);//·ôÉ«
	//ÑÛÀª
	S.ad_arc(16, -14, 76, rotation - 60, rotation + 45, 2, RGB(11, 12, 6));//ºÚ
	S.ad_triangle(36, -70, 28, -76, 42, -97, true, 2, RGB(11, 12, 6));//ºÚ
	S.ad_ellipse(6, -16, 56, 66, rotation + 30, true, 2, RGB(11, 12, 6));//ºÚ
	//ÉÏ½ÞÃ«
	S.ad_triangle(66, -4, 60, -38, 86, -42, true, 2, RGB(87, 44, 38));//ºÖ
	S.ad_triangle(60, -38, 46, -66, 77, -40, true, 2, RGB(59, 31, 26));//ºÖ
	S.ad_ellipse(6, -10, 56, 66, rotation + 30, true, 2, RGB(136, 24, 47));//ºÖ
	S.ad_ellipse(5, -8, 54, 64, rotation + 30, true, 2, RGB(17, 11, 6));//ºÚ
	//ÏÂ½ÞÃ«
	S.ad_line(52, 41, 0, 0, 3, RGB(120, 55, 51));//ºÖ
	S.ad_line(47, 44, -5, 0, 3, RGB(120, 55, 51));//ºÖ
	//Í«°×
	S.ad_ellipse(4, -4, 60, 54, rotation - 55, true, 2, RGB(227, 208, 208));//»Ò°×
	S.ad_ellipse(6, 9, 54, 44, rotation - 25, true, 2, RGB(255, 246, 243));//ÁÁ°×
	//Í«¿×
	pupil(base_x - S.ad(10), base_y - S.ad(4), size , rotation_angles + 15);
	//ÏÂÑÛÀª
	S.ad_arc(5, 0, 56, rotation + 156, rotation + 202, 4, RGB(135, 24, 47));//ºÖ
}

//»­±Ç×Ó
static void nose(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//Èùºì
	S.ad_ellipse(4, -2, 18, 24, 0, true, 2, RGB(254, 234, 220));//Èùºì1
	S.ad_ellipse(4, -2, 12, 16, 0, true, 2, RGB(254, 222, 213));//Èùºì2
	S.ad_ellipse(4, -2, 6, 8, 0, true, 2, RGB(254, 217, 208));//Èùºì3
	//±Ç¼â
	S.ad_line(-2, 0, -2, 10, 4, RGB(191, 133, 117));//Ç³ºÖ
	//±ÇÉí
	S.ad_sector(0, -2, 8, 0, 180, true, 2, RGB(255, 255, 255));//´¿°×
}

//»­×ì°Í
static void mouse(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//ÉÏ´½
    //ÏÂ´½
	S.ad_arc(0, 10, 29, 165, 195, 4, RGB(253, 247, 246));//ÁÁ°×
	S.ad_arc(0, 10, 35, 176, 184, 2, RGB(253, 200, 194));//Éî·ôÉ«
	//¿Ú
	S.ad_sector(0, 0, 30, 90, -90, true, 2, RGB(149, 71, 58));//ºÖ
	S.ad_arc(0, 90, 93, -20, 20, 4, RGB(149, 71, 58));//ºÖ
	S.ad_arc(30, 0, 60, -110, -90, 4, RGB(149, 71, 58));//ºÖ
	S.ad_arc(-30, 0, 60, 90, 110, 4, RGB(149, 71, 58));//ºÖ
	S.ad_arc(-3, 10, 26, -180, -90, 4, RGB(149, 71, 58));//ºÖ
	S.ad_arc(3, 10, 26, 90, 180, 4, RGB(149, 71, 58));//ºÖ
	//Éà
	S.ad_sector(0, 10, 28, 90, -90, true, 2, RGB(253, 165, 157));//ºìÉà
	S.ad_sector(-16, 10, 12, -90, 0, true, 2, RGB(253, 165, 157));//ºìÉà
	S.ad_sector(16, 10, 12, 0, 90, true, 2, RGB(253, 165, 157));//ºìÉà
	S.ad_arc(0, 91, 93, -14, 14, 4, RGB(253, 165, 157));//ºìÉà
	S.ad_rectangle(-16, 0, 32, 10, 0, true, 2, RGB(253, 165, 157));//ºìÉà
}

//»­Ãæ²¿
static void face(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//·ôÉ«Ìî³ä
	//S.ad_rectangle(-120, -120, 300, 300, -5, true, 2, SKIN_COLOR);//Ç³·ôÉ«
	S.ad_rectangle(105, -90, 218, 50, 170, true, 2, SKIN_COLOR);//Ç³·ôÉ«
	S.ad_triangle(-72, 44, -110, -50, 105, -90, true, 2, SKIN_COLOR);//Ç³·ôÉ«
	S.ad_triangle(-72, 44, 110, -10, 105, -90, true, 2, SKIN_COLOR);//Ç³·ôÉ«
	S.ad_triangle(-36, 72, 34, 92, 105, -90, true, 2, SKIN_COLOR);//Ç³·ôÉ«
	S.ad_triangle(65, 73, 34, 92, -110, -50, true, 2, SKIN_COLOR);//Ç³·ôÉ«
	S.ad_triangle(-36, 72, 105, -90, -110, -50, true, 2, SKIN_COLOR);//Ç³·ôÉ«
	S.ad_triangle(65, 73, 105, -90, -110, -50, true, 2, SKIN_COLOR);//Ç³·ôÉ«
	S.ad_sector(-20, 15, 60, -166, -120, true, 1, SKIN_COLOR);//Ç³·ôÉ«
	S.ad_sector(1, -15, 110, 90, 144, true, 1, SKIN_COLOR);//Ç³·ôÉ«
	//×óÑÛÈùºì
	S.ad_ellipse(-38, 2, 24, 12, -18, true, 2, RGB(254, 234, 220));//Èùºì1
	S.ad_ellipse(-38, 2, 16, 8, -18, true, 2, RGB(254, 222, 213));//Èùºì2
	S.ad_ellipse(-38, 2, 8, 4, -18, true, 2, RGB(254, 217, 208));//Èùºì3
	//ÓÒÑÛÈùºì
	S.ad_ellipse(64, -24, 24, 12, -10, true, 2, RGB(254, 234, 220));//Èùºì1
	S.ad_ellipse(64, -24, 16, 8, -10, true, 2, RGB(254, 222, 213));//Èùºì2
	S.ad_ellipse(64, -24, 8, 4, -10, true, 2, RGB(254, 217, 208));//Èùºì3
	//Á³±ßÏß
	S.ad_line(-72, 44, -110, -50, 2, RGB(80, 36, 23));//ÂÖÀªºÖÉ«
	S.ad_line(-36, 72, 34, 92, 2, RGB(80, 36, 23));//ÂÖÀªºÖÉ«
	S.ad_arc(-20, 15, 60, -166, -120, 1, RGB(80, 36, 23));//ÂÖÀªºÖÉ«
	S.ad_line(65, 73, 34, 92, 2, RGB(80, 36, 23));//ÂÖÀªºÖÉ«
	S.ad_line(110, -10, 105, -90, 2, RGB(80, 36, 23));//ÂÖÀªºÖÉ«
	S.ad_arc(1, -15, 110, 90, 144, 1, RGB(80, 36, 23));//ÂÖÀªºÖÉ«
	//Îå¹Ù
	left_eye(base_x - S.ad(54), base_y - S.ad(30), size / 2, rotation_angles);
	right_eye(base_x + S.ad(54), base_y - S.ad(56), size * 1.07 / 2, rotation_angles - 5);
	nose(base_x, base_y, size * 1.2 / 2, rotation_angles);
	mouse(base_x + S.ad(20), base_y + S.ad(36), size, rotation_angles - 15);
}

//»­Í··¢
static void hair(const int base_x, const int base_y, const double size, const double rotation_angles)
{
	adaptor S(base_x, base_y, size, rotation_angles);
	//
}

//»­Í·ÊÎ
static void headdress(const int base_x, const int base_y, const double size, const double rotation_angles);

//»­±±²¿Ðþ¾ÔµÄÍ·
static void head(const int base_x, const int base_y, const double size, const double rotation_angles);

//»­ÒÂ·þ
static void cloth(const int base_x, const int base_y, const double size, const double rotation_angles);

//»­ÉíÌå
static void body(const int base_x, const int base_y, const double size, const double rotation_angles);

void hdc_draw_cartoon_2252201(const int base_x, const int base_y)
{
	//½øÈëº¯ÊýºóÖ´ÐÐµÄµÚÒ»¸öÓï¾ä±ØÐëÊÇ hdc_cls();
	hdc_cls();

	//³õÊ¼»¯ÏÂ±³¾°£¬ÎÒÒªµÄÊÇÌì¿ÕÉ«±³¾°
	background(800, 800);

	//left_eye(base_x, base_y, 2, 0);
	//right_eye(base_x, base_y, 2, 0);
	//nose(base_x, base_y, 2, 0);
	//mouse(base_x, base_y, 2, 0);
	face(base_x, base_y, 2, 0);
	//hair(base_x, base_y);
	//headdress(base_x, base_y);
    //head(base_x, base_y);
	//cloth(base_x, base_y);
	//body(base_x, base_y);

	return;
}