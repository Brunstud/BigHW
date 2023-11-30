/*2252201 ������ ��18 */
/*��cmdͼ�ν��淽ʽʵ�ֵĸ�����*/
#include<iostream>
#include<iomanip>
#include<conio.h>
#include<windows.h>
#include "../include/cmd_console_tools.h"
#include "../include/com_console.h"
#include"90-01-b2.h"
using namespace std;

/***************************************************************************
  �������ƣ�ori_paint
  ��    �ܣ����Ƴ�ʼ��ͼ
  ���������const int star[][LINE], int row, int line, int div
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void ori_paint(const int star[][LINE], int row, int line, int div)
{
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	cct_gotoxy(0, 0);
	if (div) {
		cct_setconsoleborder(x_step * line + 5, y_step * row + 7);
		cout << "��Ļ��ǰ����Ϊ��" << y_step * row + 7 << "��" << x_step * line + 5 << "��";
	}
	else {
		cct_setconsoleborder(x_step * line + 7, y_step * row + 8);
		cout << "��Ļ��ǰ����Ϊ��" << y_step * row + 8 << "��" << x_step * line + 7 << "��";
	}
	//==============================��ӡ�׵�
	cct_gotoxy(1 - div, 1);
	for (int i = 0; i < line; i++)
		cout << setfill(' ') << setw(x_step) << i;
	cct_gotoxy(2, 2);
	cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
	cout << "�X";
	for (int i = 0; i < line * (x_step / 2) - div; i++)
		if (!div || (i + 1) % (x_step / 2))
			cout << "�T";
		else
			cout << "�j";
	cout << "�[";
	for (int i = 0; i < y_step * row - div; i++) {
		if (!((i - 1) % y_step)) {
			cct_setcolor();
			cct_gotoxy(0, i + 3);
			cout << char((i - 1) / y_step + 'A');
			cct_setcolor(COLOR_HWHITE, COLOR_BLACK);
		}
		cct_gotoxy(2, i + Y_R0);
		if (!div || (i + 1) % y_step) {
			cout << "�U";
			for (int j = 0; j < line; j++) {
				cout << setw(x_step - 2) << ' ';
				if (j < line - 1 || !div)
					cout << (div ? "�U" : "  ");
				Sleep(WAIT);
			}
			cout << "�U";
		}
		else {
			cout << "�d";
			for (int j = 0; j < line; j++) {
				for (int k = 0; k < STAR_X / 2; k++)
				    cout << "�T";
				if (j < line - 1)
					cout << "�p";
				Sleep(WAIT);
			}
			cout << "�g";
		}
	}
	cct_gotoxy(2, Y_R0 + y_step * row - div);
	cout << "�^";
	for (int i = 0; i < line * (x_step / 2) - div; i++)
		if (!div || (i + 1) % (x_step / 2))
			cout << "�T";
		else
			cout << "�m";
	cout << "�a";
	//==============================��ӡ����
	for (int r = 0; r < row; r++)
		for (int l = 0; l < line; l++) {
			int x = X_L0 + l * x_step;
			int y = Y_R0 + r * y_step;
			paint_star(x, y, star[r][l] + 8, COLOR_BLACK);
			Sleep(WAIT_STAR);
		}
	cct_setcolor();
	/*cct_gotoxy(0, Y_R0 + y_step * row + 1 - div);
	cout << "��ͷ��/����ƶ����س���/�������ѡ�񲢽���";*/
	return;
}

