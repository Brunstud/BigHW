#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//按需加入需要的头文件 
#include <sstream>
#include <fstream>
#include <iomanip>
#include "../include/class_aat.h"
#include "../include/cmd_console_tools.h"
using namespace std;

enum OPT_ARGS {
	OPT_ARGS_HELP = 0,
	OPT_ARGS_DEBUG,
	OPT_ARGS_FILE1,
	OPT_ARGS_FILE2,
	OPT_ARGS_TRIM,
	OPT_ARGS_LINE_SKIP,
	OPT_ARGS_LINE_OFFSET,
	OPT_ARGS_IGNORE_BLANK,
	OPT_ARGS_CR_CRLF_NOT_EQUAL,
	OPT_ARGS_MAX_DIFFNUM,
	OPT_ARGS_MAX_LINENUM,
	OPT_ARGS_DISPLAY
};

/***************************************************************************
  函数名称：
  功    能：
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

	msg << "Usage: " << procname << " --file1 xxx --file2 xxx [ --trim none/left/rigth/all | --lineskip 0..100 | --lineoffset -100..100 | --ignore_blank | --max_diff 0..100 | --display none/normal/detailed ]" << endl;

	msg << setw(wkey) << ' ' << "必选项：" << endl;
	msg << setw(wopt) << ' ' << "--file1 name1              : 指定要比较的第1个文件名（必选）" << endl;
	msg << setw(wopt) << ' ' << "--file2 name2              : 指定要比较的第2个文件名（必选）" << endl;
	msg << endl;

	msg << setw(wkey) << ' ' << "可选项：" << endl;
	msg << setw(wopt) << ' ' << "--trim none/left/right/all     : 指定每行的空格/tab的忽略方式（无此项则默认为none）" << endl;
	msg << setw(wopt) << ' ' << "\tnone   : 每行均严格匹配" << endl;
	msg << setw(wopt) << ' ' << "\tleft   : 每行均忽略头部空格" << endl;
	msg << setw(wopt) << ' ' << "\tright  : 每行均忽略尾部空格" << endl;
	msg << setw(wopt) << ' ' << "\tall    : 每行均忽略头尾空格" << endl;
	msg << setw(wopt) << ' ' << "--lineskip s                   : 指定文件的前s行不比较（无此项则s为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..100]，表示同时跳过两个文件的前s行" << endl;
	msg << setw(wopt) << ' ' << "--lineoffset n                 : 指定文件错位的行数（无此项则n为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[-100..100]，负数表示忽略file1的前n行，正数表示忽略file2的前n行" << endl;
	msg << setw(wopt) << ' ' << "--ignore_blank                 : 忽略文件中--trim后的所有空行(无此项则不忽略)" << endl;
	msg << setw(wopt) << ' ' << "--CR_CRLF_not_equal            : 不忽略Windows/Linux文件的换行符差异(无此项则忽略差异)" << endl;
	msg << setw(wopt) << ' ' << "--max_diff m                   : 指定文件错位的行数（无此项则m为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..100]，表示满m个不同行则结束运行，不再进行后续的比较" << endl;
	msg << setw(wopt) << ' ' << "--max_line x                   : 指定文件比较的最大行数（无此项则x为0）" << endl;
	msg << setw(wopt) << ' ' << "\t取值[0..10000]，表示比较x行后则结束运行，不再进行后续的比较" << endl;
	msg << setw(wopt) << ' ' << "--display none/normal/detailed : 指定显示的信息（无此项则默认为none）" << endl;
	msg << setw(wopt) << ' ' << "\tnone     : 仅一行匹配与否的提示" << endl;
	msg << setw(wopt) << ' ' << "\tnormal   : 每个差异行给出差异信息" << endl;
	msg << setw(wopt) << ' ' << "\tdetailed : 每个差异行给出更详细的差异信息" << endl;
	msg << endl;

	msg << "e.g.   " << procname << " --file1 my.txt --file2 std.txt" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim all" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，每行均去除头尾空格，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineoffset -2" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略my.txt的前2行(即my.txt的第3行与std.txt的第1行进行比较，每行严格匹配)，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim left --lineoffset 3" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略std.txt的前3行(即my.txt的第1行与std.txt的第4行进行比较)，每行去除头部空格，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略文件中的所有空行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineskip 1" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，第一行不比较" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank --trim right" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，忽略文件中去除头尾空格后的所有空行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_diff 3" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，有3行不匹配后结束运行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_line 7" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，仅比较前7行，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --CR_CRLF_not_queal" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，不忽略Windows/Linux的文件差异，仅一行输出" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display normal" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，不匹配的行显示信息" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display detailed" << endl;
	msg << setw(wopt) << ' ' << " : 比较my.txt和std.txt，全部匹配，不匹配的行显示详细信息" << endl;
	msg << endl;

	/* 一次全部显示出来 */ 
	cout << msg.str() << endl;
}

