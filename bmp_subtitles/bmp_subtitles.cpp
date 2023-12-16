/* 2252201 ������ */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
//#include <conio.h>
//#include <Windows.h>
#include "../include/class_aat.h"
#include "../include/cmd_hdc_tools.h"
#include "../include/bitmap_image.h"
using namespace std;

enum OPT_ARGS {
	OPT_ARGS_HELP = 0,
	OPT_ARGS_DEBUG,
	OPT_ARGS_BMPFILE,
	OPT_ARGS_OUTFILE,
	OPT_ARGS_TRADITIONAL,
	OPT_ARGS_SUBTITLES,
	OPT_ARGS_POS_X,
	OPT_ARGS_POS_Y,
	OPT_ARGS_COLOR,
	OPT_ARGS_STYLE,
	OPT_ARGS_SHOW,
};

/***************************************************************************
  �������ƣ�usage
  ��    �ܣ��÷���ʾ
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
static void usage(const char* const argv0)
{
	char procname[256];

	/* ��"xx\\xx\\txt_compare.exe��ʽ��argv[0]�н�ȡtxt_compare����" */
	strcpy(procname, argv0); //����滻���Լ��Ľ�ȡ���� 

	ostringstream msg;
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;
	cout << endl;

	msg << procname << " [Ver1.0.1]" << endl;
	msg << endl;

	msg << "Usage: " << procname << " --bmpfile xxx --outfile xxx [ --subtitles xxx | --traditional | --posx 0..1919 | --posy 0..1079 | --color HHHHHH | --style horizontal/vertical | --show ]" << endl;

	msg << setw(wkey) << ' ' << "��ѡ�" << endl;
	msg << setw(wopt) << ' ' << "--bmpfile name1             : ԭʼbmpͼƬ���ƣ���ѡ��" << endl;
	msg << setw(wopt) << ' ' << "--outfile name2             : ����Ļ���bmpͼƬ���ƣ���ѡ��" << endl;
	msg << endl;

	msg << setw(wkey) << ' ' << "��ѡ�" << endl;
	msg << setw(wopt) << ' ' << "--subtitles xxxxxx          : Ҫ�������Ļ���֣�������10�����֣�������GB2312�еĺ���" << endl;
	msg << setw(wopt) << ' ' << "--traditional		         : �Ƿ�ʹ�÷����ַ���traditional_Chinese�����������" << endl;
	msg << setw(wopt) << ' ' << "--posx x                    : Ҫ����ĵ�һ�����ֵ����Ͻ�x���꣬Ĭ��Ϊ0" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..1919]����ʾ����ĵ�һ�����ֵ����Ͻ�x����" << endl;
	msg << setw(wopt) << ' ' << "--posy y                    : Ҫ����ĵ�һ�����ֵ����Ͻ�y���꣬Ĭ��Ϊ0" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..1079]����ʾ����ĵ�һ�����ֵ����Ͻ�y����" << endl;
	msg << setw(wopt) << ' ' << "--color HHHHHH              : ����ǰ��ɫ��6��16�����ַ����ֱ�ΪRGB������FF0000-ȫ�죩��Ĭ�ϰ�ɫ" << endl;
	msg << setw(wopt) << ' ' << "--style horizontal/vertical : ��ʾ��񣬽�����/���������ѡ��Ĭ�Ϻ���" << endl;
	msg << setw(wopt) << ' ' << "\thorizontal     : ������ʾ��Ļ" << endl;
	msg << setw(wopt) << ' ' << "\tvertical		  : ������ʾ��Ļ" << endl;
	msg << endl;

	msg << "e.g.   " << procname << " --bmpfile name1 --outfile name2 --subtitles xxxxxx" << endl;
	msg << setw(wopt) << ' ' << " : ��bmp���ϽǼӺ����ɫ������Ļ" << endl;
	msg << endl;

	/* һ��ȫ����ʾ���� */
	cout << msg.str() << endl;
}

