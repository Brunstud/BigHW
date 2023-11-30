/* 2252201 ������ �ƿ�2�� */
/*���ڲ����鷽ʽʵ�ֵĸ�����*/
#include<iostream>
//#include<iomanip>
//#include<conio.h>
#include<windows.h>
//#include "../include/cmd_console_tools.h"
//#include "../include/com_console.h"
#include"90-02-b1.h"
using namespace std;

/***************************************************************************
  �������ƣ�num::set_boundary
  ��    �ܣ�������num�ĳ�ʼ�߽�
  ���������int x, int y
  �� �� ֵ����
  ˵    ��������map��ͬʱ���Ʊ�����
***************************************************************************/
void num::set_boundary(int x, int y)
{
    boundary.x = x;
    boundary.y = y;
    reset_map();
    line = score = time = 0;
    //ori_paint();
    return;
}

/***************************************************************************
  �������ƣ�num::reset_map
  ��    �ܣ�����map
  �����������
  �� �� ֵ����
  ˵    ������num���ú���
***************************************************************************/
void num::reset_map()
{
    int* pm = map[0];
    for (; pm - map[0] < MAX_HEIGHT * MAX_WIDTH; pm++)
        *pm = 0;
    return;
}

/***************************************************************************
  �������ƣ�num::set_pos
  ��    �ܣ�������num��positionλ��
  ���������int x, int y
  �� �� ֵ����
  ˵    ����
***************************************************************************/
void num::set_pos(int x, int y)
{
    position.x = x;
    position.y = y;
    return;
}

/***************************************************************************
  �������ƣ�num::set_num
  ��    �ܣ���num��¼��cur_num�У���ͨ������num_shape��¼������״
  ���������int num
  �� �� ֵ����
  ˵    ������ʼ����Ϊ0������
***************************************************************************/
void num::set_num(int num)
{
    cur_num = num;
    direction = 0;
    point* ppoint = num_shape;
    ppoint->x = ppoint->y = end_num;
    for (ppoint++; ppoint - num_shape < NUM_SHAPE; ppoint++)
        ppoint->x = ppoint->y = 0;
    {
        ppoint = num_shape;
        switch (num) {
            case 1: //��������1
                for (int iy = -2; iy <= 2; ++ppoint, ++iy) {
                    ppoint->x = 0;
                    ppoint->y = iy;
                }
                break;
            case 0: //��������0
                for (int ix = -1; ix <= 0; ++ppoint, ++ix) {
                    ppoint->x = ix;
                    ppoint->y = 2;
                }
                for (int iy = -1; iy <= 1; ++ppoint, ++iy) {
                    ppoint->x = -1;
                    ppoint->y = iy;
                }
            case 7: //��������7
                for (int ix = -1; ix <= 0; ++ppoint, ++ix) {
                    ppoint->x = ix;
                    ppoint->y = -2;
                }
            case 4:  //����������4
                for (int iy = -2; iy <= 2; ++ppoint, ++iy) {
                    ppoint->x = 1;
                    ppoint->y = iy;
                }
                break;
            case 2:
            case 3:
            case 5:
            case 6:
            case 8:
            case 9:
                for (int iy = -2; iy <= 2; ++ppoint, iy += 2) {
                    for (int jx = -1; jx <= 1; ++ppoint, ++jx) {
                        ppoint->x = jx;
                        ppoint->y = iy;
                    }
                }
                break;
            default:
                cout << "error:set_num(" << num << ")!";
        }
        switch (num) {
            case 4:
                for (int iy = -2; iy <= 0; ++ppoint, ++iy) {
                    ppoint->x = -1;
                    ppoint->y = iy;
                }
                ppoint->x = ppoint->y = 0;
                ppoint++;
                break;
            case 2:
                ppoint->x = 1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = -1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 3:
                ppoint->x = 1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 5:
                ppoint->x = -1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 6:
                ppoint->x = -1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = -1;
                ppoint->y = 1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 8:
                ppoint->x = -1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = -1;
                ppoint->y = 1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
            case 9:
                ppoint->x = -1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = -1;
                ppoint++;
                ppoint->x = 1;
                ppoint->y = 1;
                ppoint++;
                break;
        }
        ppoint->x = ppoint->y = end_num;//��ʾ��������
    }
    return;
}