/***************************************************************************
  函数名称：trim
  功    能：对字符串按要求进行trim（掐头去尾）处理
  输入参数：string& str : 需要trim处理的字符串
			const string& trim_type : trim类型
			const bool CR_CRLF_not_equal : 是否区分Windows/Linux格式
  返 回 值：处理后的str
  说    明：
***************************************************************************/
string trim(string& str, const string &trim_type, const bool CR_CRLF_not_equal = false) {
	unsigned int first = 0, last = str.length();
	// 找到第一个非空格字符的位置
	if (trim_type == "left" || trim_type == "all") {
		first = str.find_first_not_of(" \t");
		if (!CR_CRLF_not_equal)
			first = str.find_last_not_of(" \t\r");
		if (first == string::npos) {
			// 如果字符串全是空格，则返回空字符串
			return (str = "");
		}
	}
	//去除\r
	if (!CR_CRLF_not_equal)
		last = str.find_last_not_of("\r");
	// 找到最后一个非空格字符的位置
	if (trim_type == "right" || trim_type == "all")
		last = str.find_last_not_of(" \t\r");
	// 返回去除前后空格的子字符串
	return (str = str.substr(first, last - first + 1));
}

/***************************************************************************
  函数名称：findFirstDifference
  功    能：比较两个字符串并找出它们之间在第几个字符开始出现差异
  输入参数：onst string& str1, const string& str2 : 需要比对的字符串
  返 回 值：返回第一个差异字符的位置（从1开始）
  说    明：当某个字符串末尾有冗余时：
			str1较长：返回-1
			str2较长：返回-2
			两串相等：返回0（不应该存在）
***************************************************************************/
int findFirstDifference(const string& str1, const string& str2) {
	// 获取两个字符串中较短的长度
	unsigned int minLength = min(str1.length(), str2.length());

	// 遍历两个字符串，寻找第一个不同的字符位置
	for (unsigned int i = 0; i < minLength; ++i) {
		if (str1[i] != str2[i]) {
			return (int)(i + 1); // 返回第一个不同字符的位置（从1开始）
		}
	}

	// 若到达此处，说明前面的字符都相同，返回那个字符串较长
	if (str1.length() == str2.length())
		return 0;
	return (str1.length() > str2.length() ? -1 : -2);
}

/***************************************************************************
  函数名称：str2hex
  功    能：16进制查看字符串
  输入参数：const string &str : 需要转换的字符串
			const bool eof : 标记eof
			const string &prompt : 提示词，默认无
  返 回 值：返回第一个差异字符的位置（从1开始）
  说    明：当某个字符串末尾有冗余时：
			str1较长：返回-1
			str2较长：返回-2
			两串相等：返回0（不应该存在）
***************************************************************************/
void str2hex(const string &str, const bool eof, const string &prompt = "")
{
	if (prompt != "")
		cout << prompt << endl;
	if (eof) {
		cout << "<EOF>" << endl;
		return;
	}
	else if (str == "") {
		cout << "<EMPTY>" << endl;
		return;
	}
	istringstream instr(str);
	int end = 0;//标注文档结束
	unsigned char ch = instr.get();
	cout << hex; //<< setiosflags(ios::uppercase);
	for (unsigned int pos = 0; !instr.eof(); pos += 16) {
		cout << setw(8) << setfill('0') << right << pos << " : " << setw(2) << setfill('0') << (unsigned int)ch << ' ';
		for (int i = 1; i < 8; i++) {
			ch = instr.get();
			if (!instr.eof())
				cout << setw(2) << setfill('0') << (unsigned int)ch << ' ';
			else {
				cout << setw(3 * (16 - i) + 2) << setfill(' ') << ' ';
				end = 1;
				break;
			}
		}
		if (!end) {
			ch = instr.get();
			if (!instr.eof())
				cout << "- " << setw(2) << setfill('0') << (unsigned int)ch << ' ';
			else
				cout << setw(5) << setfill(' ') << ' ';
		}
		for (int i = 1; !end && i < 8; i++) {
			ch = instr.get();
			if (!instr.eof())
				cout << setw(2) << setfill('0') << (unsigned int)ch << ' ';
			else {
				cout << setw(3 * (8 - i)) << setfill(' ') << ' ';
				end = 1;
				break;
			}
		}
		cout << ' ';
		if (end)
			instr.clear();
		instr.seekg(pos, ios::beg);
		ch = instr.get();
		for (int i = 0; !instr.eof() && i < 16; i++) {
			if (ch >= 33 && ch <= 126 || ch == ' ')
				cout << ch;
			else
				cout << '.';
			ch = instr.get();
		}
		cout << endl;
	}
	cout << dec;
	return;
}

