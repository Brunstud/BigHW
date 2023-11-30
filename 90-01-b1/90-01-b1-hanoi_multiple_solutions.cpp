/* 2252201 ������ ��18 */
#include<iostream>
#include<iomanip>
#include <conio.h>
#include<windows.h>
#include "../include/cmd_console_tools.h"
#include"90-01-b1-hanoi.h"
using namespace std;

/* ----------------------------------------------------------------------------------

     ���ļ����ܣ�
	1����ű� hanoi_main.cpp �и��ݲ˵�����ֵ���õĸ��˵����Ӧ��ִ�к���

     ���ļ�Ҫ��
	1�����������ⲿȫ�ֱ�����const��#define�������Ʒ�Χ�ڣ�
	2�������徲̬ȫ�ֱ�����������Ҫ��������Ҫ�����ĵ���ʾ��ȫ�ֱ�����ʹ��׼���ǣ����á����á��ܲ��þ������ã�
	3����̬�ֲ����������������ƣ���ʹ��׼��Ҳ�ǣ����á����á��ܲ��þ�������
	4���������ϵͳͷ�ļ����Զ���ͷ�ļ��������ռ��

   ----------------------------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�input
  ��    �ܣ�����n, src, tmp, dst, speed
  ���������char * src, char * tmp, char * dst
            int speed : �Ƿ�����speed�������򷵻�speed��ֵ�����򷵻�0
  �� �� ֵ������n
  ˵    ����
***************************************************************************/
int input(int * n, char * src, char * tmp, char * dst, int speed)
{
    cct_gotoxy(0, 15);
    do {                                    //�������n
        cin.clear();
        cout << "�����뺺ŵ���Ĳ���(1-10)" << endl;
        cin >> *n;
        while (getchar() != '\n')
            continue;
    } while (!cin || *n < 1 || *n > Layers);
    do {                                    //������ʼ��src
        cin.clear();
        cout << "��������ʼ��(A-C)" << endl;
        cin >> *src;
        if (*src >= 'a' && *src <= 'c')
            *src = *src - 'a' + 'A';
        while (getchar() != '\n')
            continue;
    } while (!cin || *src < 'A' || *src > 'C');
    do {                                    //����Ŀ����dst
        cin.clear();
        cout << "������Ŀ����(A-C)" << endl;
        cin >> *dst;
        if (*dst >= 'a' && *dst <= 'c')
            *dst = *dst - 'a' + 'A';
        if (*dst == *src)
            cout << "Ŀ����(" << *dst << ")��������ʼ��(" << *src << ")��ͬ" << endl;
        while (getchar() != '\n')
            continue;
    } while (!cin || *dst < 'A' || *dst > 'C' || *dst == *src);
    *tmp = 'A' + 'B' + 'C' - *src - *dst;          //�����м���tmp
    if (speed)
        do {                                    //�����ƶ��ٶ�speed
            cin.clear();
            cout << "�������ƶ��ٶ�(0-5: 0-���س�������ʾ 1-��ʱ� 5-��ʱ���)";
            cin >> speed;
            while (getchar() != '\n')
                continue;
        } while (!cin || speed < 0 || speed > 5);
    return speed;
}

