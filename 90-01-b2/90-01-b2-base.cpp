/*2252201 ������ ��18 */
/*���ڲ����鷽ʽʵ�ֵĸ�����*/
#include<iostream>
#include<conio.h>
#include<iomanip>
#include "../include/cmd_console_tools.h"
#include"90-01-b2.h"
using namespace std;

/***************************************************************************
  �������ƣ�ori_set_star
  ��    �ܣ���ʼ��������star
  ���������int star[][LINE], int row, int line
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void ori_set_star(int star[][LINE], int row, int line)
{
	for (int i = 0; i < row; i++)
		for (int j = 0; j < line; j++)
			star[i][j] = rand() % STAR + 1;
	return;
}

/***************************************************************************
  �������ƣ�show_array
  ��    �ܣ���ʵ��ǰ����star��find
  ���������const int star[][LINE], const char find[][LINE], int row, int line, int sign
  �� �� ֵ����
  ˵    ����sign==0ʱ"��ǰ����"��sign>0ʱ"��ǰ���飨��ͬɫ��ʶ��"��sign<0ʱ"���ҽ������"
***************************************************************************/
void show_array(const char * intro, const int star[][LINE], const char find[][LINE], int row, int line, int sign)
{
    if (intro)
	    cout << intro << endl;
	cout << "  |";
	for (int i = 0; i < line; i++)
		cout << setfill(' ') << setw(3) << i;
	cout << endl;
	cout << "--+";
	cout << setfill('-') << setw(line * 3 + 1) << '-' << endl;
	for (int i = 0; i < row; i++) {
		cout << char(i + 'A') << ' ' << '|';
		for (int j = 0; j < line; j++) {
			cout << "  ";
			if (sign < 0)
				if (find[i][j])
					cout << '*';
				else
					cout << '0';
			else {
				if (sign && find[i][j] || !sign && !star[i][j])
				    cct_setcolor(COLOR_HYELLOW, COLOR_BLACK);
				cout << star[i][j];
				if (sign && find[i][j] || !sign && !star[i][j])
					cct_setcolor();
			}
		}
		cout << endl;
	}
	return;
}

/***************************************************************************
  �������ƣ�input_step
  ��    �ܣ����벽��
  ���������int star[][LINE], char find[][LINE], int row, int line
  �� �� ֵ��int
  ˵    ���������ҵ���������
***************************************************************************/
int input_step(int star[][LINE], char find[][LINE], int * r, int * l, int row, int line)
{
	int found = 0;
	while (1) {
		char r_char;
		int x, y;
		cout << "������ĸ+������ʽ[����c2]����������꣺";
		cct_getxy(x, y);
		while (1) {
			cin >> r_char >> *l;
			if (r_char >= 'a' && r_char <= 'a' + row - 1)
				r_char = r_char - 'a' + 'A';
			if (!cin || r_char < 'A' || r_char > 'A' + row - 1 || *l < 0 || *l > line - 1) {
				cct_gotoxy(x, y);
				cout << "                         ";
				cct_gotoxy(0, y + 1);
				cout << "�����������������.";
				cct_gotoxy(x, y);
				if (!cin)
					cin.clear();
			    while ('\n' != getchar())
					continue;
				continue;
			}
			break;
		}
		cout << "����Ϊ" << r_char << "��" << *l << "��         " << endl;
		*r = r_char - 'A';
		if (!star[*r][*l]) {
			cout << "����ľ�������λ��Ϊ0���Ƿ�λ�ã�������������" << endl;
			continue;
		}
		if ((found = find_star(star, find, *r, *l)) - 1)
			break;
		else {
			find[*r][*l] = '\0';//û�ҵ�������ʱɾ��ԭλ�ġ�*��
			cout << "����ľ�������λ�ô���������ֵͬ������������" << endl;
		}
	}
	return found;
}

/***************************************************************************
  �������ƣ�question1
  ��    �ܣ������һС��
  ���������int star[][LINE], char find[][LINE], int row, int line
  �� �� ֵ��int
  ˵    ���������ҵ���������
***************************************************************************/
int question1(int star[][LINE], char find[][LINE], int * r, int * l, int row, int line)
{
	int found = 0, num;
	cout << endl;
	show_array("��ǰ����:", star, find, row, line, 0);
	cout << endl;
	if (num = check_star(star, row, line)) {
		cct_setcolor(COLOR_HYELLOW, COLOR_RED);
		cout << "ʣ��" << (num > 0 ? num : 0) << "��������������ؽ�����";
		cout << endl;
		return 0;
	}
	found = input_step(star, find, r, l, row, line);
	cout << endl;
	show_array("���ҽ������:", star, find, row, line, -1);
	cout << endl;
	show_array("��ǰ����(��ͬɫ��ʶ):", star, find, row, line, 1);
	cout << endl;
	return found;
}

/***************************************************************************
  �������ƣ�question2
  ��    �ܣ�����ڶ�С��
  ���������int star[][LINE], char find[][LINE], int row, int line
  �� �� ֵ��int
  ˵    ���������Ƿ���������������Ϊ0��Ϊ������
***************************************************************************/
int question2(int star[][LINE], int* r, int* l, int row, int line, int score)
{
	while (1) {
		char find[ROW][LINE] = { '\0' };
		int found = question1(star, find, r, l, row, line);
		if (!found) {
			int num = check_star(star, row, line);
			score += score_in_end(num > 0 ? num : 0);
			cout << "�����" << score << "��." << endl;
			cct_setcolor();
			score = 0;
			break;
		}
		cout << "��ȷ���Ƿ��" << char(*r + 'A') << *l << "����Χ����ֵͬ����(Y/N/Q)��";
		int x, y;
		char answer;
		cct_getxy(x, y);
		do {
			cct_gotoxy(x, y);
			answer = _getch();
			cout << answer;
		} while (answer != 'Y' && answer != 'N' && answer != 'Q' && answer != 'y' && answer != 'n' && answer != 'q');
		if ('Y' == answer || 'y' == answer) {
			cout << endl << endl;
			delete_star(star, find);
			show_array("��ֵͬ�鲢�������(��ͬɫ��ʶ)��", star, find, row, line, 1);
			cout << endl;
			cout << "��������" << found << "������" << endl;
			cout << "���ε÷֣�" << score_by(found) << " �ܵ÷֣�" << (score += score_by(found)) << endl;
			cout << endl;
			cout << "���س������������������...";
			while (_getch() != '\r')
				continue;
			cout << endl;
			fallen_star(star, find, row, line);
			show_array("��������飺", star, find, row, line, 0);
			cout << endl;
			break;
		}
		if ('Q' == answer || 'q' == answer) {
			score = 0;
			cout << endl;
			break;
		}
		if ('N' == answer || 'n' == answer) {
			cout << endl;
			continue;
		}
	}
	return score;
}

/***************************************************************************
  �������ƣ�question3
  ��    �ܣ��������С��
  ���������int star[][LINE], char find[][LINE], int row, int line
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void question3(int star[][LINE], int* r, int* l, int row, int line, int score)
{
	while (1) {
		score = question2(star, r, l, row, line, score);
		if (!score)
			break;
		cout << "�����������������س���������һ�ֵ�����...";
		while (_getch() != '\r')
			continue;
		cout << endl;
	}
	return;
}