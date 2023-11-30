/* 2252201 胡世成 计科2班 */
/*放一些内部数字/图形方式公用的函数*/
#include<iostream>
//#include<iomanip>
//#include<conio.h>
#include<windows.h>
#include "../include/cmd_console_tools.h"
//#include "../include/com_console.h"
#include"90-02-b1.h"
using namespace std;

/***************************************************************************
  函数名称：operate
  功    能：根据菜单选择进行实现
  输入参数：num * game, int choice
  返 回 值：无
  说    明：
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
			cout << "请输入随机数种子" << endl;
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
  函数名称：get_next_num
  功    能：给出下一个随机数
  输入参数：const bool new_seed = false, const unsigned int seed = 0
  返 回 值：返回下一个数字(0-9)
  说    明：
***************************************************************************/
int get_next_num(const bool new_seed, const unsigned int seed)
{
	if (new_seed)
		srand(seed);

	return rand() % 10;
}

/***************************************************************************
  函数名称：num::GAME
  功    能：实现一局完整的游戏
  输入参数：int seed
  返 回 值：无
  说    明：需要传递随机数种子
***************************************************************************/
void num::GAME(int seed)
{
	ori_paint(1);//打印背景板及游戏说明
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