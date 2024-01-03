/*2251561 任昱扬 自动化*/
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
int leap(int year) {
	if (((year % 4 == 0) && (year % 100 != 0)) || (year % 400 == 0))
