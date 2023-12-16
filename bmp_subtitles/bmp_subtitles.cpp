/* 2252201 胡世成 */
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
  函数名称：usage
  功    能：用法提示
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
static void usage(const char* const argv0)
{
	char procname[256];

	/* 从"xx\\xx\\txt_compare.exe形式的argv[0]中截取txt_compare出来" */
	strcpy(procname, argv0); //这句替换成自己的截取函数 

	ostringstream msg;
	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;
	cout << endl;

	msg << procname << " [Ver1.0.1]" << endl;
	msg << endl;

	msg << "Usage: " << procname << " --bmpfile xxx --outfile xxx [ --subtitles xxx | --traditional | --posx 0..1919 | --posy 0..1079 | --color HHHHHH | --style horizontal/vertical | --show ]" << endl;

	msg << setw(wkey) << ' ' << "必选项：" << endl;
	msg << setw(wopt) << ' ' << "--bmpfile name1             : 原始bmp图片名称（必选）" << endl;
	msg << setw(wopt) << ' ' << "--outfile name2             : 加字幕后的bmp图片名称（必选）" << endl;
	msg << endl;

	msg << setw(wkey) << ' ' << "可选项：" << endl;
	msg << setw(wopt) << ' ' << "--subtitles xxxxxx          : 要加入的字幕文字（不超过10个汉字，仅考虑GB2312有的汉字" << endl;
	msg << setw(wopt) << ' ' << "--traditional		         : 是否使用繁体字符（traditional_Chinese），无则简体" << endl;
	msg << setw(wopt) << ' ' << "--posx x                    : 要加入的第一个汉字的左上角x坐标，默认为0" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..1919]，表示加入的第一个汉字的左上角x坐标" << endl;
	msg << setw(wopt) << ' ' << "--posy y                    : 要加入的第一个汉字的左上角y坐标，默认为0" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..1079]，表示加入的第一个汉字的左上角y坐标" << endl;
	msg << setw(wopt) << ' ' << "--color HHHHHH              : 汉字前景色，6个16进制字符，分别为RGB（例：FF0000-全红），默认白色" << endl;
	msg << setw(wopt) << ' ' << "--style horizontal/vertical : 显示风格，仅横向/纵向两项可选，默认横向" << endl;
	msg << setw(wopt) << ' ' << "\thorizontal     : 横向显示字幕" << endl;
	msg << setw(wopt) << ' ' << "\tvertical		  : 纵向显示字幕" << endl;
	msg << endl;

	msg << "e.g.   " << procname << " --bmpfile name1 --outfile name2 --subtitles xxxxxx" << endl;
	msg << setw(wopt) << ' ' << " : 在bmp左上角加横向白色简体字幕" << endl;
	msg << endl;

	/* 一次全部显示出来 */
	cout << msg.str() << endl;
}

/***************************************************************************
  函数名称：test_HanziFont
  功    能：测试字典库读取
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void test_HanziFont()
{
	HanziFont jian("HZK16");
	char font[32];
	string s("我想吃屎");
	jian.getCharacter(&s[6], font);
	jian.displayCharacterBitmap();
	return;
}

/***************************************************************************
  函数名称：bitmap_subtitles
  功    能：bmp加字幕核心函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void bitmap_subtitles_fun(args_analyse_tools args[])
{
	//文件名
	const string bmpfile = args[OPT_ARGS_BMPFILE].get_string();
	const string outfile = args[OPT_ARGS_OUTFILE].get_string();
	//参数配置
	const bool traditional = args[OPT_ARGS_TRADITIONAL].existed();
	const string subtitles = args[OPT_ARGS_SUBTITLES].get_string();
	const int posx = args[OPT_ARGS_POS_X].get_int();
	const int posy = args[OPT_ARGS_POS_Y].get_int();
	const string color = args[OPT_ARGS_COLOR].get_string();
	const string style = args[OPT_ARGS_STYLE].get_string();
	const bool show = args[OPT_ARGS_SHOW].existed();
	//初始化hdc
	system("mode con cols=175 lines=45"); //在16x16的字体下，保证1366x768的显示区域
	system("cls");
	hdc_init(0, 7, 1366, 768);
	hdc_cls();
	//开始处理
	int ret = 1;
	{
		bitmap_subtitles bmp(bmpfile, outfile, subtitles);
		if (show)
			bmp.show_image();
		//test_HanziFont();
		//while (getchar() != '\n') continue;
		if (bmp.set_pos(posx + 5, posy + 7)) {
			cerr << "字幕位置设置失败，默认为0." << endl;
			cerr << "pos x = " << posx << endl << "pos y = " << posy << endl;
		}
		if (bmp.set_color(color) == -1)
			cerr << "字幕颜色设置失败，默认白色." << endl << "color = #" << color << endl;
		bmp.set_traditional(traditional);
		if (style == "vertical")
			bmp.set_vertical();
		ret = bmp.add_subtitles();
	}
	//结果判断
	if (ret)
		cout << "字幕添加失败" << endl;
	else {
		cout << "字幕添加成功，回车继续" << endl;
		/* 等待回车键继续 */
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
	//释放hdc
	hdc_release();
	system("mode con cols=100 lines=30");
	return;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int main(int argc, char** argv)
{
	/* 指定去除空格的方式 */
	const string Style[] = { "horizontal",	"vertical", "" };

	args_analyse_tools args[] = {
		args_analyse_tools("--help",		ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_HELP = 0
		args_analyse_tools("--debug",		ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_DEBUG
		args_analyse_tools("--bmpfile",		ST_EXTARGS_TYPE::str, 1, string("")),					//OPT_ARGS_BMPFILE
		args_analyse_tools("--outfile",		ST_EXTARGS_TYPE::str, 1, string("")),					//OPT_ARGS_OUTFILE
		args_analyse_tools("--traditional",	ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_TRADTIONAL
		args_analyse_tools("--subtitles",	ST_EXTARGS_TYPE::str, 1, string("默认字幕")),				//OPT_ARGS_SUBTITLE
		args_analyse_tools("--posx",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 1919),	//OPT_ARGS_POS_X
		args_analyse_tools("--posy",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 1079),	//OPT_ARGS_POS_Y
		args_analyse_tools("--color",		ST_EXTARGS_TYPE::str, 1, string("FFFFFF")),				//OPT_ARGS_COLOR
		args_analyse_tools("--style",		ST_EXTARGS_TYPE::str_with_set_default, 1, 0, Style),	//OPT_ARGS_STYLE
		args_analyse_tools("--show",		ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_SHOW
		args_analyse_tools()  //最后一个，用于结束
	};
	int cur_argc, ret = 0;
	
	//最后一个参数1，表示除选项参数外，还有其它参数
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		args_analyse_print(args);
		usage(argv[0]);
		return -1;
	}

	/* 对help做特殊处理 */
	if (args[OPT_ARGS_HELP].existed()) {
		//只要有 --help，其它参数都忽略，显示帮助即可
		args_analyse_print(args);
		usage(argv[0]);
		return -1; //执行完成直接退出
	}

	if (args[OPT_ARGS_BMPFILE].existed() + args[OPT_ARGS_OUTFILE].existed() != 2) {
		usage(argv[0]);
		cout << "必须指定参数[" << args[OPT_ARGS_BMPFILE].get_name() << "和" << args[OPT_ARGS_OUTFILE].get_name() << "]" << endl;
		return -1;
	}

	if (args[OPT_ARGS_DEBUG].existed())
		args_analyse_print(args);

	/* 进入实际的功能调用，完成相应的功能 */
	bitmap_subtitles_fun(args);

	return 0;
}