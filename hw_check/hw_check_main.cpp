/* 2252201 胡世成 计科 */
#include <iostream>     // cin,cout等
#include <iomanip>      // setw等
#include "hw_check.h"	// 作业检查
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有
#include "../include/class_aat.h"                    // 读指令
#include "../include/class_crt.h"					 // 读配置文件
using namespace std;

enum OPT_ARGS {
	OPT_ARGS_HELP = 0,
	OPT_ARGS_DEBUG,
	OPT_ARGS_ACTION,
	OPT_ARGS_CNO,
	OPT_ARGS_STU,
	OPT_ARGS_FILE,
	OPT_ARGS_CHAPTER,
	OPT_ARGS_WEEK,
	OPT_ARGS_DISPLAY,
	OPT_ARGS_CFGFILE
};

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
static void usage(const char* const full_procname)
{
	const char *procname = strrchr(full_procname, '\\');
	if (procname == NULL)
		procname = full_procname;

	const int wkey = 7 + strlen(procname) + 1;
	const int wopt = 7 + strlen(procname) + 4;
	cout << endl;

	cout << "Usage: " << procname << " 必选项/可选项" << endl;
	cout << endl;
	cout << setw(wkey) << ' ' << "必选项：指定操作" << endl;
	cout << setw(wopt) << ' ' << "--action opname : 支持的opname如下" << endl;
	cout << setw(wopt) << ' ' << "\t base              : 基础检查(文件是否提交、是否被改动、编码是否正确)" << endl;
	cout << setw(wopt) << ' ' << "\t firstline         : 首行检查（仅源程序文件需要）" << endl;
	cout << setw(wopt) << ' ' << "\t secondline        : 次行检查（仅部分源程序文件需要）" << endl;

	cout << setw(wkey) << ' ' << "必选项：指定课号" << endl;
	cout << setw(wopt) << ' ' << "--cno course_no     : 课号" << endl;

	cout << setw(wkey) << ' ' << "必选项：指定学生" << endl;
	cout << setw(wopt) << ' ' << "--stu no/all        : 指定单个/全部(7位数字为学号,all为全部)" << endl;

	cout << setw(wkey) << ' ' << "必选项：指定文件" << endl;
	cout << setw(wopt) << ' ' << "--file filename/all : 具体文件名/全部(all为全部,其余为具体文件名)" << endl;

	cout << setw(wkey) << ' ' << "可选项：" << endl;
	cout << setw(wopt) << ' ' << "--chapter n         : 在--file的基础上再进行章节的筛选(无/-1则全部章节,可与--week共存)" << endl;
	cout << setw(wopt) << ' ' << "--week n            : 在--file的基础上再进行周次的筛选(无/-1则全部周次,可与--chapter共存)" << endl;
	cout << setw(wopt) << ' ' << "--display xxxxx     : 每位0/1分别表示正常信息/未提交信息/错误信息/汇总信息/严重错误信息" << endl;
	cout << setw(wopt) << ' ' << "--cfgfile filename  : 指定配置文件名(缺省:hw_check.conf)" << endl;
	cout << endl;

	const char* DEMO_CNO = "10108001";
	const char* DEMO_SRC_FNAME = "12-b1.cpp";
	const char* DEMO_STUNO = "2259999";

	cout << "e.g.   " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all             : 检查" << DEMO_CNO << "所有学生的所有作业的基本信息" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : 检查" << DEMO_CNO << "所有学生的第4章作业的基本信息" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu all --file all --week 6    : 检查" << DEMO_CNO << "所有学生的第6周作业的基本信息" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的所有作业的基本信息" << endl;
	cout << "       " << procname << " --action base --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的" << DEMO_SRC_FNAME << "作业的基本信息" << endl;
	cout << endl;

	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all             : 检查" << DEMO_CNO << "所有学生的所有作业的首行信息" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --chapter 4 : 检查" << DEMO_CNO << "所有学生的第4章作业的首行信息" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu all --file all --week 6    : 检查" << DEMO_CNO << "所有学生的第6周作业的首行信息" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file all         : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的所有作业的首行信息" << endl;
	cout << "       " << procname << " --action firstline --cno " << DEMO_CNO << " --stu " << DEMO_STUNO << " --file " << DEMO_SRC_FNAME << "   : 检查" << DEMO_CNO << "的" << DEMO_STUNO << "学生的" << DEMO_SRC_FNAME << "作业的首行信息" << endl;
	cout << endl;

	cout << "       " << procname << " --action secondline --cno " << DEMO_CNO << " --stu all --file " << DEMO_SRC_FNAME << " : 检查" << DEMO_CNO << "的所有学生的" << DEMO_SRC_FNAME << "作业的次行信息" << endl;
	cout << endl;

	cout << "       " << procname << " --cfgfile E:\\test\\my.conf --action base --cno " << DEMO_CNO << " --stu all --file all : 检查" << DEMO_CNO << "所有学生的所有作业的基本信息(指定配置文件)" << endl;
	cout << endl;


	cout << endl;
}

