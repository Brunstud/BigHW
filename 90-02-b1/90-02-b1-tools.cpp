/* 2252201 ������ �ƿ�2�� */
/*��һЩ�ڲ�����/ͼ�η�ʽ���õĺ���*/
#include<iostream>
//#include<iomanip>
//#include<conio.h>
#include<windows.h>
#include "../include/cmd_console_tools.h"
//#include "../include/com_console.h"
#include"90-02-b1.h"
using namespace std;

/***************************************************************************
  �������ƣ�operate
  ��    �ܣ����ݲ˵�ѡ�����ʵ��
  ���������num * game, int choice
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void operate(num * game, char choice)
{
	if ('F' != choice && 'E' != choice)
		game->ori_paint();
	if ('A' == choice)
		return;
	else if ('B' == choice) {
		game->set_num(0);
		game->set_pos(3, 3);
		for (int i = 0; i < 10; ++i) {
			game->show_nex_num((i + 1) % 10);
			game->paint_num();
			game->time_fly(200);
			for (int j = 0; j < 5; j++) {
				game->rotate();
				game->time_fly(200);
			}
			game->erase_num();
		}
	}
	else if ('C' == choice) {
		game->set_num(get_next_num(true, unsigned int(time(0))));
		game->start_num();
		int drop;
		while (!(drop = game->drop_num())) {
			game->time_fly(200);
			game->rotate();
		}
	}
	else if ('D' == choice) {
		game->set_num(get_next_num(true, unsigned int(time(0))));
		game->start_num();
		while (1) {
			int drop;
			while (!(drop = game->drop_num())) {
				game->time_fly(100);
				game->rotate();
			}
			if (-1 == drop)
				break;
			game->end_a_num();
			game->show_nex_num(get_next_num());
			if (game->start_num())
				break;
		}
	}
	else if ('E' == choice)
		game->GAME(unsigned int(time(0)));
	else if ('F' == choice) {
		int seed;
		while (1) {
			cout << "���������������" << endl;
			cin >> seed;
			if (seed >= 0)
				break;
			if (!cin) {
				cin.clear();
				while ('\n' == getchar())
					continue;
			}
		}
		game->GAME(seed);
	}
	return;
}

/***************************************************************************
  �������ƣ�get_next_num
  ��    �ܣ�������һ�������
  ���������const bool new_seed = false, const unsigned int seed = 0
  �� �� ֵ��������һ������(0-9)
  ˵    ����
***************************************************************************/
int get_next_num(const bool new_seed, const unsigned int seed)
{
	if (new_seed)
		srand(seed);

	return rand() % 10;
}

/***************************************************************************
  �������ƣ�num::GAME
  ��    �ܣ�ʵ��һ����������Ϸ
  ���������int seed
  �� �� ֵ����
  ˵    ������Ҫ�������������
***************************************************************************/
void num::GAME(int seed)
{
	ori_paint(1);//��ӡ�����弰��Ϸ˵��
	set_num(get_next_num(true, seed));
	start_num();
	while (1) {
		int drop = 0;
		while (!drop && !(drop = drop_num())) {
			for (int i = 0; i < 10; i++) {
				if (drop = move())
					break;
				time_fly(100);
			}
		}
		if (-1 == drop)
			break;
		end_a_num();
		show_nex_num(get_next_num());
		if (start_num())
			break;
	}
	return;
}