/***************************************************************************
  �������ƣ�ori_set_stack
  ��    �ܣ����ó�ʼstack��top
  ���������int stack[][Layers], int top[], int n, char src
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void ori_set_stack(int stack[][Layers], int top[], int n, char src)
{
    switch (src) {                             //���ó�ʼstack��top
        case 'A':
            for (int i = 0; i < n; i++)
                stack[0][i] = n - i;
            top[0] = n;
            break;
        case 'B':
            for (int i = 0; i < n; i++)
                stack[1][i] = n - i;
            top[1] = n;
            break;
        case 'C':
            for (int i = 0; i < n; i++)
                stack[2][i] = n - i;
            top[2] = n;
            break;
    }
    return;
}

/***************************************************************************
  �������ƣ�start
  ��    �ܣ���ʼ����
  ���������int stack[][Layers], int top[], int n, char src, char dst, int speed, int choice
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void start(int stack[][Layers], int top[], int n, char src, char dst, int speed, int choice)
{
    if (choice < 4)
        return;
    cct_cls();
    cct_gotoxy(0,0);
    cout << "��" << setw(2) << src << " �ƶ���" << setw(2) << dst << "����" << setw(2) << n << " ��";
    if (4 == choice || 8 == choice)
        cout << "����ʱ����Ϊ" << setw(2) << speed;
    if (4 == choice) {
        show_tower(stack, n, src, dst, 0, 16);
        if (speed > 0)
            Sleep(1000 / speed / speed / speed);
        else if (!speed)
            while (_getch() != '\r')
                continue;
    }
    if (choice >= 6) {
        print_base();
        print_load(stack);
    }
    if (7 == choice) {
        first_move(stack, top, n, src, dst);
    }
    if (8 == choice) {
        show_tower(stack, n, src, dst, 0, 30);
        if (speed > 0)
            Sleep(1000 / speed / speed / speed);
        else if (!speed)
            while (_getch() != '\r')
                continue;
    }
    if (9 == choice) {
        show_tower(stack, n, src, dst, 0, 30);
    }
    return;
}

/***************************************************************************
  �������ƣ�hanoi_game
  ��    �ܣ���ŵ����Ϸ
  ���������int stack[][Layers], int top[], int n, char src, char dst
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void hanoi_game(int stack[][Layers], int top[], int n, char src, char dst)
{
    int end = 0;
    char s, d;
    int N = 0;//�Ʋ���
    cct_gotoxy(0, 36);
    cout << "�������ƶ�������(������ʽ��AC = A���˵������ƶ���C��Q = �˳�) ��";
    while (1) {
        while (1) {
            s = _getch();
            cout << s;
            if ('Q' == s || 'q' == s) {
                end = 1;
                break;
            }
            if (s >= 'a' && s <= 'c')
                s = s - 'a' + 'A';
            if (s < 'A' || s > 'C') {
                cct_gotoxy(64, 36);
                cout << "  ";
                cct_gotoxy(64, 36);
                continue;
            }
            d = _getch();
            cout << d;
            if (d >= 'a' && d <= 'c')
                d = d - 'a' + 'A';
            if (d < 'A' || d > 'C' || d == s) {
                cct_gotoxy(64, 36);
                cout << "  ";
                cct_gotoxy(64, 36);
                continue;
            }
            if (stack[d - 'A'][0] && stack[d - 'A'][top[d - 'A'] - 1] < stack[s - 'A'][top[s - 'A'] - 1] || !stack[s - 'A'][top[s - 'A'] - 1]) {
                cct_gotoxy(0, 37);
                if (!stack[s - 'A'][top[s - 'A'] - 1])
                    cout << "Դ��Ϊ�գ�";
                else
                    cout << "����ѹС�̣��Ƿ��ƶ���";
                Sleep(1000);
                cct_gotoxy(0, 37);
                cout << "                      ";
                cct_gotoxy(64, 36);
                cout << "  ";
                cct_gotoxy(64, 36);
                continue;
            }
            break;
        }
        if (end)
            break;
        stack_operation(stack, top, s, d);
        move_block(top, stack[d - 'A'][top[d - 'A'] - 1], s, d, 3);
        show_tower(stack, n, src, dst, 0, 30);
        N++;
        if (n == stack[dst - 'A'][0] && 0 == stack[src - 'A'][0] && 0 == stack['B' + 'C' - src - dst][0])
            break;
        cct_gotoxy(64, 36);
        cout << "  ";
        cct_gotoxy(64, 36);
    }
    if (end) {
        cct_gotoxy(0, 37);
        cout << "��Ϸ��ֹ��"<< endl << endl;
    }
    else {
        cct_gotoxy(0, 37);
        cout << "��Ϸʤ��������" << N << "��" << endl << endl;
    }
    return;
}

/***************************************************************************
  �������ƣ�stack_operation
  ��    �ܣ����������������
  ���������int stack[][Layers], int top[], char src, char dst
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void stack_operation(int stack[][Layers], int top[], char src, char dst)
{
    switch (src) {                             //����stack
        case 'A':
            if ('B' == dst) {
                stack[1][top[1]++] = stack[0][--top[0]];
                stack[0][top[0]] = 0;
            }
            else {
                stack[2][top[2]++] = stack[0][--top[0]];
                stack[0][top[0]] = 0;
            }
            break;
        case 'B':
            if ('C' == dst) {
                stack[2][top[2]++] = stack[1][--top[1]];
                stack[1][top[1]] = 0;
            }
            else {
                stack[0][top[0]++] = stack[1][--top[1]];
                stack[1][top[1]] = 0;
            }
            break;
        case 'C':
            if ('A' == dst) {
                stack[0][top[0]++] = stack[2][--top[2]];
                stack[2][top[2]] = 0;
            }
            else {
                stack[1][top[1]++] = stack[2][--top[2]];
                stack[2][top[2]] = 0;
            }
            break;
    }
}
/***************************************************************************
  �������ƣ�hanoi_operation
  ��    �ܣ���ŵ���ݹ麯�����ò�������
  ���������int stack[][Layers], int top[], int n, char src, char dst, int N, int speed, int choice
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hanoi_operation(int stack[][Layers], int top[], int n, char src, char dst, int N, int speed, int choice)
{
    stack_operation(stack, top, src, dst);
    if (1 == choice) {
        cout << n << "# " << src << "---->" << dst << endl;
    }
    if (2 == choice) {
        cout << "��" << setw(4) << N << " ��(" << setw(2) << n << "#: " << src << "-->" << dst << ")" << endl;
    }
    if (3 == choice) {
        show_array(stack, n, src, dst, N);
    }
    if (4 == choice) {
        show_tower(stack, n, src, dst, N, 16);
    }
    if (8 == choice) {
        show_tower(stack, n, src, dst, N, 30);
        move_block(top, n, src, dst, speed ? speed : 2);
    }
    if (speed > 0)
        Sleep(1000 / speed / speed / speed);
    else if (!speed)
        while (_getch() != '\r')
            continue;
    return;
}

/***************************************************************************
  �������ƣ�hanoi
  ��    �ܣ���ŵ���ݹ麯��
  ���������int stack[][Layers], int top[]����������
            int n������
            char src����ʼ��
            char tmp���м���
            char dst��Ŀ����
            int N   : ��������ʼ����1
            int speed  : �ٶ�/��ʱ
            int choice : ѡ��
  �� �� ֵ��������N
  ˵    ����
***************************************************************************/
int hanoi(int stack[][Layers], int top[], int n, char src, char tmp, char dst, int N, int speed, int choice)
{
    if (n != 1)
        N = hanoi(stack, top, n - 1, src, dst, tmp, N, speed, choice);
    hanoi_operation(stack, top, n, src, dst, N, speed, choice);
    N++;
    if (n != 1)
        N = hanoi(stack, top, n - 1, tmp, src, dst, N, speed, choice);
    return N;
}

