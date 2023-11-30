/* 2252201 ������ ��18 */
#pragma once

/* ------------------------------------------------------------------------------------------------------

     ���ļ����ܣ�
	1��Ϊ�˱�֤ hanoi_main.cpp/hanoi_menu.cpp/hanoi_multiple_solutions.cpp ���໥���ʺ����ĺ�������
	2��һ�����ϵ�cpp���õ��ĺ궨�壨#define����ȫ��ֻ����const����������������
	3�����Բο� cmd_console_tools.h ��д���������Ķ�����ᣩ
   ------------------------------------------------------------------------------------------------------ */

#define Layers 10
#define X_A Layers + 1
#define X_B X_A + 3 * Layers + 2
#define X_C X_B + 3 * Layers + 2
#define Y_base 4 + Layers + 1
#define Y_fly 1

//�˵����ʼ�趨
int input(int* n, char* src, char* tmp, char* dst, int speed);
void ori_set_stack(int stack[][Layers], int top[], int n, char src);
void start(int stack[][Layers], int top[], int n, char src, char dst, int speed, int choice);
void hanoi_game(int stack[][Layers], int top[], int n, char src, char dst);

//��ŵ���ݹ麯��������Ƕ����
int hanoi(int stack[][Layers], int top[], int n, char src, char tmp, char dst, int N, int speed, int choice);
void hanoi_operation(int stack[][Layers], int top[], int n, char src, char dst, int N, int speed, int choice);
void stack_operation(int stack[][Layers], int top[], char src, char dst);

//��������ʾ��Ƕ����
void show_array(int stack[][Layers], int n, char src, char dst, int N);
void show_tower(int stack[][Layers], int n, char src, char dst, int N, const int Y);

//αͼ�β�������
void print_base();
void print_load(int stack[][Layers]);
void first_move(int stack[][Layers], int top[], int n, int src, int dst);
void move_block(int top[], int n, int src, int dst, int speed);