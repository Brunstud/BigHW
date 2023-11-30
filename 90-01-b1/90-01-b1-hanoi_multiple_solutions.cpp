/* 2252201 胡世成 信18 */
#include<iostream>
#include<iomanip>
#include <conio.h>
#include<windows.h>
#include "../include/cmd_console_tools.h"
#include"90-01-b1-hanoi.h"
using namespace std;

/* ----------------------------------------------------------------------------------

     本文件功能：
	1、存放被 hanoi_main.cpp 中根据菜单返回值调用的各菜单项对应的执行函数

     本文件要求：
	1、不允许定义外部全局变量（const及#define不在限制范围内）
	2、允许定义静态全局变量（具体需要的数量不要超过文档显示，全局变量的使用准则是：少用、慎用、能不用尽量不用）
	3、静态局部变量的数量不限制，但使用准则也是：少用、慎用、能不用尽量不用
	4、按需加入系统头文件、自定义头文件、命名空间等

   ----------------------------------------------------------------------------------- */

/***************************************************************************
  函数名称：input
  功    能：输入n, src, tmp, dst, speed
  输入参数：char * src, char * tmp, char * dst
            int speed : 是否输入speed，若是则返回speed的值，否则返回0
  返 回 值：层数n
  说    明：
***************************************************************************/
int input(int * n, char * src, char * tmp, char * dst, int speed)
{
    cct_gotoxy(0, 15);
    do {                                    //输入层数n
        cin.clear();
        cout << "请输入汉诺塔的层数(1-10)" << endl;
        cin >> *n;
        while (getchar() != '\n')
            continue;
    } while (!cin || *n < 1 || *n > Layers);
    do {                                    //输入起始柱src
        cin.clear();
        cout << "请输入起始柱(A-C)" << endl;
        cin >> *src;
        if (*src >= 'a' && *src <= 'c')
            *src = *src - 'a' + 'A';
        while (getchar() != '\n')
            continue;
    } while (!cin || *src < 'A' || *src > 'C');
    do {                                    //输入目标柱dst
        cin.clear();
        cout << "请输入目标柱(A-C)" << endl;
        cin >> *dst;
        if (*dst >= 'a' && *dst <= 'c')
            *dst = *dst - 'a' + 'A';
        if (*dst == *src)
            cout << "目标柱(" << *dst << ")不能与起始柱(" << *src << ")相同" << endl;
        while (getchar() != '\n')
            continue;
    } while (!cin || *dst < 'A' || *dst > 'C' || *dst == *src);
    *tmp = 'A' + 'B' + 'C' - *src - *dst;          //计算中间柱tmp
    if (speed)
        do {                                    //输入移动速度speed
            cin.clear();
            cout << "请输入移动速度(0-5: 0-按回车单步演示 1-延时最长 5-延时最短)";
            cin >> speed;
            while (getchar() != '\n')
                continue;
        } while (!cin || speed < 0 || speed > 5);
    return speed;
}

/***************************************************************************
  函数名称：ori_set_stack
  功    能：设置初始stack与top
  输入参数：int stack[][Layers], int top[], int n, char src
  返 回 值：无
  说    明：
***************************************************************************/
void ori_set_stack(int stack[][Layers], int top[], int n, char src)
{
    switch (src) {                             //设置初始stack与top
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
  函数名称：start
  功    能：初始设置
  输入参数：int stack[][Layers], int top[], int n, char src, char dst, int speed, int choice
  返 回 值：无
  说    明：
***************************************************************************/
void start(int stack[][Layers], int top[], int n, char src, char dst, int speed, int choice)
{
    if (choice < 4)
        return;
    cct_cls();
    cct_gotoxy(0,0);
    cout << "从" << setw(2) << src << " 移动到" << setw(2) << dst << "，共" << setw(2) << n << " 层";
    if (4 == choice || 8 == choice)
        cout << "，延时设置为" << setw(2) << speed;
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
  函数名称：hanoi_game
  功    能：汉诺塔游戏
  输入参数：int stack[][Layers], int top[], int n, char src, char dst
  返 回 值：无
  说    明：
***************************************************************************/
void hanoi_game(int stack[][Layers], int top[], int n, char src, char dst)
{
    int end = 0;
    char s, d;
    int N = 0;//计步数
    cct_gotoxy(0, 36);
    cout << "请输入移动的柱号(命令形式：AC = A顶端的盘子移动到C，Q = 退出) ：";
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
                    cout << "源柱为空！";
                else
                    cout << "大盘压小盘，非法移动！";
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
        cout << "游戏终止！"<< endl << endl;
    }
    else {
        cct_gotoxy(0, 37);
        cout << "游戏胜利！共用" << N << "步" << endl << endl;
    }
    return;
}

/***************************************************************************
  函数名称：stack_operation
  功    能：内置数组操作函数
  输入参数：int stack[][Layers], int top[], char src, char dst
  返 回 值：
  说    明：
***************************************************************************/
void stack_operation(int stack[][Layers], int top[], char src, char dst)
{
    switch (src) {                             //调整stack
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
  函数名称：hanoi_operation
  功    能：汉诺塔递归函数内置操作函数
  输入参数：int stack[][Layers], int top[], int n, char src, char dst, int N, int speed, int choice
  返 回 值：
  说    明：
***************************************************************************/
void hanoi_operation(int stack[][Layers], int top[], int n, char src, char dst, int N, int speed, int choice)
{
    stack_operation(stack, top, src, dst);
    if (1 == choice) {
        cout << n << "# " << src << "---->" << dst << endl;
    }
    if (2 == choice) {
        cout << "第" << setw(4) << N << " 步(" << setw(2) << n << "#: " << src << "-->" << dst << ")" << endl;
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
  函数名称：hanoi
  功    能：汉诺塔递归函数
  输入参数：int stack[][Layers], int top[]：传递数组
            int n：层数
            char src：起始柱
            char tmp：中间柱
            char dst：目标柱
            int N   : 步数，初始输入1
            int speed  : 速度/延时
            int choice : 选项
  返 回 值：步骤数N
  说    明：
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
  函数名称：show_array
  功    能：显示内部数组
  输入参数：int stack[][Layers], int n, char src, char dst, int N
  返 回 值：无
  说    明：
***************************************************************************/
void show_array(int stack[][Layers], int n, char src, char dst, int N)
{
    if (N) {
        cout << "第" << setw(4) << N << " 步(" << setw(2) << n << "#: " << src << "-->" << dst << ") ";
    }
    else
        cout << "初始: ";
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
  函数名称：show_tower
  功    能：显示汉诺塔
  输入参数：int stack[][Layers], int n, char src, char dst, int N, const int Y
  返 回 值：空
  说    明：
***************************************************************************/
void show_tower(int stack[][Layers], int n, char src, char dst, int N, const int Y)
{
    int X = 12;
    int gap = 9;//ABC之间间距
    cct_gotoxy(X, Y);//打印汉诺塔
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
  函数名称：print_base()
  功    能：打印三个底盘
  输入参数：无
  返 回 值：空
  说    明：
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
  函数名称：print_load
  功    能：初始条件下加装块块
  输入参数：int stack[][Layers]
  返 回 值：空
  说    明：
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
  函数名称：first_move
  功    能：只移动第一步
  输入参数：int stack[][Layers], int top[], int n, int src, int dst
  返 回 值：空
  说    明：
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
  函数名称：move_block
  功    能：移动模块
  输入参数：int top[], int n, int src, int dst, int speed
  返 回 值：空
  说    明：内嵌有print_block与delet_block两个仅在本函数中使用的函数
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