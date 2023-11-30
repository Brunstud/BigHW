/* 2252201 胡世成 计科2班 */
#pragma once

/* 宏定义参数 */
#define MIN_HEIGHT 18
#define MAX_HEIGHT 26
#define MIN_WIDTH 12
#define MAX_WIDTH 21
#define NUM_SHAPE 20//开小了时，程序结束时会栈溢出

/* 核心实现类 */
struct point { int x; int y; };
class num {
private:
/* ========类内参数======== */
    /* 棋盘参数 */
    int line = 0, score = 0, numbers = 1, time = 0, speed = 1;
    point boundary;
    int map[MAX_HEIGHT][MAX_WIDTH] = { 0 };
    /* 单个数字参数 */
    int cur_num;
    int nex_num = end_num;
    const int end_num = 10;
    point position;
    int direction = 0;
    point num_shape[NUM_SHAPE] = { end_num };
/* ========内部函数======== */
    /* 预置处理 */
    void reset_map();
    /* 结束下落处理 */
    int land();
    int over();
    void record_num();
    int check_line();
    void eliminate(int line);
    void renew_score(int line);
public:
    /* 初始设置 */
    void set_boundary(int x, int y);
    void set_pos(int x, int y);
    void set_num(int num);
    /* 绘图函数 */
    void ori_paint(int instru = 0);
    void paint_num();
    void erase_num();
    void show_nex_num(int num);
    void rotate();
    /* 数字下落功能函数 */
    int start_num();
    int drop_num();
    void end_a_num();
    /* 键盘读取移动 */
    int move();
    /* 时间流逝 */
    int time_fly(int t);
    /* 完整游戏 */
    void GAME(int seed);
};

/* 90-02-b1-console */
//void num::ori_paint();
//void num::paint_num();
//void num::erase_num();
//void num::show_nex_num(int num);
//void num::eliminate(int line);
//void num::renew_score(int lines);
//int num::move();
//int num::time_fly(int t);

/* 90-02-b1-base */
//void num::set_boundary(int x, int y);
//void num::reset_map();
//void num::set_pos(int x, int y);
//void num::set_num(int num);
//void num::rotate();
//int num::start_num();
//int num::drop_num();
//int num::land();
//int num::over();
//void num::record_num();
//int num::check_line();
//void num::end_a_num();

/* 90-02-b1-tools */
void operate(num* game, char choice);
int get_next_num(const bool new_seed = false, const unsigned int seed = 0);
//void num::GAME(int seed);