/* 2252201 胡世成 计科2班 */
#pragma once

/* 定义颜色（用宏定义取代数字，方便记忆） */
#define COLOR_BLACK		0	//黑
#define COLOR_BLUE		1	//蓝
#define COLOR_GREEN		2	//绿
#define COLOR_CYAN		3	//青
#define COLOR_RED		4	//红
#define COLOR_PINK		5	//粉
#define COLOR_YELLOW	6	//黄
#define COLOR_WHITE		7	//白
#define COLOR_HBLACK	8	//亮黑
#define COLOR_HBLUE		9	//亮蓝
#define COLOR_HGREEN	10	//亮绿
#define COLOR_HCYAN		11	//亮青
#define COLOR_HRED		12	//亮红
#define COLOR_HPINK		13	//亮粉
#define COLOR_HYELLOW	14	//亮黄
#define COLOR_HWHITE	15	//亮白

/*绘制所用参数*/
#define STAR_X 6
#define STAR_Y 3

/*放cmd图形界面方式实现的共用函数*/
void paint_star(const int x, const int y, const int bg_color = COLOR_WHITE, const int fg_color = COLOR_BLACK);
void erase_star(const int x, const int y, const int bg_color = COLOR_HWHITE);
void bg_paint(const int width, const int height, const int bg_color = COLOR_HWHITE, const int fg_color = COLOR_BLACK);
int move_keyboard();