/***************************************************************************
  函数名称：show_both_line
  功    能：利用gotoxy同时标记两行
  输入参数：const string& line1 : 显示该行
			const string& line2 : 对比该行
			const int eof : 标记eof
  返 回 值：
  说    明：
***************************************************************************/
void show_both_line(const string& prompt, const string& line1, const string& line2, const int eof)
{
	int x, y;//标记位置
	cout << "文件1 : ";
	cct_getxy(x, y);
	cct_gotoxy(0, y + 1);
	cout << "文件2 : ";
	x++;
	unsigned int len = min(line1.length(), line2.length());
	for (unsigned int i = 0; i < len; ++i) {
		if (line1[i] != line2[i]) {
			cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
			cct_gotoxy(x, y);
			cout << (line1[i] != '\r' ? line1[i] : 'X');
			cct_gotoxy(x, y + 1);
			cout << (line2[i] != '\r' ? line2[i] : 'X');
			cct_setcolor();
		}
		else if (line1[i] < 0 && line1[i + 1] != line2[i + 1]) {
			// 处理多字符编码的情况，一个汉字占两个字符位置
			char ch1 = line1[i++];
			cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
			cct_gotoxy(x, y);
			cout << ch1 << line1[i];
			cct_gotoxy(x, y + 1);
			cout << ch1 << line2[i];
			cct_setcolor();
		}
		else {  //无差异，普通打印
			cct_gotoxy(x, y);
			cout << (line1[i] != '\r' ? line1[i] : 'X');
			cct_gotoxy(x, y + 1);
			cout << (line2[i] != '\r' ? line2[i] : 'X');
		}
		x++;//向后一位打印
	}
	cct_gotoxy(x, y);
	if (line1.length() == 0) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
		if (eof == 1)
			cout << "<EOF>";
		else {
			cout << "<EMPTY>";
		}
		cct_setcolor();
	}
	if (line1.length() > line2.length()) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
		for (int i = len; i < line1.length(); i++)
			cout << (line1[i] != '\r' ? line1[i] : 'X');
		//cout << line1.substr(len);
		cct_setcolor();
	}
	cout << (eof == 1 ? "" : "<CR>");
	cct_gotoxy(x, y + 1);
	if (line2.length() == 0) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
		if (eof == 2)
			cout << "<EOF>";
		else {
			cout << "<EMPTY>";
		}
		cct_setcolor();
	}
	if (line2.length() > line1.length()) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
		for (int i = len; i < line2.length(); i++)
			cout << (line2[i] != '\r' ? line2[i] : 'X');
		//cout << line2.substr(len);
		cct_setcolor();
	}
	cout << (eof == 2 ? "" : "<CR>") << endl;
}

/***************************************************************************
  函数名称：show_line
  功    能：标记差异字符显示一行
  输入参数：const string &prompt : 开头提示
			const string& line1 : 显示该行
			const string& line2 : 对比该行
			const bool eof : 标记eof
  返 回 值：
  说    明：
***************************************************************************/
void show_line(const string &prompt ,const string& line1, const string& line2, const bool eof)
{
	cout << prompt;
	unsigned int len = min(line1.length(), line2.length());
	for (unsigned int i = 0; i < len; ++i) {
		if (line1[i] != line2[i]) {
			cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
			cout << (line1[i] != '\r' ? line1[i] : 'X');
			cct_setcolor();
		}
		else if (line1[i] > 0xc2 && line1[i] < 0xdf && line1[i + 1] != line2[i + 1]) {
			// 处理多字符编码的情况，一个汉字占两个字符位置
			char ch1 = line1[i++];
			cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
			cout << ch1 << (line1[i] != '\r' ? line1[i] : 'X');
			cct_setcolor();
		}
		else {  //无差异，普通打印
			cout << (line1[i] != '\r' ? line1[i] : 'X');
		}
	}
	if (line1.length() == 0) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
		if (eof == 1)
			cout << "<EOF>";
		else {
			cout << "<EMPTY>";
		}
		cct_setcolor();
	}
	if (line1.length() > line2.length()) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//高亮标记差异字符
		for (int i = len; i < line1.length(); i++)
			cout << (line1[i] != '\r' ? line1[i] : 'X');
		//cout << line1.substr(len);
		cct_setcolor();
	}
	cout << (eof ? "" : "<CR>") << endl;
	return;
}

