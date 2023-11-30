/* 2252201 ������ �ƿ�2�� */
#pragma once

/* �궨����� */
#define MIN_HEIGHT 18
#define MAX_HEIGHT 26
#define MIN_WIDTH 12
#define MAX_WIDTH 21
#define NUM_SHAPE 20//��С��ʱ���������ʱ��ջ���

/* ����ʵ���� */
struct point { int x; int y; };
class num {
private:
/* ========���ڲ���======== */
    /* ���̲��� */
    int line = 0, score = 0, numbers = 1, time = 0, speed = 1;
    point boundary;
    int map[MAX_HEIGHT][MAX_WIDTH] = { 0 };
    /* �������ֲ��� */
    int cur_num;
    int nex_num = end_num;
    const int end_num = 10;
    point position;
    int direction = 0;
    point num_shape[NUM_SHAPE] = { end_num };
/* ========�ڲ�����======== */
    /* Ԥ�ô��� */
    void reset_map();
    /* �������䴦�� */
    int land();
    int over();
    void record_num();
    int check_line();
    void eliminate(int line);
    void renew_score(int line);
public:
    /* ��ʼ���� */
    void set_boundary(int x, int y);
    void set_pos(int x, int y);
    void set_num(int num);
    /* ��ͼ���� */
    void ori_paint(int instru = 0);
    void paint_num();
    void erase_num();
    void show_nex_num(int num);
    void rotate();
    /* �������书�ܺ��� */
    int start_num();
    int drop_num();
    void end_a_num();
    /* ���̶�ȡ�ƶ� */
    int move();
    /* ʱ������ */
    int time_fly(int t);
    /* ������Ϸ */
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