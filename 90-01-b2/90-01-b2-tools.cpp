/*2252201 胡世成 信18*/
/*放一些内部数字/图形方式公用的函数，如判断结束等*/
#include<iostream>
#include<iomanip>
#include<conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/com_console.h"
#include"90-01-b2.h"
using namespace std;

/***************************************************************************
  函数名称：operate_by_choice
  功    能：依据菜单返回的choice执行各类程序
  输入参数：choice
  返 回 值：无
  说    明：
***************************************************************************/
void operate_by_choice(int star[][LINE], int row, int line, char choice)
{
	int r = 0, l = 0, score = 0;
	char find[ROW][LINE] = { '\0' };
	switch (choice) {
		case 'A':
			cct_setconsoletitle("消灭星星——数组初级版");
			question1(star, find, &r, &l, row, line);
			break;
		case 'B':
			cct_setconsoletitle("消灭星星——数组中级版");
			question2(star, &r, &l, row, line, score);
			break;
		case 'C':
			cct_setconsoletitle("消灭星星——数组完整版");
			question3(star, &r, &l, row, line, score);
			break;
		case 'D':
		case 'E':
			cct_setconsoletitle("消灭星星——打印星星");
			cct_cls();
			ori_paint(star, row, line, choice - 'D');
			choose_star(star, &r, &l, row, line, choice - 'D');
			break;
		case 'F':
			cct_setconsoletitle("消灭星星——第一步");
			cct_cls();
			ori_paint(star, row, line, 0);
			paint_one_step(star, row, line, 0, 1);
			break;
		case 'G':
			cct_setconsoletitle("消灭星星——游戏版");
			paint_game(row, line, 1);
			break;
		case 'H':
			cct_setconsoletitle("消灭星星——自动版");
			paint_game(row, line, 1, -1);
			break;
		default:
			cout << "choice_error!";
	}
	return;
}

/***************************************************************************
  函数名称：find_star
  功    能：查找指定坐标位置及周围相同项的函数(默认该处有值)
  输入参数：const int star[][LINE], char find[][LINE], int r, int l
  返 回 值：int found
  说    明：返回是否有连续相通值
***************************************************************************/
int find_star(const int star[][LINE], char find[][LINE], int r, int l)
{
	int found = 1, twice = 0;
	int origin = star[r][l];
	if (!origin) {
		cout << "error:no star here";
		return 0;
	}
	if (find[r][l])
		found = 0;
	find[r][l] = '*';
	int r_min = (r ? - 1 : 0);
	int r_max = (ROW - 1 == r ? 0 : 1);
	int l_min = (l ? - 1 : 0);
	int l_max = (LINE - 1 == l ? 0 : 1);
	for (int dr = r_min; dr <= r_max; dr++)
		for (int dl = l_min; dl <= l_max; dl++) {
			if (dr && dl || !dr && !dl)
				continue;
			int row = r + dr;
			int line = l + dl;
			if (origin == star[row][line])
				if (!find[row][line]) {
					found++;
					find[row][line] = '*';
					found += find_star(star, find, row, line);
				}
		}
	/*if (!found && !twice)
		find[r][l] = '\0';*/
	return found;
}

/***************************************************************************
  函数名称：delete_star
  功    能：删除对应位置的星星
  输入参数：int star[][LINE], const char find[][LINE]
  返 回 值：无
  说    明：
***************************************************************************/
void delete_star(int star[][LINE], const char find[][LINE], int paint, int div)
{
	for (int r = 0; r < ROW; r++)
		for (int l = 0; l < LINE; l++)
			if (find[r][l]) {
				star[r][l] = 0;
				if (paint) {
					const int x_step = STAR_X + 2 * div;
					const int y_step = STAR_Y + div;
					cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
					for (int i = 0; i < STAR_Y; i++) {
						cct_gotoxy(X_L0 + l * x_step, Y_R0 + r * y_step + i);
						cout << setfill(' ') << setw(STAR_X) << ' ';
					}
					cct_setcolor();
				}
			}
	return;
}