/***************************************************************************
  函数名称：hw_check
  功    能：作业检查核心函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hw_check_fun(args_analyse_tools args[])
{
	// 获得参数
	const string action = args[OPT_ARGS_ACTION].get_string();
	const string cno = args[OPT_ARGS_CNO].get_string();
	const string stu = args[OPT_ARGS_STU].get_string();
	const string file = args[OPT_ARGS_FILE].get_string();
	const int chapter = args[OPT_ARGS_CHAPTER].get_int();
	const int week = args[OPT_ARGS_WEEK].get_int();
	const string display = args[OPT_ARGS_DISPLAY].get_string();
	const string cfgfile = args[OPT_ARGS_CFGFILE].get_string();
	// 检查
	hw_check stu_check(action, cno, stu, file, cfgfile, chapter, week);
	//hw_check stu_check("base", cno, "all", "all", cfgfile, chapter, week);
	//hw_check stu_check("firstline", cno, "all", "all", cfgfile, chapter, week);
	//hw_check stu_check("secondline", cno, "all", "17-b1-main.cpp", cfgfile, chapter, week);
	stu_check.check();
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
	const string Action_Name[] = {"base","firstline","secondline",""}; //也可以根据需要，放入头文件中以便共享 

	args_analyse_tools args[] = {
		args_analyse_tools("--help",     ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--debug",    ST_EXTARGS_TYPE::boolean,            0, false),
		args_analyse_tools("--action",   ST_EXTARGS_TYPE::str_with_set_error, 1, -1, Action_Name),	//参数-1无意义，表示无默认，必须指定
		args_analyse_tools("--cno",      ST_EXTARGS_TYPE::str,                1, string("")),
		args_analyse_tools("--stu",      ST_EXTARGS_TYPE::str,                1, string("")),
		args_analyse_tools("--file",     ST_EXTARGS_TYPE::str,                1, string("")),
		args_analyse_tools("--chapter",  ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 99), //参数-1表示不进行章节选择
		args_analyse_tools("--week",     ST_EXTARGS_TYPE::int_with_error,     1, -1, -1, 20), //参数-1表示不进行周次选择
		args_analyse_tools("--display",  ST_EXTARGS_TYPE::str,                1, string("11111")),
		args_analyse_tools("--cfgfile",  ST_EXTARGS_TYPE::str,                1, string("hw_check.conf")),
		args_analyse_tools()  //最后一个，用于结束
	};
	int cur_argc, ret = 0;

	//最后一个参数1，表示除选项参数外，还有其它参数
	if ((cur_argc = args_analyse_process(argc, argv, args, 0)) < 0) {
		//错误信息在函数中已打印
		//args_analyse_print(args);
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

	//必选项
	if (!args[OPT_ARGS_ACTION].existed()) {
		usage(argv[0]);
		cout << "参数[" << args[OPT_ARGS_ACTION].get_name() << "]必须指定." << endl;
		return -1;
	}
	if (!args[OPT_ARGS_CNO].existed()) {
		usage(argv[0]);
		cout << "参数[" << args[OPT_ARGS_CNO].get_name() << "]必须指定." << endl;
		return -1;
	}
	if (!args[OPT_ARGS_STU].existed()) {
		usage(argv[0]);
		cout << "参数[" << args[OPT_ARGS_STU].get_name() << "]必须指定." << endl;
		return -1;
	}
	if (!args[OPT_ARGS_FILE].existed()) {
		usage(argv[0]);
		cout << "参数[" << args[OPT_ARGS_FILE].get_name() << "]必须指定." << endl;
		return -1;
	}
	
	//参数要求
	if ((args[OPT_ARGS_CHAPTER].existed() || args[OPT_ARGS_WEEK].existed()) && args[OPT_ARGS_FILE].get_string() != "all") {
		usage(argv[0]);
		cout << "参数[" << args[OPT_ARGS_CHAPTER].get_name() << '/' << args[OPT_ARGS_WEEK].get_name() << "]不能出现在[" << args[OPT_ARGS_FILE].get_name() << " 单个文件名]时." << endl;
		return -1;
	}
	if (args[OPT_ARGS_DISPLAY].existed()) {
		string display = args[OPT_ARGS_DISPLAY].get_string();
		bool correct = (5 == display.size());
		if (correct)
			for (char ch : display)
				if (ch != '0' && ch != '1') {
					correct = false;
					break;
				}
		if (!correct) {
			usage(argv[0]);
			cout << "参数[" << args[OPT_ARGS_FILE].get_name() << "]的长度不正确，只能是[5]位的0/1." << endl;
			return -1;
		}
	}

	//冲突报错
	
	//Debug模式
	if (args[OPT_ARGS_DEBUG].existed())
		args_analyse_print(args);

	/* 进入实际的功能调用，完成相应的功能 */
	hw_check_fun(args);


	return 0;
}