/***************************************************************************
  函数名称：display_line
  功    能：显示一行
  输入参数：const string& line1, const string& line2 : 该行字符
			const int lineNum1, const int lineNum2   : 第几行
			const int eof : 标记eof的文件
			const string& display : 显示模式
  返 回 值：两字符串不相等返回true
  说    明：
***************************************************************************/
bool display_line(const string& line1, const string& line2, const int lineNum1, const int lineNum2, const int eof, const string& display = "none", const string& trim_type = "none")
{
	const int diff_pos = findFirstDifference(line1, line2);
	if (!diff_pos && !eof) //两串相等直接结束
		return false;
	if (display == "none")//不显示直接返回结果
		return true;
	//行数标记
	cout << "第[" << lineNum1 << " / " << lineNum2 << "]行 - ";
	if (!diff_pos && eof) {
		cout << "文件1" << (eof == 1 ? "已结束" : "仍有内容") << "/文件2" << (eof == 2 ? "已结束" : "仍有内容") << (trim_type != "none" ? "." : "：") << endl;
		if (trim_type != "none")
			return true;
	}
	else if (diff_pos > 0)
		cout << "第[" << diff_pos - 1 << "]个字符开始有差异：" << endl;
	else
		cout << "文件" << -diff_pos << "的尾部有多余字符：" << endl;
	//补充打印刻度尺
	if (display == "detailed") {//详细打印
		const unsigned int max_len = max(line1.length(), line2.length());
		const unsigned int times = (max_len + 2) / 10 + 1;
		cout << setw(8) << ' ' << setw(10 * times) << setfill('-') << '-' << endl << setfill(' ');
		cout << setw(8) << ' ';
		for (int i = 0; i < times; i++)
			cout << setw(10) << setfill(' ') << left << i;
		cout << endl;
		cout << setw(8) << ' ';
		for (int i = 0; i < times; i++)
			for (int j = 0; j < 10; j++)
				cout << j;
		cout << endl;
		cout << setw(8) << ' ' << setw(10 * times) << setfill('-') << '-' << endl << setfill(' ');
	}
	//错误彩色打印
	show_line("文件1 : ", line1, line2, eof == 1);
	show_line("文件2 : ", line2, line1, eof == 2);
	//补充打印hex
	if (display == "detailed") {
		str2hex(line1, (eof == 1), "文件1(HEX) : ");
		str2hex(line2, (eof == 2), "文件2(HEX) : ");
	}
	//完成打印
	cout << endl;
	return true;
}

