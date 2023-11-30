/* 2252201 ������ ��18 */
#include<iostream>
#include <conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/menu.h"
#include"90-01-b1-hanoi.h"
using namespace std;

/* ----------------------------------------------------------------------------------

     ���ļ����ܣ�
	1����main����
	2����ʼ����Ļ
	3�����ò˵�������hanoi_menu.cpp�У�������ѡ��
	4������ѡ����ò˵������Ӧ��ִ�к�����hanoi_multiple_solutions.cpp�У�

     ���ļ�Ҫ��
	1����������ȫ�ֱ��������ⲿȫ�ֺ;�̬ȫ�֣�const��#define�������Ʒ�Χ�ڣ�
	2����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	3���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main()
{
	/* demo������ִ�д˾䣬��cmd��������Ϊ40��x120�У����������120�У�����9000�У���cmd�����Ҳ���д�ֱ�����ˣ�*/
	cct_setconsoleborder(120, 40, 120, 9000);
	int choice;
	const char* instru[] = { "�ŵ����", "1.������", "2.������(������¼)", "3.�ڲ�������ʾ(����)", "4.�ڲ�������ʾ(���� + ����)", "5.ͼ�ν� - Ԥ�� - ������Բ��",
							 "6.ͼ�ν� - Ԥ�� - ����ʼ���ϻ�n������", "7.ͼ�ν� - Ԥ�� - ��һ���ƶ�", "8.ͼ�ν� - �Զ��ƶ��汾", "9.ͼ�ν� - ��Ϸ��", "0.�˳�", NULL};
	while (choice = menu(instru) - '0') {
		int N = 0;//���ڼ���
		int top[3] = { 0 };//ջ��ָ��
		int stack[3][Layers] = { 0 };//Tony Stark
		int n, speed = -1;
		char src, tmp, dst;
		if (5 == choice) {
			cct_cls();
			print_base();
		}
		else {
			if (4 == choice || 8 == choice)
				speed = input(&n, &src, &tmp, &dst, 1);
			else
				input(&n, &src, &tmp, &dst, 0);
			ori_set_stack(stack, top, n, src);
			start(stack, top, n, src, dst, speed, choice);
		}
		if (9 != choice && !(choice >= 5 && choice <= 7))
			hanoi(stack, top, n, src, tmp, dst, 1, speed, choice);
		if (9 == choice)
			hanoi_game(stack, top, n, src, dst);
		if (8 == choice)
			cct_gotoxy(0, 38);
		cout << endl << "���س�������";
		while (_getch() != '\r')
			continue;
	}
	return 0;
}
