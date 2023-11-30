/* 2252201 ������ �ƿ�2�� */
#include <iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
using namespace std;

/***************************************************************************
  �������ƣ�paint_star
  ��    �ܣ���������
  ���������int x, int y, int bg_color, int fg_color
  �� �� ֵ����
  ˵    ������������Ĭ������
***************************************************************************/
void paint_star(const int x, const int y, const int bg_color, const int fg_color)
{
	cct_setcolor(bg_color, fg_color);
	cct_gotoxy(x, y);
	cout << "�X�T�[";
	cct_gotoxy(x, y + 1);
	cout << "�U��U";
	cct_gotoxy(x, y + 2);
	cout << "�^�T�a";
	cct_setcolor();
	return;
}

/***************************************************************************
  �������ƣ�erase_star
  ��    �ܣ���������
  ���������int x, int y, int bg_color
  �� �� ֵ����
  ˵    ������������Ĭ������
***************************************************************************/
void erase_star(const int x, const int y, const int bg_color)
{
	cct_setcolor(bg_color);
	cct_gotoxy(x, y);
	cout << "      ";
	cct_gotoxy(x, y + 1);
	cout << "      ";
	cct_gotoxy(x, y + 2);
	cout << "      ";
	cct_setcolor();
	return;
}

/***************************************************************************
  �������ƣ�bg_paint
  ��    �ܣ��ӹ��λ�ÿ�ʼ���Ʊ�����
  ���������int width, int height, int bg_color = COLOR_HWHITE, int fg_color = COLOR_BLACK
  �� �� ֵ����
  ˵    ����Ĭ�Ϻڱ߰׵ף�w��h��ʾ�ڲ��ռ���
  ע    �⣺��1���ڻ��Ʊ�����ǰ��ע��������ڴ�С
            ��2��width���Ϊ2�ı�����������һ
***************************************************************************/
void bg_paint(const int width, const int height, const int bg_color, const int fg_color)
{
	int x, y;
	cct_getxy(x, y);
	cct_setcolor(bg_color, fg_color);
	//==============================��ӡ������
	cct_gotoxy(x-2, y-1);
	cout << "�X";
	for (int i = 0; i < width / 2; i++)
		cout << "�T";
	cout << "�[";
	if (width % 2) {
		cct_gotoxy(x-2, y-1);
		cout << "warning: width is not an even!";
	}
	for (int i = 0; i < height; i++) {
		cct_gotoxy(x-2, y + i);
		cout << "�U";
		for (int j = 0; j < width / 2; j++)
			cout << "  ";
		cout << "�U";
	}
	cct_gotoxy(x-2, y + height);
	cout << "�^";
	for (int i = 0; i < width / 2; i++)
		cout << "�T";
	cout << "�a";
	cct_setcolor();
	return;
}

/***************************************************************************
  �������ƣ�move_keyboard
  ��    �ܣ��ж��Ƿ��м��̰��������ж��ƶ�����
  �����������
  �� �� ֵ����1����2����3����4����������-1��ȷ������-2
            ���޹涨���̲���������0��
  ˵    ����W���ϼ�ͷ��ʾ���ϡ���S���¼�ͷ��ʾ���¡���A�����ͷ��ʾ���󡱣�D���Ҽ�ͷ��ʾ���ҡ���
            Q��ESC��ʾ�����������س���ո��ʾ��ȷ����
  ע    �⣺
***************************************************************************/
int move_keyboard()
{
	if (_kbhit()) {
		int X = 0, Y = 0;
		int ret, maction;
		int keycode1, keycode2;
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_KEYBOARD_EVENT) {
			switch (keycode1) {
				case 13:    //�س�
				case 32:    //�ո�
					return -2;
				case 'Q':
				case 'q':
				case 27:    //ESC
					return -1;
					break;
				case 'W':
				case 'w':
					return 1;
				case 'S':
				case 's':
					return 2;
				case 'A':
				case 'a':
					return 3;
				case 'D':
				case 'd':
					return 4;
				case 224:
					switch (keycode2) {
						case KB_ARROW_UP:            //�ϼ�ͷ
							return 1;
						case KB_ARROW_DOWN:          //�¼�ͷ
							return 2;
						case KB_ARROW_LEFT:          //���ͷ
							return 3;
						case KB_ARROW_RIGHT:          //�Ҽ�ͷ
							return 4;
					}
				default:
					return 0;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT��
	}
	return 0;
}