/***************************************************************************
  函数名称：txt_compare
  功    能：文件比对核心函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void txt_compare(args_analyse_tools args[])
{
	ifstream infile1(args[OPT_ARGS_FILE1].get_string(), ios::in | ios::binary);
	ifstream infile2(args[OPT_ARGS_FILE2].get_string(), ios::in | ios::binary);
	if (!infile1.is_open() || !infile2.is_open()) {
		cerr << "打开文件失败" << endl;
		return;
	}
	//参数配置
	const int lineskip = args[OPT_ARGS_LINE_SKIP].get_int();//ok
	const int lineoffset = args[OPT_ARGS_LINE_OFFSET].get_int();//ok
	const string trim_type = args[OPT_ARGS_TRIM].get_string();//ok
	const bool ignore_blank = args[OPT_ARGS_IGNORE_BLANK].existed();//ok
	const bool CR_CRLF_not_equal = args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed();
	const int max_diff = args[OPT_ARGS_MAX_DIFFNUM].get_int();//ok
	const int max_line = args[OPT_ARGS_MAX_LINENUM].get_int();//ok
	const string display = args[OPT_ARGS_DISPLAY].get_string();//ok
	//开始比对
	string line1, line2;
	int lineNum1 = 0, lineNum2 = 0;
	bool areDifferent = false;
	//跳过开头
	for (int i = 0; i < lineskip + (lineoffset < 0 ? -lineoffset : 0); ) {
		getline(infile1, line1);
		trim(line1, trim_type);//处理trim
		lineNum1++;
		if (ignore_blank && !line1[0])
			continue;
		i++;
	}
	for (int i = 0; i < lineskip + (lineoffset > 0 ? lineoffset : 0); ) {
		getline(infile2, line2);
		trim(line2, trim_type);//处理trim
		lineNum2++;
		if (ignore_blank && !line2[0])
			continue;
		i++;
	}
	//打印提示
	if (display != "none") {
		cout << "比较结果输出：" << endl;
		cout << "====================================================================================================" << endl;
	}
	//开始比对
	int times = 0, difftimes = 0, eof = 0;
	while (1) {
		if (eof)
			break;
		eof = (infile1.eof() && !infile2.eof() ? 1 : (infile2.eof() && !infile1.eof() ? 2 : 0));
		//读行
		do {
			getline(infile1, line1);
			trim(line1, trim_type, CR_CRLF_not_equal);//处理trim
			lineNum1++;
		} while (ignore_blank && !line1.length() && !infile1.eof());
		if (infile1.eof() && !eof && !line1.length())
			eof = 1;
		do {
			getline(infile2, line2);
			trim(line2, trim_type, CR_CRLF_not_equal);//处理trim
			lineNum2++;
		} while (ignore_blank && !line2.length() && !infile2.eof());
		if (infile2.eof() && !eof && !line2.length())
			eof = 2;
		if (infile1.eof() && infile2.eof())
			break;
		//输出结果
		if (display_line(line1, line2, lineNum1, lineNum2, eof, display, trim_type)) {
			areDifferent = true;
			difftimes++;
		}
		times++;
		if (max_line && times >= max_line || max_diff && difftimes >= max_diff)
			break;
	}
	infile1.close();
	infile2.close();
	//最终判断
	if (display == "none") {
		if (areDifferent)
			cout << "文件不同." << endl;
		else
			cout << "文件相同." << endl;
	}
	else {
		if (areDifferent) {
			cout << "====================================================================================================" << endl;
			cout << "在指定检查条件下共" << difftimes << "行有差异." << endl;
			cout << "阅读提示：" << endl;
			cout << "	1、空行用<EMPTY>标出" << endl;
			cout << "	2、文件结束用<EOF>标出" << endl;
			cout << "	3、两行相同列位置的差异字符用亮色标出" << endl;
			cout << "	4、每行中的CR/LF/VT/BS/BEL用X标出(方便看清隐含字符)" << endl;
			cout << "	5、每行尾的多余的字符用亮色标出，CR/LF/VT/BS/BEL用亮色X标出(方便看清隐含字符)" << endl;
			cout << "	6、每行最后用<CR>标出(方便看清隐含字符)" << endl;
			cout << "	7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出" << endl;
			if (display != "detailed")
				cout << "	8、用--display detailed可以得到更详细的信息" << endl;
		}
		else
			cout << "在指定检查条件下完全一致." << endl;
		cout << "====================================================================================================" << endl;
	}
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
	const string TrimType[] = {	"none",	"left",	"right", "all", "" };
	const string DisplayType[] = { "none",	"normal",	"detailed", "" };

	args_analyse_tools args[] = {
		args_analyse_tools("--help",			ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_HELP = 0
		args_analyse_tools("--debug",			ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_DEBUG
		args_analyse_tools("--file1",			ST_EXTARGS_TYPE::str, 1, string("")),					//OPT_ARGS_FILE1
		args_analyse_tools("--file2",			ST_EXTARGS_TYPE::str, 1, string("")),					//OPT_ARGS_FILE2
		args_analyse_tools("--trim",			ST_EXTARGS_TYPE::str_with_set_error, 1, 0, TrimType),	//OPT_ARGS_TRIM
		args_analyse_tools("--lineskip",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 100),		//OPT_ARGS_LINE_SKIP
		args_analyse_tools("--lineoffset",		ST_EXTARGS_TYPE::int_with_default,   1, 0, -100, 100),	//OPT_ARGS_LINE_OFFSET
		args_analyse_tools("--ignore_blank",	ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_IGNORE_BLANK
		args_analyse_tools("--CR_CRLF_not_equal",ST_EXTARGS_TYPE::boolean, 0, false),					//OPT_ARGS_CR_CRLF_NOT_EQUAL
		args_analyse_tools("--max_diff",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 100),		//OPT_ARGS_MAX_DIFFNUM
		args_analyse_tools("--max_line",		ST_EXTARGS_TYPE::int_with_default,   1, 0, 0, 10000),	//OPT_ARGS_MAX_LINENUM
		args_analyse_tools("--display",			ST_EXTARGS_TYPE::str_with_set_error, 1, 0, DisplayType),//OPT_ARGS_DISPLAY
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

	if (args[OPT_ARGS_FILE1].existed() + args[OPT_ARGS_FILE2].existed() != 2) {
		usage(argv[0]);
		cout << "必须指定参数[" << args[OPT_ARGS_FILE1].get_name() << "和"<< args[OPT_ARGS_FILE2].get_name() << "]" << endl;
		return -1;
	}

	string trim_type = args[OPT_ARGS_TRIM].get_string();
	if (args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() && (trim_type == "right" || trim_type == "all")) {
		usage(argv[0]);
		cout << "参数[" << args[OPT_ARGS_CR_CRLF_NOT_EQUAL].get_name() << "]不能和[" << args[OPT_ARGS_TRIM].get_name() << " right/all]同时存在." << endl;
		return -1;
	}

	if (args[OPT_ARGS_DEBUG].existed())
		args_analyse_print(args);

	/* 进入实际的功能调用，完成相应的功能 */ 
	txt_compare(args);

	return 0;
}