/***************************************************************************
  �������ƣ�mouse_key
  ��    �ܣ����/��ͷ�ƶ�
  ���������int * r, int * l, int row, int line, int div
  �� �� ֵ��1:���/��ͷ�ƿ�
            2:�������/�س�
			3:�����Ҽ�/Q��
  ˵    ����
***************************************************************************/
int mouse_key(const int star[][LINE], int * r, int * l, int row, int line, int div)
{
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	const int end_row = Y_R0 + y_step * row + 1 - div;

	int X = 0, Y = 0;
	int ret, maction;
	int keycode1, keycode2;

	cct_enable_mouse();                 //�����
	cct_setcursor(CURSOR_INVISIBLE);	//�رչ��

	int feedback = 0;
	while (!feedback) {
		/* �����/���̣�����ֵΪ���������е�ĳһ��, ��ǰ���λ����<X,Y>�� */
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		if (ret == CCT_MOUSE_EVENT) {
			switch (maction) {
				case MOUSE_ONLY_MOVED:                  //����ƶ�
					if (Y < Y_R0 + *r * y_step || Y >= Y_R0 + *r * y_step + STAR_Y || X < X_L0 + *l * x_step || X >= X_L0 + *l * x_step + STAR_X)  //�ж�����Ƿ��ƿ���ǰ����
						feedback = 1;
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//�������
					feedback = 2;
					break;
				case MOUSE_RIGHT_BUTTON_CLICK:			//�����Ҽ�
					feedback = 3;
					break;
				default:
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			switch (keycode1) {
				case 13:    //�س�
				case 32:    //�ո�
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
						case KB_ARROW_UP:            //�ϼ�ͷ
							do {
								*r = (*r - 1 + row) % row;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_DOWN:          //�¼�ͷ
							do {
								*r = (*r + 1) % row;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_LEFT:          //���ͷ
							do {
								*l = (*l - 1 + line) % line;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_RIGHT:          //�Ҽ�ͷ
							do {
								*l = (*l + 1) % line;
							} while (!star[*r][*l]);
							break;
					}
					break;
				default:
					break;
			}//end of swicth(keycode1)
		}//end of else if(ret == CCT_KEYBOARD_EVENT��
	} //end of while(1)
    cct_disable_mouse();	//�������
    cct_setcursor(CURSOR_VISIBLE_NORMAL);	//�򿪹��
	return feedback;
}

/***************************************************************************
  �������ƣ�choose_star
  ��    �ܣ����/��ͷѡȡ����
  ���������const int star[][LINE], int row, int line, int div
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void choose_star(const int star[][LINE], int * r, int * l, int row, int line, int div)
{
	const int x_step = STAR_X + 2 * div;
	const int y_step = STAR_Y + div;
	const int end_row = Y_R0 + y_step * row + 1 - div;
	int vaild = 1;//��ʾ���ǵ���������vaild��ʾ�Ƿ�����Чλ��

	cct_setcolor();
	cct_gotoxy(0, end_row);
	cout << "��ͷ��/����ƶ����س���/�������ѡ�񲢽���                                                  ";

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
	cct_enable_mouse();                 //�����
	cct_setcursor(CURSOR_INVISIBLE);	//�رչ��

	while (loop) {
		/* �����/���̣�����ֵΪ���������е�ĳһ��, ��ǰ���λ����<X,Y>�� */
		ret = cct_read_keyboard_and_mouse(X, Y, maction, keycode1, keycode2);
		vaild = 1;
		if (ret == CCT_MOUSE_EVENT) {
			/* ����ȡ��ԭλ�÷��� */
			if (star[*r][*l])
			    paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_BLACK);

			switch (maction) {
				case MOUSE_ONLY_MOVED:                  //����ƶ�
					if (Y < Y_R0 + *r * y_step || Y >= Y_R0 + *r * y_step + STAR_Y || X < X_L0 + *l * x_step || X >= X_L0 + *l * x_step + STAR_X) {  //�ж�����Ƿ��ƿ���ǰ����
						vaild = 0;
						if (Y >= Y_R0 && Y < Y_R0 + row * y_step && X >= X_L0 && X < X_L0 + line * x_step
							&& (!div || ((X - X_L0) % x_step < STAR_X && (Y - Y_R0) % y_step < STAR_Y))) {                                           //�ж���굱ǰλ��֪����Ч
							vaild = 1;
							*l = (X - X_L0) / x_step;
							*r = (Y - Y_R0) / y_step;
							if (!star[*r][*l])
								vaild = 0;
						}
						cct_gotoxy(0, Y_R0 + y_step * row + 1 - div);
						if (vaild)
							cout << "[��ǰ���] " << char(*r + 'A') << "��" << *l << "��                                                            ";
						else
							cout << "[��ǰ���] λ�÷Ƿ�                                                                 ";
					}
					break;
				case MOUSE_LEFT_BUTTON_CLICK:			//�������
					loop = 0;
					break;
				default:                                //��������
					break;
			} //end of switch(maction)
		} //end of if (CCT_MOUSE_EVENT)
		else if (ret == CCT_KEYBOARD_EVENT) {
			switch (keycode1) {
				case 13:    //�س�
				case 32:    //�ո�
					loop = 0;
					break;
				case 224:
					if (star[*r][*l])
					    paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_BLACK);
					switch (keycode2) {
						case KB_ARROW_UP:            //�ϼ�ͷ
							do {
								*r = (*r - 1 + row) % row;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_DOWN:          //�¼�ͷ
							do {
								*r = (*r + 1) % row;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_LEFT:          //���ͷ
							do {
                                *l = (*l - 1 + line) % line;
							} while (!star[*r][*l]);
							break;
						case KB_ARROW_RIGHT:          //�Ҽ�ͷ
							do {
								*l = (*l + 1) % line;
							} while (!star[*r][*l]);
							break;
					}
					break;
				default:	//�������޷�Ӧ
					break;
			}//end of swicth(keycode1)

			cct_gotoxy(0, end_row);
			cout << "[��ǰ����] " << char(*r + 'A') << "��" << *l << "��                                              ";

		}//end of else if(ret == CCT_KEYBOARD_EVENT��

		if (vaild && star[*r][*l])
		    paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_HWHITE);

	} //end of while(1)
	cct_disable_mouse();	//�������
	cct_setcursor(CURSOR_VISIBLE_NORMAL);	//�򿪹��
	cct_gotoxy(0, end_row);
	cout << "[��ѡ��] " << char(*r + 'A') << "��" << *l << "��                                                          ";
	cct_gotoxy(0, end_row);
	return;
}

