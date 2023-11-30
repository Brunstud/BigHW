/*2252201 胡世成 信18 */
/*放cmd图形界面方式实现的各函数*/
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/com_console.h"
#include"90-01-b2.h"
using namespace std;

/***************************************************************************
  函数名称：ori_paint
  功    能：绘制初始绘图
  输入参数：const int star[][LINE], int row, int line, int div
  返 回 值：无
  说    明：
***************************************************************************/
void ori_paint(const int star[][LINE], int row, int line, int div)
{
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	cct_gotoxy(0, 0);
	if (div) {
		cct_setconsoleborder(x_step * line + 5, y_step * row + 7);
		cout << "屏幕当前设置为：" << y_step * row + 7 << "行" << x_step * line + 5 << "列";
	}
	else {
		cct_setconsoleborder(x_step * line + 7, y_step * row + 8);
		cout << "屏幕当前设置为：" << y_step * row + 8 << "行" << x_step * line + 7 << "列";
	}
	//==============================打印白底
	cct_gotoxy(1 - div, 1);
	for (int i = 0; i < line; i++)
		cout << setfill(' ') << setw(x_step) << i;
	cct_gotoxy(2, 2);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "╔";
	for (int i = 0; i < line * (x_step / 2) - div; i++)
		if (!div || (i + 1) % (x_step / 2))
			cout << "═";
		else
			cout << "╦";
	cout << "╗";
	for (int i = 0; i < y_step * row - div; i++) {
		if (!((i - 1) % y_step)) {
			cct_setcolor();
			cct_gotoxy(0, i + 3);
			cout << char((i - 1) / y_step + 'A');
			cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
		}
		cct_gotoxy(2, i + Y_R0);
		if (!div || (i + 1) % y_step) {
			cout << "║";
			for (int j = 0; j < line; j++) {
				cout << setw(x_step - 2) << ' ';
				if (j < line - 1 || !div)
					cout << (div ? "║" : "  ");
				Sleep(WAIT);
			}
			cout << "║";
		}
		else {
			cout << "╠";
			for (int j = 0; j < line; j++) {
				for (int k = 0; k < STAR_X / 2; k++)
				    cout << "═";
				if (j < line - 1)
					cout << "╬";
				Sleep(WAIT);
			}
			cout << "╣";
		}
	}
	cct_gotoxy(2, Y_R0 + y_step * row - div);
	cout << "╚";
	for (int i = 0; i < line * (x_step / 2) - div; i++)
		if (!div || (i + 1) % (x_step / 2))
			cout << "═";
		else
			cout << "╩";
	cout << "╝";
	//==============================打印星星
	for (int r = 0; r < row; r++)
		for (int l = 0; l < line; l++) {
			int x = X_L0 + l * x_step;
			int y = Y_R0 + r * y_step;
			paint_star(x, y, star[r][l] + 8, COLOR_BLACK);
			Sleep(WAIT_STAR);
		}
	cct_setcolor();
	/*cct_gotoxy(0, Y_R0 + y_step * row + 1 - div);
	cout << "箭头键/鼠标移动，回车键/单击左键选择并结束";*/
	return;
}