/* 
阅读提示：
1、空行用 <EMPTY> 标出
2、文件结束用 <EOF> 标出
3、两行相同列位置的差异字符用亮色标出
4、每行中的CR/LF/VT/BS/BEL用X标出(方便看清隐含字符)
5、每行尾的多余的字符用亮色标出，CR/LF/VT/BS/BEL用亮色X标出(方便看清隐含字符)
6、每行最后用 <CR> 标出(方便看清隐含字符)"
7、中文因为编码问题，差异位置可能报在后半个汉字上，但整个汉字都亮色标出
8、用--display detailed可以得到更详细的信息
*/ 

#if 0
    //分隔行===的宽度计算 
	int width = (this->line_maxlen / 10 + 1) * 10 + 8 + 2; //比---的标尺多2个：line_maxlen向上取10倍整数，8是"文件1 : "

	/* 如果加了hex输出，则最小宽度为80 */
	if (this->display_detailed && width < 80)
		width = 80;
#endif

/* 测试文件准备
	1.txt ：正常文件
	2.txt ：在1.txt的基础上，某些行前面无空格
	3.txt ：在1.txt的基础上，某些行后面无空格
	4.txt ：在1.txt的基础上，最后一行没有回车
	5.txt ：在1.txt的基础上，多一些空行
	6.txt ：在1.txt的基础上，多一些空行，其中某些行有空格和tab
	7.txt ：和1.txt完全相同
	8.txt ：是1.txt的Linux格式版

txt_compare --file1 1.txt --file2 2.txt --trim left
txt_compare --file1 1.txt --file2 2.txt --trim all
txt_compare --file1 2.txt --file2 3.txt --trim all
txt_compare --file1 1.txt --file2 4.txt --trim right --ignore_blank
txt_compare --file1 1.txt --file2 4.txt --trim all --ignore_blank
txt_compare --file1 2.txt --file2 4.txt --trim all --ignore_blank
txt_compare --file1 3.txt --file2 4.txt --trim right --ignore_blank
txt_compare --file1 3.txt --file2 4.txt --trim all --ignore_blank
txt_compare --file1 1.txt --file2 5.txt --trim right --ignore_blank
txt_compare --file1 1.txt --file2 6.txt --ignore_blank --trim right
txt_compare --file1 5.txt --file2 6.txt --ignore_blank --trim all
txt_compare --file1 1.txt --file2 7.txt
txt_compare --file1 1.txt --file2 8.txt

rem 不相同的例子
txt_compare --file1 1.txt --file2 2.txt
txt_compare --file1 1.txt --file2 2.txt --trim right
txt_compare --file1 1.txt --file2 5.txt
txt_compare --file1 1.txt --file2 5.txt --trim right
txt_compare --file1 1.txt --file2 6.txt
txt_compare --file1 1.txt --file2 6.txt --ignore_blank
txt_compare --file1 1.txt --file2 8.txt --CR_CRLF_not_equal

*/