/***************************************************************************
  �������ƣ�choose_star_block
  ��    �ܣ����/��ͷѡȡ����
  ���������const int star[][LINE], int row, int line, int div
  �� �� ֵ����
  ˵    ���������Ƿ�ѡ��
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
				find[*r][*l] = '\0';//û�ҵ�������ʱɾ��ԭλ�ġ�*��
				cct_gotoxy(0, end_row);
				cct_setcolor(COLOR_BLACK, COLOR_HYELLOW);
				cout << "��Χ����ֵͬ��";
				cct_setcolor();
				cout << "��ͷ��/����ƶ���Q/ESC/�����Ҽ�����                          ";
				while (!feedback || 2 == feedback)
					feedback = mouse_key(star, r, l, row, line, div);
				if (3 == feedback)
					break;
			}
		}
		if (3 == feedback)
			break;
		feedback = 0;
		for (int i = 0; i < row; i++)                                     //��ע��������
			for (int j = 0; j < line; j++) {
				if (i == *r && j == *l)
					continue;
				if (find[i][j])
					paint_star(X_L0 + j * x_step, Y_R0 + i * y_step, star[i][j] + 8, COLOR_WHITE);
			}
		cct_gotoxy(0, end_row);
		cout << "��ͷ��/����ƶ�ȡ����ǰѡ�񣬻س���/�ո��/��������ϳɣ�Q/ESC/�����Ҽ�����                 ";
		while (!feedback)
			feedback = mouse_key(star, r, l, row, line, div);
		if (1 == feedback)
			for (int i = 0; i < row; i++)                                  //ȡ����ע
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
  �������ƣ�drop_by_row
  ��    �ܣ����Ǵ��м�׹�䣬������ҵƻ�
  ���������const int star[][LINE], int row, int line, int div
  �� �� ֵ����
  ˵    ���������Ƿ�ѡ��
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
				cout << "�T";
		else
			cout << setfill(' ') << setw(STAR_X) << ' ';
		cct_setcolor();
		Sleep(WAIT_STAR);
	}
	return;
}

/***************************************************************************
  �������ƣ�drop_by_line
  ��    �ܣ����Ǵ��м��ᣬ���ƶ�ת����
  ���������const int star[][LINE], int row, int line, int div
  �� �� ֵ����
  ˵    ���������Ƿ�ѡ��
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
				cout << "�U";
			else
				cout << "  ";
		}
		cct_setcolor();
		Sleep(WAIT_STAR);
	}
	return;
}

/***************************************************************************
  �������ƣ�paint_one_step
  ��    �ܣ�αͼ�����һ������
  ���������const int star[][LINE], int row, int line, int div
  �� �� ֵ����
  ˵    �������ظ���ķ���
***************************************************************************/
int paint_one_step(int star[][LINE], int row, int line, int div, int wait)
{
	int r = 0, l = 0, feedback = 0, score = 0;
	int tmp1 = 0, tmp2 = 0;//�����壬����mouse_key����
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
		return 0;//ֱ�ӽ���
	delete_star(star, find, 1, div);
	if (wait > 0) {
		cct_gotoxy(0, end_row);
		cout << "�ϳ���ɣ��س���/�ո��/�����������                                    ";
		while (2 != feedback)
			feedback = mouse_key(star, &tmp1, &tmp2, row, line, div);
	}
	fallen_star(star, find, row, line, 1, div);
	cct_gotoxy(0, 0);
	cout << "���ε÷�:" << (score = score_by(found)) << "                                    ";
	if (wait > 0) {	
		cct_gotoxy(0, end_row);
		cout << "���κϳɽ������س���/�ո��/�������������������                                               ";
		while (2 != feedback)
			feedback = mouse_key(star, &tmp1, &tmp2, row, line, div);
	}
	
	int num;
	if (num = check_star(star, row, line)) {
		num = (num > 0 ? num : 0);
		cct_setcolor(COLOR_HYELLOW, COLOR_RED);
		cct_gotoxy(0, end_row);
		cout << "ʣ��" << num << "��������������ؽ�����(�س�����/Q����)";
		cct_setcolor();
		cout << "                             ";
		cout << endl;
		score += score_in_end(num);
		return -score;
	}
	cct_gotoxy(0, end_row);
	return score;//���ر��ַ���
}

/***************************************************************************
  �������ƣ�paint_game
  ��    �ܣ�αͼ������������Ϸ
  ���������const int star[][LINE], int row, int line, int div
  �� �� ֵ����
  ˵    �������ظ���ķ���
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
					cout << "֮ǰ��߷�:" << high_score << "  ";
				high_score = (high_score > score) ? high_score : score;
			}
			cout << "���ֵ÷�:" << score;
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