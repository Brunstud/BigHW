/*2252201 ������ ��18*/
/*��һЩ�ڲ�����/ͼ�η�ʽ���õĺ��������жϽ�����*/
#include<iostream>
#include<iomanip>
#include<conio.h>
#include "../include/cmd_console_tools.h"
#include "../include/com_console.h"
#include"90-01-b2.h"
using namespace std;

/***************************************************************************
  �������ƣ�operate_by_choice
  ��    �ܣ����ݲ˵����ص�choiceִ�и������
  ���������choice
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void operate_by_choice(int star[][LINE], int row, int line, char choice)
{
	int r = 0, l = 0, score = 0;
	char find[ROW][LINE] = { '\0' };
	switch (choice) {
		case 'A':
			cct_setconsoletitle("�������ǡ������������");
			question1(star, find, &r, &l, row, line);
			break;
		case 'B':
			cct_setconsoletitle("�������ǡ��������м���");
			question2(star, &r, &l, row, line, score);
			break;
		case 'C':
			cct_setconsoletitle("�������ǡ�������������");
			question3(star, &r, &l, row, line, score);
			break;
		case 'D':
		case 'E':
			cct_setconsoletitle("�������ǡ�����ӡ����");
			cct_cls();
			ori_paint(star, row, line, choice - 'D');
			choose_star(star, &r, &l, row, line, choice - 'D');
			break;
		case 'F':
			cct_setconsoletitle("�������ǡ�����һ��");
			cct_cls();
			ori_paint(star, row, line, 0);
			paint_one_step(star, row, line, 0, 1);
			break;
		case 'G':
			cct_setconsoletitle("�������ǡ�����Ϸ��");
			paint_game(row, line, 1);
			break;
		case 'H':
			cct_setconsoletitle("�������ǡ����Զ���");
			paint_game(row, line, 1, -1);
			break;
		default:
			cout << "choice_error!";
	}
	return;
}

/***************************************************************************
  �������ƣ�find_star
  ��    �ܣ�����ָ������λ�ü���Χ��ͬ��ĺ���(Ĭ�ϸô���ֵ)
  ���������const int star[][LINE], char find[][LINE], int r, int l
  �� �� ֵ��int found
  ˵    ���������Ƿ���������ֵͨ
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
  �������ƣ�delete_star
  ��    �ܣ�ɾ����Ӧλ�õ�����
  ���������int star[][LINE], const char find[][LINE]
  �� �� ֵ����
  ˵    ����
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
  �������ƣ�fallen_star
  ��    �ܣ������������ƶ�
  ���������int star[][LINE], char find[][LINE]
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void fallen_star(int star[][LINE], const char find[][LINE], int row, int line, int paint, int div)
{
	//===============================================================����
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
	//==============================================================����
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
  �������ƣ�check_star
  ��    �ܣ�����Ƿ���������
  ���������int star[][LINE]
  �� �� ֵ��int
  ˵    ���������������򷵻�0�����޿�������򷵻�ʣ�����
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
  �������ƣ�auto_star
  ��    ��: �ս���
  ���������
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void auto_star(int row, int line, int div)
{
    
	return;
}

/***************************************************************************
  �������ƣ�auto_choose_star_block
  ��    ��: �ս���
  ���������
  �� �� ֵ��
  ˵    ��������found
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
	//===========================================================��ʼ����
	for (int ro = row - 1; ro >= 0; ro--)
		for (int ln = 0; ln < line; ln++) {
			if (star[ro][ln] && !occupy[ro][ln]) {
				if (find_star(star, occupy, ro, ln) == 1)
					continue;
				//===================================����׼��
				int auto_star[ROW][LINE] = { 0 };
				for (int i = 0; i < row; i++)
					for (int j = 0; j < line; j++)
						if (star[i][j])
							auto_star[i][j] = star[i][j];
				char auto_find[ROW][LINE] = { '\0' };
				//====================================����׼��
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
	cout << "�Զ�������ѽⷨ�У������ĵȴ�������";
	cct_gotoxy(0, end_row + 1);
	cout << "���ȣ�" << "  0" << "/" << setw(3) << row * line;

	int score = 0, tmp1 = 0, tmp2 = 0, h_nex_row = 0, h_nex_line = 0;
	int h_row = row - 1, h_line = 0, h_score = 0;
	char occupy[ROW][LINE] = { '\0' };
	//===========================================================��ʼ����
	for (int ro = row - 1; ro >= 0; ro--) {
		for (int ln = 0; ln < line; ln++) {
			if (star[ro][ln] && !occupy[ro][ln]) {
				if (find_star(star, occupy, ro, ln) == 1)
					continue;
				//===================================����׼��
				int auto_star[ROW][LINE] = { 0 };
				for (int i = 0; i < row; i++)
					for (int j = 0; j < line; j++)
						if (star[i][j])
							auto_star[i][j] = star[i][j];
				char auto_find[ROW][LINE] = { '\0' };
				//====================================����׼��
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
				cout << "���ȣ�" << setfill(' ') << setw(3) << (row - ro - 1) * line + ln + 1 << "/" << setw(3) << row * line;//������
				//cct_gotoxy(0, 0);
				//show_array(NULL, star, occupy, row, line, -1);
				//cout << "��ǰ��߷֣�" << setw(5) << h_score << " �ôε����÷֣�" << setw(5) << score;
				//cout << "��һ��" << char(tmp1 + 'A') << tmp2;
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
		cout << "��һ��:   END";
	else
	    cout << "��һ��:    " << char(h_nex_row + 'A') << h_nex_line;
	//=================================================================================================
	*r = h_row;
	*l = h_line;
	int found = find_star(star, find, *r, *l);
	paint_star(X_L0 + *l * x_step, Y_R0 + *r * y_step, star[*r][*l] + 8, COLOR_HWHITE);
	for (int i = 0; i < row; i++)                                     //��ע��������
		for (int j = 0; j < line; j++) {
			if (i == *r && j == *l)
				continue;
			if (find[i][j])
				paint_star(X_L0 + j * x_step, Y_R0 + i * y_step, star[i][j] + 8, COLOR_WHITE);
		}
	cct_gotoxy(0, end_row);
	cout << "�ò������£����س�������               ";
	while (_getch() != '\r')
		continue;
	for (int i = 0; i < row; i++)                                  //ȡ����ע
		for (int j = 0; j < line; j++)
			if (find[i][j])
				paint_star(X_L0 + j * x_step, Y_R0 + i * y_step, star[i][j] + 8, COLOR_BLACK);
	return found;
}