/***************************************************************************
  �������ƣ�num::rotate
  ��    �ܣ���ʱ����ת���ֲ�������ʾ
  �����������
  �� �� ֵ����
  ˵    �����Դ����������»��ƹ���,��Ҫ��ʹ��paint_num��start_num
***************************************************************************/
void num::rotate()
{
    erase_num();
    int possible = 0;
    while (!possible) {
        possible = 1;
        for (point* ppoint = num_shape; ppoint->x != end_num; ppoint++) {
            int tmp = ppoint->x;
            ppoint->x = -ppoint->y;
            ppoint->y = tmp;
            int x = position.x + ppoint->x;
            int y = position.y + ppoint->y;
            if (possible && (x < 0 || x >= boundary.x || y >= boundary.y || y >= 0 && map[y][x]))
                possible = 0;
        }
        direction = ++direction % 4;
    }
    paint_num();
    return;
}

/***************************************************************************
  �������ƣ�num::start_num
  ��    �ܣ�ʹ�Ѿ����õ����ִ��м俪ʼ����
  �����������
  �� �� ֵ�����������������1�����������򷵻�0
  ˵    �����Դ����ƹ��ܣ���Ҫ��ͨ��set_num��show_nex_num���ú�num
***************************************************************************/
int num::start_num()
{
    //set_pos(boundary.x / 2, -2);
    set_pos(rand()%(boundary.x - 2) + 1, -2);
    for (point* ppoint = num_shape; ppoint->x != end_num; ppoint++)
        if (2 == ppoint->y && map[0][position.x + ppoint->x])
            return 1;
    paint_num();
    return 0;
}

/***************************************************************************
  �������ƣ�num::drop_num
  ��    �ܣ�ʹ�����½�һ��
  �����������
  �� �� ֵ���䵽�ײ�����1,��Ϸ��������-1,�������䷵��0
  ˵    �����Դ����������»��ƹ��ܣ���Ҫ��ʹ��start_num
***************************************************************************/
int num::drop_num()
{
    if (land()) {
        if (over()) {
            //cout << "over!";
            return -1;
        }
        return 1;
    }
    erase_num();
    position.y++;
    paint_num();
    return 0;
}

/***************************************************************************
  �������ƣ�num::land()
  ��    �ܣ��ж������Ƿ��Ѿ����ײ�
  �����������
  �� �� ֵ���Ѿ����ײ�����1�����򷵻�0
  ˵    ������num�ڲ�����
***************************************************************************/
int num::land()
{
    point* ppoint = num_shape;
    for (; ppoint->x != end_num; ppoint++) {
        int x = position.x + ppoint->x;
        int y = position.y + ppoint->y;
        if (boundary.y - 1 == y || y + 1 >= 0 && map[y + 1][x]) {
           // cout << "already_land!";
            return 1;
        }
    }
    return 0;
}

/***************************************************************************
  �������ƣ�num::over()
  ��    �ܣ��жϸþ���Ϸ�Ƿ����
  �����������
  �� �� ֵ���Ѿ�����������1�����򷵻�0
  ˵    ������num�ڲ���������land()֮������ж�
***************************************************************************/
int num::over()
{
    point* ppoint = num_shape;
    for (; ppoint->x != end_num; ppoint++) {
        if (position.y + ppoint->y < 0)
            return 1;
    }
    return 0;
}

/***************************************************************************
  �������ƣ�num::record_num()
  ��    �ܣ�����ǰ���ּ���λ�ü�¼��map��
  �����������
  �� �� ֵ����
  ˵    ������land()֮��ʹ��
***************************************************************************/
void num::record_num()
{
    point* ppoint = num_shape;
    for (; ppoint->x != end_num; ppoint++) {
        int x = position.x + ppoint->x;
        int y = position.y + ppoint->y;
        map[y][x] = 1 + cur_num;
    }
    return;
}

/***************************************************************************
  �������ƣ�num::check_line()
  ��    �ܣ�����������������������eliminate()��������
  �����������
  �� �� ֵ����ǰ��������
  ˵    ������record_num()֮��ʹ��
***************************************************************************/
int num::check_line()
{
    int line = 0;
    int(*pmap)[MAX_WIDTH] = map;
    int* ppoint = map[0];
    for (pmap = map; pmap - map < boundary.y; pmap++) {
        for (ppoint = *pmap; ppoint - *pmap < boundary.x; ppoint++)
            if (!*ppoint)
                break;
        if (ppoint - *pmap == boundary.x) {//���ֿ�������
            line++;
            eliminate(pmap - map);
        }
    }
    return line;
}

/***************************************************************************
  �������ƣ�num::end_a_num()
  ��    �ܣ������䵽��֮���¼��ǰ���֣������в����·���
  �����������
  �� �� ֵ����
  ˵    ������land()�ж�Ϊ1֮��ʹ��
***************************************************************************/
void num::end_a_num()
{
    record_num();
    renew_score(check_line());
    return;
}