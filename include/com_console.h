/* 2252201 ������ �ƿ�2�� */
#pragma once

/* ������ɫ���ú궨��ȡ�����֣�������䣩 */
#define COLOR_BLACK		0	//��
#define COLOR_BLUE		1	//��
#define COLOR_GREEN		2	//��
#define COLOR_CYAN		3	//��
#define COLOR_RED		4	//��
#define COLOR_PINK		5	//��
#define COLOR_YELLOW	6	//��
#define COLOR_WHITE		7	//��
#define COLOR_HBLACK	8	//����
#define COLOR_HBLUE		9	//����
#define COLOR_HGREEN	10	//����
#define COLOR_HCYAN		11	//����
#define COLOR_HRED		12	//����
#define COLOR_HPINK		13	//����
#define COLOR_HYELLOW	14	//����
#define COLOR_HWHITE	15	//����

/*�������ò���*/
#define STAR_X 6
#define STAR_Y 3

/*��cmdͼ�ν��淽ʽʵ�ֵĹ��ú���*/
void paint_star(const int x, const int y, const int bg_color = COLOR_WHITE, const int fg_color = COLOR_BLACK);
void erase_star(const int x, const int y, const int bg_color = COLOR_HWHITE);
void bg_paint(const int width, const int height, const int bg_color = COLOR_HWHITE, const int fg_color = COLOR_BLACK);
int move_keyboard();