/*******************************************y********************************
  函数名称：fallen_star
  功    能：数组下落与移动
  输入参数：int star[][LINE], char find[][LINE]
  返 回 值：无
  说    明：
***************************************************************************/
void fallen_star(int star[][LINE], const char find[][LINE], int row, int line, int paint, int div)
{
	//===============================================================下落
	/*for (int l = 0; l < line; l++)
		for (int r = 0; r < row; r++) {
			if (find[r][l] && r > 0)
				for (int ro = r; ro > 0 && !find[ro - 1][l]; ro--) {
					find[ro][l] = '\0';
					star[ro][l] = star[ro - 1][l];
					if (paint)
						drop_by_row(star, ro - 1, l, div);
					find[ro - 1][l] = '*';
					star[ro - 1][l] = 0;
				}
		}*/
	for (int l = 0; l < line; l++)
		for (int r = row - 2; r >= 0; r--)
			if (star[r][l] && !star[r + 1][l])
				for (int ro = r; ro < row - 1 && !star[ro + 1][l]; ro++) {
					if (paint)
						drop_by_row(star, ro, l, div);
					star[ro + 1][l] = star[ro][l];
					star[ro][l] = 0;
				}
	//==============================================================并行
	/*for (int l = line - 1; l >= 0; l--)
		if (!star[row - 1][l])
			for (int ln = l; ln < line - 1 && star[row - 1][ln + 1]; ln++)
				for (int r = 0; r < row; r++) {
					if (paint)
						drop_by_line(star, r, ln + 1, div);
					star[r][ln] = star[r][ln + 1];
					star[r][ln + 1] = 0;
				}*/
	for (int l = 0; l < line - 1; l++)
		if (!star[row - 1][l] && star[row - 1][l + 1])
			for (int ln = l + 1; ln > 0 && !star[row - 1][ln - 1]; ln--)
				for (int r = row - 1; r >= 0; r--) {
					if (paint)
						drop_by_line(star, r, ln, div);
					star[r][ln - 1] = star[r][ln];
					star[r][ln] = 0;
				}
	return;
}

/*******************************************y********************************
  函数名称：check_star
  功    能：检查是否还有消除项
  输入参数：int star[][LINE]
  返 回 值：int
  说    明：若有消除项则返回0；若无可消除项，则返回剩余个数
***************************************************************************/
int check_star(const int star[][LINE], int row, int line)
{
	if (!star[row - 1][0])
		return -1;
	int num = 1;
	for (int r = row - 1; r >= 0; r--) {
		for (int l = 0; l < line; l++)
			if (star[r][l]) {
				int r_min = (r ? -1 : 0);
				int r_max = (row - 1 == r ? 0 : 1);
				int l_min = (l ? -1 : 0);
				int l_max = (line - 1 == l ? 0 : 1);
				for (int dr = r_min; dr <= r_max; dr++)
					for (int dl = l_min; dl <= l_max; dl++) {
						if (dr && dl || !dr && !dl)
							continue;
						if (star[r][l] == star[r + dr][l + dl])
							num = 0;
					}
				if (!num)
					break;
				num++;
			}
		if (!num)
			break;
	}
	if (num)
		num--;
	return num;
}

//=======================================================================================================PART H
//=======================================================================================================PART H
//=======================================================================================================PART H
/***************************************************************************
  函数名称：auto_star
  功    能: 终结者
  输入参数：
  返 回 值：无
  说    明：
***************************************************************************/
void auto_star(int row, int line, int div)
{
    
	return;
}

