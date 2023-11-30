/* 2252201 胡世成 信18 */
#pragma once

/* ------------------------------------------------------------------------------------------------------

     本文件功能：
	1、为了保证 hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp 能相互访问函数的函数声明
	2、一个以上的cpp中用到的宏定义（#define）或全局只读（const）变量，个数不限
	3、可以参考 cmd_console_tools.h 的写法（认真阅读并体会）
   ------------------------------------------------------------------------------------------------------ */

#define Layers 10
#define X_A Layers + 1
#define X_B X_A + 3 * Layers + 2
#define X_C X_B + 3 * Layers + 2
#define Y_base 4 + Layers + 1
#define Y_fly 1

//菜单与初始设定
int input(int* n, char* src, char* tmp, char* dst, int speed);
void ori_set_stack(int stack[][Layers], int top[], int n, char src);
void start(int stack[][Layers], int top[], int n, char src, char dst, int speed, int choice);
void hanoi_game(int stack[][Layers], int top[], int n, char src, char dst);

//汉诺塔递归函数及其内嵌函数
int hanoi(int stack[][Layers], int top[], int n, char src, char tmp, char dst, int N, int speed, int choice);
void hanoi_operation(int stack[][Layers], int top[], int n, char src, char dst, int N, int speed, int choice);
void stack_operation(int stack[][Layers], int top[], char src, char dst);

//横纵向显示内嵌数组
void show_array(int stack[][Layers], int n, char src, char dst, int N);
void show_tower(int stack[][Layers], int n, char src, char dst, int N, const int Y);

//伪图形操作函数
void print_base();
void print_load(int stack[][Layers]);
void first_move(int stack[][Layers], int top[], int n, int src, int dst);
void move_block(int top[], int n, int src, int dst, int speed);