/***************************************************************************
  �������ƣ�test_HanziFont
  ��    �ܣ������ֵ���ȡ
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void test_HanziFont()
{
	HanziFont jian("HZK16");
	char font[32];
	string s("�����ʺ");
	jian.getCharacter(&s[6], font);
	jian.displayCharacterBitmap();
	return;
}

/***************************************************************************
  �������ƣ�bitmap_subtitles
  ��    �ܣ�bmp����Ļ���ĺ���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void bitmap_subtitles_fun(args_analyse_tools args[])
{
	//�ļ���
	const string bmpfile = args[OPT_ARGS_BMPFILE].get_string();
	const string outfile = args[OPT_ARGS_OUTFILE].get_string();
	//��������
	const bool traditional = args[OPT_ARGS_TRADITIONAL].existed();
	const string subtitles = args[OPT_ARGS_SUBTITLES].get_string();
	const int posx = args[OPT_ARGS_POS_X].get_int();
	const int posy = args[OPT_ARGS_POS_Y].get_int();
	const string color = args[OPT_ARGS_COLOR].get_string();
	const string style = args[OPT_ARGS_STYLE].get_string();
	const bool show = args[OPT_ARGS_SHOW].existed();
	//��ʼ��hdc
	system("mode con cols=175 lines=45"); //��16x16�������£���֤1366x768����ʾ����
	system("cls");
	hdc_init(0, 7, 1366, 768);
	hdc_cls();
	//��ʼ����
	int ret = 1;
	{
		bitmap_subtitles bmp(bmpfile, outfile, subtitles);
		if (show)
			bmp.show_image();
		//test_HanziFont();
		//while (getchar() != '\n') continue;
		if (bmp.set_pos(posx + 5, posy + 7)) {
			cerr << "��Ļλ������ʧ�ܣ�Ĭ��Ϊ0." << endl;
			cerr << "pos x = " << posx << endl << "pos y = " << posy << endl;
		}
		if (bmp.set_color(color) == -1)
			cerr << "��Ļ��ɫ����ʧ�ܣ�Ĭ�ϰ�ɫ." << endl << "color = #" << color << endl;
		bmp.set_traditional(traditional);
		if (style == "vertical")
			bmp.set_vertical();
		ret = bmp.add_subtitles();
	}
	//����ж�
	if (ret)
		cout << "��Ļ���ʧ��" << endl;
	else {
		cout << "��Ļ��ӳɹ����س�����" << endl;
		/* �ȴ��س������� */
		while (getchar() != '\n')
			continue;
		if (show) {
			hdc_cls();
			bitmap_image result(outfile);
			result.show_image(0,0,2);
			while (getchar() != '\n')
				continue;
		}
	}
	//�ͷ�hdc
	hdc_release();
	system("mode con cols=100 lines=30");
	return;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int main(int argc, char** argv)
{
	/* ָ��ȥ���ո�ķ�ʽ */
	const string Style[] = { "horizontal",	"vertical", "" };

	args_analyse_tools args[] = {
		args_analyse_tools("--help",		ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_HELP = 0
		args_analyse_tools("--debug",		ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_DEBUG
		args_analyse_tools("--bmpfile",		ST_EXTARGS_TYPE::str, 1, string("")),					//OPT_ARGS_BMPFILE
		args_analyse_tools("--outfile",		ST_EXTARGS_TYPE::str, 1, string("")),					//OPT_ARGS_OUTFILE
		args_analyse_tools("--traditional",	ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_TRADTIONAL
		args_analyse_tools("--subtitles",	ST_EXTARGS_TYPE::str, 1, string("Ĭ����Ļ")),				//OPT_ARGS_SUBTITLE
		args_analyse_tools("--posx",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 1919),	//OPT_ARGS_POS_X
		args_analyse_tools("--posy",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 1079),	//OPT_ARGS_POS_Y
		args_analyse_tools("--color",		ST_EXTARGS_TYPE::str, 1, string("FFFFFF")),				//OPT_ARGS_COLOR
		args_analyse_tools("--style",		ST_EXTARGS_TYPE::str_with_set_default, 1, 0, Style),	//OPT_ARGS_STYLE
		args_analyse_tools("--show",		ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_SHOW
		args_analyse_tools()  //���һ�������ڽ���
	};
	int cur_argc, ret = 0;
	
	//���һ������1����ʾ��ѡ������⣬������������
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//������Ϣ�ں������Ѵ�ӡ
		args_analyse_print(args);
		usage(argv[0]);
		return -1;
	}

	/* ��help�����⴦�� */
	if (args[OPT_ARGS_HELP].existed()) {
		//ֻҪ�� --help���������������ԣ���ʾ��������
		args_analyse_print(args);
		usage(argv[0]);
		return -1; //ִ�����ֱ���˳�
	}

	if (args[OPT_ARGS_BMPFILE].existed() + args[OPT_ARGS_OUTFILE].existed() != 2) {
		usage(argv[0]);
		cout << "����ָ������[" << args[OPT_ARGS_BMPFILE].get_name() << "��" << args[OPT_ARGS_OUTFILE].get_name() << "]" << endl;
		return -1;
	}

	if (args[OPT_ARGS_DEBUG].existed())
		args_analyse_print(args);

	/* ����ʵ�ʵĹ��ܵ��ã������Ӧ�Ĺ��� */
	bitmap_subtitles_fun(args);

	return 0;
}