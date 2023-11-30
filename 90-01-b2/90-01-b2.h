/*2252201 ������ ��18 */
#pragma once

#define MIN 4
#define ROW 10
#define LINE 10
#define STAR 5
#define WAIT 2
#define WAIT_STAR 5

#define X_L0 4
#define Y_R0 3

/*popstar-main.cpp*/
/*��������*/
int score_by(int found);//ÿ�ֵ÷ּ���
int score_in_end(int num);//����þֽ����÷�

/*popstar-base.cpp*/
/*���ڲ����鷽ʽʵ�ֵĸ�����*/
void ori_set_star(int star[][LINE], int row, int line);
void show_array(const char * intro, const int star[][LINE], const char find[][LINE], int row, int line, int sign);
int input_step(int star[][LINE], char find[][LINE], int* r, int* l, int row, int line);
int question1(int star[][LINE], char find[][LINE], int* r, int* l, int row, int line);
int question2(int star[][LINE], int* r, int* l, int row, int line, int score);
void question3(int star[][LINE], int* r, int* l, int row, int line, int score);

/*popstar-console.cpp*/
/*��cmdͼ�ν��淽ʽʵ�ֵĸ�����*/
void ori_paint(const int star[][LINE], int row, int line, int div);
void paint_star(int x, int y, int bg_color, int fg_color);
int mouse_key(const int star[][LINE], int* r, int* l, int row, int line, int div);
void choose_star(const int star[][LINE], int* r, int* l, int row, int line, int div);
int choose_star_block(const int star[][LINE], char find[][LINE], int* r, int* l, int row, int line, int div);
void drop_by_row(const int star[][LINE], int r, int l, int div);
void drop_by_line(const int star[][LINE], int r, int l, int div);
int paint_one_step(int star[][LINE], int row, int line, int div, int wait = 0);
void paint_game(int row, int line, int div, int wait = 0);//ģʽG������

/*popstar-tools.cpp*/
/*��һЩ�ڲ�����/ͼ�η�ʽ���õĺ��������жϽ�����*/
void operate_by_choice(int star[][LINE], int row, int line, char choice);
int find_star(const int star[][LINE], char find[][LINE], int r, int l);
void delete_star(int star[][LINE], const char find[][LINE], int paint = 0, int div = 0);
void fallen_star(int star[][LINE], const char find[][LINE], int row, int line, int paint = 0, int div = 0);
int check_star(const int star[][LINE], int row, int line);
//================================================================================================PART H
int auto_choose_star_block(const int star[][LINE], char find[][LINE], int* r, int* l, int row, int line, int div);
//����popstar-console.cpp�е�choose_star_block���������ʵ����Ѱ����ʾ����������ԵĹ��ܣ�����auto