/***************************************************************************
  函数名称：mouse_key
  功    能：鼠标/箭头移动
  输入参数：int * r, int * l, int row, int line, int div
  返 回 值：1:鼠标/箭头移开
            2:按下左键/回车
			3:按下右键/Q键
  说    明：
***************************************************************************/
int mouse_key(const int star[][LINE], int * r, int * l, int row, int line, int div)
{
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	const int end_row = Y_R0 + y_step * row + 1 - div;

	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;

	cct_enable_mouse();                 //打卡鼠标
	cct_setcursor(CURSOR_INVISIBLE);	//关闭光标

	int feedback = 0;
	while (!feedback) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT) {
			switch (maction) {
				case MOUSE_ONLY_MOVED:                  //鼠标移动
					if (Y < Y_R0 + *r * y_step || Y >= Y_R0 + *r * y_step + STAR_Y || X < X_L0 + *l * x_step || X >= X_L0 + *l * x_step + STAR_X)  //判断鼠标是否移开当前星星
						feedback = 1;
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					feedback = 2;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//按下右键
					feedback = 3;
					break;
				default:
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			switch (keycode1) {
				case 13:    //回车
				case 32:    //空格
					feedback = 2;
					break;
				case 'Q':
				case 'q':
				case 27:    //ESC
					feedback = 3;
					break;
				case 224:
					if (star[*r][*l])
					    paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_BLACK);
					feedback = 1;
					switch (keycode2) {
						case KB_ARROW_UP:            //上箭头
							do {
								*r = (*r - 1 + row) % row;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_DOWN:          //下箭头
							do {
								*r = (*r + 1) % row;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_LEFT:          //左箭头
							do {
								*l = (*l - 1 + line) % line;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_RIGHT:          //右箭头
							do {
								*l = (*l + 1) % line;
							} while (!star[*r][*l]);
							break;
					}
					break;
				default:
					break;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT）
	} //end of while(1)
    cct_disable_mouse();	//禁用鼠标
    cct_setcursor(CURSOR_VISIBLE_NORMAL);	//打开光标
	return feedback;
}

/***************************************************************************
  函数名称：choose_star
  功    能：鼠标/箭头选取星星
  输入参数：const int star[][LINE], int row, int line, int div
  返 回 值：无
  说    明：
***************************************************************************/
void choose_star(const int star[][LINE], int * r, int * l, int row, int line, int div)
{
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	const int end_row = Y_R0 + y_step * row + 1 - div;
	int vaild = 1;//表示星星的行列数，vaild表示是否在有效位置

	cct_setcolor();
	cct_gotoxy(0, end_row);
	cout << "箭头键/鼠标移动，回车键/单击左键选择并结束                                                  ";

	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;
	int loop = 1;
	//==========================================================================================
	//*r = *l = 0;
	while (!star[*r][*l])
		if (row - 1 == *r)
			(*l)--;
		else
		    (*r)++;
	/*if (star[*r][*l] && vaild)
		paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_HWHITE);*/
	//==========================================================================================
	cct_enable_mouse();                 //打卡鼠标
	cct_setcursor(CURSOR_INVISIBLE);	//关闭光标

	while (loop) {
		/* 读鼠标/键盘，返回值为下述操作中的某一种, 当前鼠标位置在<X,Y>处 */
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		vaild = 1;
		if (ret == CCT_MOUSE_EVENT) {
			/* 首先取消原位置反显 */
			if (star[*r][*l])
			    paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_BLACK);

			switch (maction) {
				case MOUSE_ONLY_MOVED:                  //鼠标移动
					if (Y < Y_R0 + *r * y_step || Y >= Y_R0 + *r * y_step + STAR_Y || X < X_L0 + *l * x_step || X >= X_L0 + *l * x_step + STAR_X) {  //判断鼠标是否移开当前星星
						vaild = 0;
						if (Y >= Y_R0 && Y < Y_R0 + row * y_step && X >= X_L0 && X < X_L0 + line * x_step
							&& (!div || ((X - X_L0) % x_step < STAR_X && (Y - Y_R0) % y_step < STAR_Y))) {                                           //判断鼠标当前位置知否有效
							vaild = 1;
							*l = (X - X_L0) / x_step;
							*r = (Y - Y_R0) / y_step;
							if (!star[*r][*l])
								vaild = 0;
						}
						cct_gotoxy(0, Y_R0 + y_step * row + 1 - div);
						if (vaild)
							cout << "[当前鼠标] " << char(*r + 'A') << "行" << *l << "列                                                            ";
						else
							cout << "[当前鼠标] 位置非法                                                                 ";
					}
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//按下左键
					loop = 0;
					break;
				default:                                //其他操作
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			switch (keycode1) {
				case 13:    //回车
				case 32:    //空格
					loop = 0;
					break;
				case 224:
					if (star[*r][*l])
					    paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_BLACK);
					switch (keycode2) {
						case KB_ARROW_UP:            //上箭头
							do {
								*r = (*r - 1 + row) % row;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_DOWN:          //下箭头
							do {
								*r = (*r + 1) % row;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_LEFT:          //左箭头
							do {
                                *l = (*l - 1 + line) % line;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_RIGHT:          //右箭头
							do {
								*l = (*l + 1) % line;
							} while (!star[*r][*l]);
							break;
					}
					break;
				default:	//其他键无反应
					break;
			}//end of swicth(keycode1)

			cct_gotoxy(0, end_row);
			cout << "[当前键盘] " << char(*r + 'A') << "行" << *l << "列                                              ";

		}//end of else if(ret == CCT_KEYBOARD_EVENT）

		if (vaild && star[*r][*l])
		    paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_HWHITE);

	} //end of while(1)
	cct_disable_mouse();	//禁用鼠标
	cct_setcursor(CURSOR_VISIBLE_NORMAL);	//打开光标
	cct_gotoxy(0, end_row);
	cout << "[已选择] " << char(*r + 'A') << "行" << *l << "列                                                          ";
	cct_gotoxy(0, end_row);
	return;
}

/***************************************************************************
  函数名称：choose_star_block
  功    能：鼠标/箭头选取星星
  输入参数：const int star[][LINE], int row, int line, int div
  返 回 值：无
  说    明：返回是否选择
***************************************************************************/
int choose_star_block(const int star[][LINE], char find[][LINE], int* r, int* l, int row, int line, int div)
{
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	const int end_row = Y_R0 + y_step * row + 1 - div;

	int chosen = 0, found = 0, feedback = 0;
	while (!chosen) {
		while (1) {
			feedback = 0;
			choose_star(star, r, l, row, line, div);
			if ((found = find_star(star, find, *r, *l)) - 1)
				break;
			else {
				find[*r][*l] = '\0';//没找到连续项时删除原位的‘*’
				cct_gotoxy(0, end_row);
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
				cout << "周围无相同值，";
				cct_setcolor();
				cout << "箭头键/鼠标移动，Q/ESC/单击右键结束                          ";
				while (!feedback || 2 == feedback)
					feedback = mouse_key(star, r, l, row, line, div);
				if (3 == feedback)
					break;
			}
		}
		if (3 == feedback)
			break;
		feedback = 0;
		for (int i = 0; i < row; i++)                                     //标注相邻星星
			for (int j = 0; j < line; j++) {
				if (i == *r && j == *l)
					continue;
				if (find[i][j])
					paint_star(X_L0 + j * x_step, Y_R0 + i * y_step, star[i][j] + 8, COLOR_WHITE);
			}
		cct_gotoxy(0, end_row);
		cout << "箭头键/鼠标移动取消当前选择，回车键/空格键/单击左键合成，Q/ESC/单击右键结束                 ";
		while (!feedback)
			feedback = mouse_key(star, r, l, row, line, div);
		if (1 == feedback)
			for (int i = 0; i < row; i++)                                  //取消标注
				for (int j = 0; j < line; j++)
					if (find[i][j]) {
						find[i][j] = '\0';
						paint_star(X_L0 + j * x_step, Y_R0 + i * y_step, star[i][j] + 8, COLOR_BLACK);
					}
		if (2 == feedback)
			chosen = 1;
		if (3 == feedback)
			break;
	}
	if (!chosen)
		found = 0;
	return found;
}

/***************************************************************************
  函数名称：drop_by_row
  功    能：星星从行间坠落，好似万家灯火
  输入参数：const int star[][LINE], int row, int line, int div
  返 回 值：无
  说    明：返回是否选择
***************************************************************************/
void drop_by_row(const int star[][LINE], int r, int l, int div)
{
	if (!star[r][l])
		return;
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	for (int move = 1; move <= y_step; move++) {
		paint_star(X_L0 + l * x_step, Y_R0 + r * y_step + move, star[r][l] + 8, COLOR_BLACK);
		cct_gotoxy(X_L0 + l * x_step, Y_R0 + r * y_step + move - 1);
		cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
		if (div && y_step == move)
			for (int i = 0; i < STAR_X / 2; i++)
				cout << "═";
		else
			cout << setfill(' ') << setw(STAR_X) << ' ';
		cct_setcolor();
		Sleep(WAIT_STAR);
	}
	return;
}

/***************************************************************************
  函数名称：drop_by_line
  功    能：星星从列间横贯，好似斗转星移
  输入参数：const int star[][LINE], int row, int line, int div
  返 回 值：无
  说    明：返回是否选择
***************************************************************************/
void drop_by_line(const int star[][LINE], int r, int l, int div)
{
	if (!star[r][l])
		return;
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	for (int move = 1; move <= x_step / 2; move++) {
		paint_star(X_L0 + l * x_step - 2 * move, Y_R0 + r * y_step, star[r][l] + 8, COLOR_BLACK);
		cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
		for (int i = 0; i < STAR_Y; i++) {
			cct_gotoxy(X_L0 + l * x_step - 2 * move + STAR_X, Y_R0 + r * y_step + i);
			if (div && x_step / 2 == move)
				cout << "║";
			else
				cout << "  ";
		}
		cct_setcolor();
		Sleep(WAIT_STAR);
	}
	return;
}

/***************************************************************************
  函数名称：paint_one_step
  功    能：伪图形完成一次消灭
  输入参数：const int star[][LINE], int row, int line, int div
  返 回 值：无
  说    明：返回该题的分数
***************************************************************************/
int paint_one_step(int star[][LINE], int row, int line, int div, int wait)
{
	int r = 0, l = 0, feedback = 0, score = 0;
	int tmp1 = 0, tmp2 = 0;//无意义，辅助mouse_key函数
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	const int end_row = Y_R0 + y_step * row + 1 - div;
	char find[ROW][LINE] = { '\0' };

	int found;
	if (-1 == wait)
		found = auto_choose_star_block(star, find, &r, &l, row, line, div);
	else
    	found = choose_star_block(star, find, &r, &l, row, line, div);
	if (!found)
		return 0;//直接结束
	delete_star(star, find, 1, div);
	if (wait > 0) {
		cct_gotoxy(0, end_row);
		cout << "合成完成，回车键/空格键/单击左键下落                                    ";
		while (2 != feedback)
			feedback = mouse_key(star, &tmp1, &tmp2, row, line, div);
	}
	fallen_star(star, find, row, line, 1, div);
	cct_gotoxy(0, 0);
	cout << "本次得分:" << (score = score_by(found)) << "                                    ";
	if (wait > 0) {	
		cct_gotoxy(0, end_row);
		cout << "本次合成结束，回车键/空格键/单击左键继续消灭星星                                               ";
		while (2 != feedback)
			feedback = mouse_key(star, &tmp1, &tmp2, row, line, div);
	}
	
	int num;
	if (num = check_star(star, row, line)) {
		num = (num > 0 ? num : 0);
		cct_setcolor(COLOR_HYELLOW, COLOR_RED);
		cct_gotoxy(0, end_row);
		cout << "剩余" << num << "个不可消除项，本关结束！(回车继续/Q结束)";
		cct_setcolor();
		cout << "                             ";
		cout << endl;
		score += score_in_end(num);
		return -score;
	}
	cct_gotoxy(0, end_row);
	return score;//返回本局分数
}

/***************************************************************************
  函数名称：paint_game
  功    能：伪图形消灭星星游戏
  输入参数：const int star[][LINE], int row, int line, int div
  返 回 值：无
  说    明：返回该题的分数
***************************************************************************/
void paint_game(int row, int line, int div, int wait)
{
	int loop = 1, high_score = 0;
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	const int end_row = Y_R0 + y_step * row + 1 - div;
	while (loop) {
		int score = 0, ds = 0;
		int star[ROW][LINE] = { '0' };
		ori_set_star(star, row, line);
		cct_cls();
		ori_paint(star, row, line, div);
		while (1) {
			ds = paint_one_step(star, row, line, div, wait);
			if (!ds) {
				loop = 0;
				break;
			}
			score += (ds > 0 ? ds : -ds);
			cct_gotoxy(0, 0);
			if (ds < 0) {
				cct_setcolor(COLOR_HYELLOW, COLOR_RED);
				if (high_score)
					cout << "之前最高分:" << high_score << "  ";
				high_score = (high_score > score) ? high_score : score;
			}
			cout << "本局得分:" << score;
			cct_setcolor();
			cout << "                                    ";
			if (ds < 0) {
				char input = _getch();
				while (input != '\r' && input != 'Q' && input !='q')
					input = _getch();
				if (input == 'Q' || input == 'q')
					loop = 0;
				break;
			}
		}
	}
	cct_gotoxy(0, end_row);
	return;
}