/***************************************************************************
  �������ƣ�show_array
  ��    �ܣ���ʾ�ڲ�����
  ���������int stack[][Layers], int n, char src, char dst, int N
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void show_array(int stack[][Layers], int n, char src, char dst, int N)
{
    if (N) {
        cout << "��" << setw(4) << N << " ��(" << setw(2) << n << "#: " << src << "-->" << dst << ") ";
    }
    else
        cout << "��ʼ: ";
    cout << " A:";
    for (int i = 0; i < Layers; i++) {
        if (stack[0][i])
            cout << setw(2) << stack[0][i];
        else
            cout << "  ";
    }
    cout << " B:";
    for (int i = 0; i < Layers; i++) {
        if (stack[1][i])
            cout << setw(2) << stack[1][i];
        else
            cout << "  ";
    }
    cout << " C:";
    for (int i = 0; i < Layers; i++) {
        if (stack[2][i])
            cout << setw(2) << stack[2][i];
        else
            cout << "  ";
    }
    cout << endl;
    return;
}

/***************************************************************************
  �������ƣ�show_tower
  ��    �ܣ���ʾ��ŵ��
  ���������int stack[][Layers], int n, char src, char dst, int N, const int Y
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void show_tower(int stack[][Layers], int n, char src, char dst, int N, const int Y)
{
    int X = 12;
    int gap = 9;//ABC֮����
    cct_gotoxy(X, Y);//��ӡ��ŵ��
    cout << "  A";
    cout << setw(gap) << ' ';
    cout << "B";
    cout << setw(gap) << ' ';
    cout << "C  ";
    cct_gotoxy(X, Y - 1);
    for (int i = 0; i < 2 * gap + 7; i++)
        cout << "=";
    for (int i = 0; i < Layers; i++) {
        cct_gotoxy(X + 1, Y - i - 2);
        if (stack[0][i])
            cout << setw(2) << stack[0][i];
        else
            cout << "  ";
    }
    for (int i = 0; i < Layers; i++) {
        cct_gotoxy(X + gap + 2, Y - i - 2);
        if (stack[1][i])
            cout << setw(2) << stack[1][i];
        else
            cout << "  ";
    }
    for (int i = 0; i < Layers; i++) {
        cct_gotoxy(X + gap * 2 + 3, Y - i - 2);
        if (stack[2][i])
            cout << setw(2) << stack[2][i];
        else
            cout << "  ";
    }
    cct_gotoxy(0, Y + 4);
    show_array(stack, n, src, dst, N);
    return;
}

/***************************************************************************
  �������ƣ�print_base()
  ��    �ܣ���ӡ��������
  �����������
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void print_base()
{
    cct_showch(X_A - Layers - 1, Y_base, ' ', COLOR_HYELLOW, COLOR_HYELLOW, 2 * Layers + 3);
    cct_showch(X_B - Layers - 1, Y_base, ' ', COLOR_HYELLOW, COLOR_HYELLOW, 2 * Layers + 3);
    cct_showch(X_C - Layers - 1, Y_base, ' ', COLOR_HYELLOW, COLOR_HYELLOW, 2 * Layers + 3);
    int X_abc[] = { X_A, X_B, X_C };
    for (int i = 1; i <= Layers + 2; i++)
        for (int j = 0; j < 3; j++) {
            cct_showch(X_abc[j], Y_base - i, ' ', COLOR_HYELLOW, COLOR_HYELLOW, 1);
            Sleep(33);
        }
    cct_gotoxy(0, 32);
    cct_setcolor();
    return;
}

/***************************************************************************
  �������ƣ�print_load
  ��    �ܣ���ʼ�����¼�װ���
  ���������int stack[][Layers]
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void print_load(int stack[][Layers])
{
    int X_abc[] = { X_A, X_B, X_C };
    for (int i = 0; i < 3; i++)
        if (stack[i][0])
            for (int j = 0; j < 10; j++)
                if (stack[i][j]) {
                    cct_showch(X_abc[i] - stack[i][j], Y_base - j - 1, ' ', stack[i][j], stack[i][j], 2 * stack[i][j] + 1);
                    Sleep(100);
                }
    cct_gotoxy(0, 32);
    cct_setcolor();
    return;
}

/***************************************************************************
  �������ƣ�first_move
  ��    �ܣ�ֻ�ƶ���һ��
  ���������int stack[][Layers], int top[], int n, int src, int dst
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void first_move(int stack[][Layers], int top[], int n, int src, int dst)
{
    if (!(n % 2))
        dst = 'A' + 'B' + 'C' - src - dst;
    top[src - 'A']--;
    top[dst - 'A']++;
    move_block(top, 1, src, dst, 1);
    cct_gotoxy(0, 32);
    cct_setcolor();
    return;
}

/***************************************************************************
  �������ƣ�move_block
  ��    �ܣ��ƶ�ģ��
  ���������int top[], int n, int src, int dst, int speed
  �� �� ֵ����
  ˵    ������Ƕ��print_block��delet_block�������ڱ�������ʹ�õĺ���
***************************************************************************/
void print_block(const int X, const int Y, const int n);
void delet_block(const int X, const int Y, const int n);
void move_block(int top[], int n, int src, int dst, int speed)
{
    int X_abc[] = { X_A, X_B, X_C };
    int x_src = X_abc[src - 'A'];
    int x_dst = X_abc[dst - 'A'];
    int y_src = Y_base - 1 - top[src - 'A'];
    int y_dst = Y_base - top[dst - 'A'];
    for (int y = y_src; y > Y_fly; y--) {
        delet_block(x_src, y, n);
        print_block(x_src, y - 1, n);
        Sleep(100 / speed / speed / speed);
    }
    if (dst > src)
        for (int x = x_src; x < x_dst; x++) {
            delet_block(x, Y_fly, n);
            print_block(x + 1, Y_fly, n);
            Sleep(50 / speed / speed / speed);
        }
    else
        for (int x = x_src; x > x_dst; x--) {
            delet_block(x, Y_fly, n);
            print_block(x - 1, Y_fly, n);
            Sleep(50 / speed / speed / speed);
        }
    for (int y = Y_fly; y < y_dst; y++) {
        delet_block(x_dst, y, n);
        print_block(x_dst, y + 1, n);
        Sleep(100 / speed / speed / speed);
    }
    cct_setcolor();
}
void print_block(const int X, const int Y, const int n)
{
    cct_showch(X - n, Y, ' ', n, n, 2 * n + 1);
    return;
}
void delet_block(const int X, const int Y, const int n)
{
    cct_showch(X - n, Y, ' ', COLOR_BLACK, COLOR_BLACK, 2 * n + 1);
    int X_abc[] = { X_A, X_B, X_C };
    for (int i = 0; i < 3; i++)
        if (X == X_abc[i] && (Y < Y_base && Y >= Y_base - Layers - 2))
            cct_showch(X, Y, ' ', COLOR_HYELLOW, COLOR_HYELLOW, 1);
    return;
}


//==========================================================================================================================
//==========================================================================================================================
//==========================================================================================================================