/***************************************************************************
  函数名称：auto_choose_star_block
  功    能: 终结者
  输入参数：
  返 回 值：
  说    明：返回found
***************************************************************************/
int auto_choose_star_block_recur(const int star[][LINE], int * r, int * l, int row, int line)
{
	if (int num = check_star(star, row, line)) {
		num = num > 0 ? num : 0;
		*r = *l = 0;
		return score_in_end(num);
	}

	int score = 0, tmp1 = 0, tmp2 = 0;
	int h_row = row - 1, h_line = 0, h_score = 0;
	char occupy[ROW][LINE] = { '\0' };
	//===========================================================开始遍历
	for (int ro = row - 1; ro >= 0; ro--)
		for (int ln = 0; ln < line; ln++) {
			if (star[ro][ln] && !occupy[ro][ln]) {
				if (find_star(star, occupy, ro, ln) == 1)
					continue;
				//===================================数组准备
				int auto_star[ROW][LINE] = { 0 };
				for (int i = 0; i < row; i++)
					for (int j = 0; j < line; j++)
						if (star[i][j])
							auto_star[i][j] = star[i][j];
				char auto_find[ROW][LINE] = { '\0' };
				//====================================数组准备
				int found = find_star(star, auto_find, ro, ln);
				delete_star(auto_star, auto_find);
				fallen_star(auto_star, auto_find, row, line);
				score = auto_choose_star_block_recur(auto_star, &tmp1, &tmp2, row, line) + score_by(found);
				if (score > h_score) {
					h_row = ro;
					h_line = ln;
					h_score = score;
				}
			}
		}
	*r = h_row;
	*l = h_line;
	return h_score;
}
int auto_choose_star_block(const int star[][LINE], char find[][LINE], int* r, int* l, int row, int line, int div)
{
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	const int end_row = Y_R0 + y_step * row + 1 - div;

	cct_gotoxy(0, end_row);
	cout << "自动计算最佳解法中，请耐心等待。。。";
	cct_gotoxy(0, end_row + 1);
	cout << "进度：" << "  0" << "/" << setw(3) << row * line;

	int score = 0, tmp1 = 0, tmp2 = 0, h_nex_row = 0, h_nex_line = 0;
	int h_row = row - 1, h_line = 0, h_score = 0;
	char occupy[ROW][LINE] = { '\0' };
	//===========================================================开始遍历
	for (int ro = row - 1; ro >= 0; ro--) {
		for (int ln = 0; ln < line; ln++) {
			if (star[ro][ln] && !occupy[ro][ln]) {
				if (find_star(star, occupy, ro, ln) == 1)
					continue;
				//===================================数组准备
				int auto_star[ROW][LINE] = { 0 };
				for (int i = 0; i < row; i++)
					for (int j = 0; j < line; j++)
						if (star[i][j])
							auto_star[i][j] = star[i][j];
				char auto_find[ROW][LINE] = { '\0' };
				//====================================数组准备
				int found = find_star(star, auto_find, ro, ln);
				delete_star(auto_star, auto_find);
				fallen_star(auto_star, auto_find, row, line);
				score = auto_choose_star_block_recur(auto_star, &tmp1, &tmp2, row, line) + score_by(found);
				if (score > h_score) {
					h_row = ro;
					h_line = ln;
					h_nex_row = tmp1;
					h_nex_line = tmp2;
					h_score = score;
				}
				//===========================================================================
				cct_gotoxy(0, end_row + 1);
				cout << "进度：" << setfill(' ') << setw(3) << (row - ro - 1) * line + ln + 1 << "/" << setw(3) << row * line;//进度条
				//cct_gotoxy(0, 0);
				//show_array(NULL, star, occupy, row, line, -1);
				//cout << "当前最高分：" << setw(5) << h_score << " 该次迭代得分：" << setw(5) << score;
				//cout << "下一步" << char(tmp1 + 'A') << tmp2;
				//while (_getch() != '\r')
				//	continue;
				//===========================================================================
			}
		}
	}
	//=================================================================================================
	cct_gotoxy(0, end_row + 1);
	//cout << "             ";
	if (!h_nex_row && !h_nex_line)
		cout << "下一步:   END";
	else
	    cout << "下一步:    " << char(h_nex_row + 'A') << h_nex_line;
	//=================================================================================================
	*r = h_row;
	*l = h_line;
	int found = find_star(star, find, *r, *l);
	paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_HWHITE);
	for (int i = 0; i < row; i++)                                     //标注相邻星星
		for (int j = 0; j < line; j++) {
			if (i == *r && j == *l)
				continue;
			if (find[i][j])
				paint_star(X_L0 + j * x_step, Y_R0 + i * y_step, star[i][j] + 8, COLOR_WHITE);
		}
	cct_gotoxy(0, end_row);
	cout << "该步解如下（按回车继续）               ";
	while (_getch() != '\r')
		continue;
	for (int i = 0; i < row; i++)                                  //取消标注
		for (int j = 0; j < line; j++)
			if (find[i][j])
				paint_star(X_L0 + j * x_step, Y_R0 + i * y_step, star[i][j] + 8, COLOR_BLACK);
	return found;
}