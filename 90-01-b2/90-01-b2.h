/*2252201 胡世成 信18 */
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
/*基础设置*/
int score_by(int found);//每局得分计算
int score_in_end(int num);//计算该局结束得分

/*popstar-base.cpp*/
/*放内部数组方式实现的各函数*/
void ori_set_star(int star[][LINE], int row, int line);
void show_array(const char * intro, const int star[][LINE], const char find[][LINE], int row, int line, int sign);
int input_step(int star[][LINE], char find[][LINE], int* r, int* l, int row, int line);
int question1(int star[][LINE], char find[][LINE], int* r, int* l, int row, int line);
int question2(int star[][LINE], int* r, int* l, int row, int line, int score);
void question3(int star[][LINE], int* r, int* l, int row, int line, int score);

/*popstar-console.cpp*/
/*放cmd图形界面方式实现的各函数*/
void ori_paint(const int star[][LINE], int row, int line, int div);
void paint_star(int x, int y, int bg_color, int fg_color);
int mouse_key(const int star[][LINE], int* r, int* l, int row, int line, int div);
void choose_star(const int star[][LINE], int* r, int* l, int row, int line, int div);
int choose_star_block(const int star[][LINE], char find[][LINE], int* r, int* l, int row, int line, int div);
void drop_by_row(const int star[][LINE], int r, int l, int div);
void drop_by_line(const int star[][LINE], int r, int l, int div);
int paint_one_step(int star[][LINE], int row, int line, int div, int wait = 0);
void paint_game(int row, int line, int div, int wait = 0);//模式G主函数

/*popstar-tools.cpp*/
/*放一些内部数字/图形方式公用的函数，如判断结束等*/
void operate_by_choice(int star[][LINE], int row, int line, char choice);
int find_star(const int star[][LINE], char find[][LINE], int r, int l);
void delete_star(int star[][LINE], const char find[][LINE], int paint = 0, int div = 0);
void fallen_star(int star[][LINE], const char find[][LINE], int row, int line, int paint = 0, int div = 0);
int check_star(const int star[][LINE], int row, int line);
//================================================================================================PART H
int auto_choose_star_block(const int star[][LINE], char find[][LINE], int* r, int* l, int row, int line, int div);
//代替popstar-console.cpp中的choose_star_block函数后可以实现找寻并显示最优消灭策略的功能，故名auto