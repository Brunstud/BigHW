/* 2252201 胡世成 计科 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//按需加入需要的头文件 
#include <sstream>
#include <fstream>
#include <iomanip>
#include <climits>
#include "hw_check.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有
using namespace std;

/***************************************************************************
  函数名称：trim
  功    能：辅助函数：去除字符串首尾空格和制表符
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static inline string& trim(string& str) {
	str.erase(0, str.find_first_not_of(" \t")); // 去除开头空格和制表符
	str.erase(str.find_last_not_of(" \t") + 1); // 去除结尾空格和制表符
	return str;
}

/***************************************************************************
  隶属类名：hw_check_info
  函数名称：
  功    能：构造和析构函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
// 构造函数
hw_check_info::hw_check_info(const char* _cfgname) : cfgfile_read_tools(_cfgname) {
	read_cfgfile();//读取配置文件
	init_mysql();//初始化数据库
}
hw_check_info::hw_check_info(const std::string& _cfgname) : cfgfile_read_tools(_cfgname) {
	read_cfgfile();//读取配置文件
	init_mysql();//初始化数据库
}
// 析构函数
hw_check_info::~hw_check_info() {
	/* 关闭数据库连接 */
	mysql_close(mysql);
}

/***************************************************************************
  隶属类名：hw_check_info
  函数名称：
  功    能：读取配置文件
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hw_check_info::read_cfgfile() {
	if (cfgfile_read_tools::is_open()) {
		if (item_get_value("[文件目录]", "src_rootdir", ".\\23241 - source\\", src_rootdir))
			cerr << "配置文件[文件目录]读取错误" << endl;
		if (item_get_value("[日志目录]", "db_hw_logfile", "hw_check.log", db_hw_logfile) ||
			item_get_value("[日志目录]", "db_hw_logsize", 1048576, 0, INT_MAX, db_hw_logsize))
			cerr << "配置文件[目录日志]读取错误" << endl;
		if (item_get_value("[数据库]", "db_host", "", db_host) ||				// = 10.80.42.244			# 数据库服务器的IP地址（校园网内）
			item_get_value("[数据库]", "db_port", "", db_port) ||				// = 3306					# 数据库服务器的端口号
			item_get_value("[数据库]", "db_name", "1", db_name) ||				// = homework				# 数据库名称
			item_get_value("[数据库]", "db_username", "", db_username) ||		// = hwcheck				# 用户名
			item_get_value("[数据库]", "db_passwd", "", db_passwd) ||			// = hw_CheCk - For23241 * oOP # 用户密码
			item_get_value("[数据库]", "db_currterm", "", db_currterm))			// = 2023 / 2024 / 1           # 当前学期
			cerr << "配置文件[数据库]读取错误" << endl;
	}
	else
		cerr << "无法打开配置文件：" << cfgname << endl;
}

/***************************************************************************
  隶属类名：hw_check_info
  函数名称：
  功    能：初始化及连接数据库
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hw_check_info::init_mysql() {
	/* 设置初值 */
	result = NULL;
	row = NULL;

	/* 初始化 mysql 变量，失败返回NULL */
	if ((mysql = mysql_init(NULL)) == NULL) {
		cerr << "mysql_init failed" << endl;
		return;
	}

	/* 连接数据库，失败返回NULL
		1、mysqld没运行
		2、没有指定名称的数据库存在 */
	if (mysql_real_connect(mysql, db_host.c_str(), db_username.c_str(), db_passwd.c_str(), db_name.c_str(), 0, NULL, 0) == NULL) {
		cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
		return;
	}

	/* 设置字符集，否则读出的字符乱码 */
	mysql_set_character_set(mysql, "gbk");
}

/***************************************************************************
  隶属类名：hw_check_info
  函数名称：
  功    能：判断是否连接数据库
  输入参数：
  返 回 值：
  说    明：数据库查找函数
***************************************************************************/
bool hw_check_info::is_connected() const {
	return (mysql != NULL);
}

/***************************************************************************
  隶属类名：hw_check_info
  函数名称：
  功    能：获取学生信息
  输入参数：
  返 回 值：
  说    明：数据库查找函数
***************************************************************************/
const vector<StuInfo>& hw_check_info::load_stu_info(const string& stu_cno) {
	// 清空原学生信息列表
	stu_list.clear();
	// 判断是否连接数据库
	if (!mysql)
		return stu_list;

	/* 查询命令：
			select * from view_hwcheck_stulist where stu_cno = "xxx"
		含义：表示查询某课程的所有学生信息
		返回值：成功返回0，不成功非0
		不成功的常见可能性：
			1、查询字符串存在语法错误
			2、查询不存在的数据表 */
	//if (mysql_query(mysql, ("select * from view_hwcheck_stulist where stu_cno = \"" + stu_cno + "\"").c_str())) {
	if (mysql_query(mysql, "select * from view_hwcheck_stulist where stu_cno = \"10108001\"")) {
		cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
		return stu_list;//返回空表
	}

	/* 将查询结果存储起来，出现错误则返回NULL
		注意：查询结果为NULL，不会返回NULL */
	if ((result = mysql_store_result(mysql)) == NULL) {
		cout << "mysql_store_result failed" << endl;
		return stu_list;//返回空表
	}

	/* 打印当前查询到的记录的数量 */
	//cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

	/* 循环读取所有满足条件的记录
		2、row的排列顺序与select返回的列顺序相同，本例：
			row[0] <=> term
			row[1] <=> grade
			row[2] <=> sno
			row[3] <=> name
			row[4] <=> sex
			row[5] <=> bmajor
			row[6] <=> major
			row[7] <=> cno	*/
	while ((row = mysql_fetch_row(result)) != NULL) {
		StuInfo stu;
		stu.term = row[0];
		stu.grade = row[1];
		stu.sno = row[2];
		stu.name = row[3];
		stu.sex = row[4];
		stu.bmajor = row[5];
		stu.major = row[6];
		stu.cno = row[7];
		stu_list.push_back(stu);
	}

	/* 释放result，否则会丢失内存 */
	mysql_free_result(result);

	return stu_list;
}

/***************************************************************************
  隶属类名：hw_check_info
  函数名称：
  功    能：获取作业信息
  输入参数：
  返 回 值：
  说    明：数据库查找函数
***************************************************************************/
const vector<HwInfo>& hw_check_info::load_hw_info(const string& hw_cno, int hw_chapter, int hw_week) {
	// 清空原作业信息列表
	hw_list.clear();
	// 判断是否连接数据库
	if (!mysql)
		return hw_list;

	/* 查询命令：
			select * from view_hwcheck_hwlist where hw_cno = "xxx" and hw_chapter = "xxx" and hw_week = "xxx"
		含义：表示查询作业信息
		返回值：成功返回0，不成功非0
		不成功的常见可能性：
			1、查询字符串存在语法错误
			2、查询不存在的数据表 */
	if (-1 == hw_chapter && -1 == hw_week && mysql_query(mysql, ("select * from view_hwcheck_hwlist where hw_cno = \"" + hw_cno + "\"").c_str()) ||
		-1 != hw_chapter && -1 == hw_week && mysql_query(mysql, ("select * from view_hwcheck_hwlist where hw_cno = \"" + hw_cno + "\" and hw_chapter = \"" + to_string(hw_chapter) + "\"").c_str()) ||
		-1 == hw_chapter && -1 != hw_week && mysql_query(mysql, ("select * from view_hwcheck_hwlist where hw_cno = \"" + hw_cno + "\" and hw_week = \"" + to_string(hw_week) + "\"").c_str()) ||
		-1 != hw_chapter && -1 != hw_week && mysql_query(mysql, ("select * from view_hwcheck_hwlist where hw_cno = \"" + hw_cno + "\" and hw_chapter = \"" + to_string(hw_chapter) + "\" and hw_week = \"" + to_string(hw_week) + "\"").c_str())
		) {
		cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
		return hw_list;//返回空表
	}

	/* 将查询结果存储起来，出现错误则返回NULL
		注意：查询结果为NULL，不会返回NULL */
	if ((result = mysql_store_result(mysql)) == NULL) {
		cout << "mysql_store_result failed" << endl;
		return hw_list;//返回空表
	}

	/* 打印当前查询到的记录的数量 */
	//cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

	/* 循环读取所有满足条件的记录
		2、row的排列顺序与select返回的列顺序相同，本例：
			row[0] <=> type
			row[1] <=> cno
			row[2] <=> hw_no
			row[3] <=> chapter
			row[4] <=> week
			row[5] <=> filename
			row[6] <=> score	*/
	while ((row = mysql_fetch_row(result)) != NULL) {
		HwInfo hwork;
		hwork.type = row[0];
		hwork.cno = row[1];
		hwork.hw_no = row[2];
		hwork.chapter = stoi(row[3]);
		hwork.week = stoi(row[4]);
		hwork.filename = row[5];
		hwork.score = row[6];
		hw_list.push_back(hwork);
	}

	/* 释放result，否则会丢失内存 */
	mysql_free_result(result);

	return hw_list;
}

/***************************************************************************
  隶属类名：hw_check_info
  函数名称：
  功    能：从当前的学生名单中找到特定学生姓名
  输入参数：
  返 回 值：
  说    明：未找到则返回空串，即""
***************************************************************************/
const string hw_check_info::get_stu_name(const string& stu_sno) const {
	for (StuInfo stu : stu_list)
		if (stu.sno == stu_sno)
			return stu.name;
	// 未找到，返回空串
	return string("");
}

/***************************************************************************
  隶属类名：hw_check_info
  函数名称：
  功    能：打印学生列表信息
  输入参数：
  返 回 值：
  说    明：Debug函数
***************************************************************************/
void hw_check_info::show_stu_list() const {
	cout << "学生列表：" << endl << left;
	for (StuInfo stu : stu_list) {
		cout << "#" << stu.sno << ' ' << setw(8) << stu.name << " : " << stu.grade << ' ' << stu.cno << ' ' 
			<< stu.sex << ' ' << stu.term << ' ' << stu.major << "（" << stu.bmajor << "） " << endl;
	}
	cout << endl << right;
	return;
}

/***************************************************************************
  隶属类名：hw_check_info
  函数名称：
  功    能：打印作业列表信息
  输入参数：
  返 回 值：
  说    明：Debug函数
***************************************************************************/
void hw_check_info::show_hw_list() const {
	cout << "作业列表：" << endl << left;
	for (HwInfo hw : hw_list) {
		cout << "#" << setw(2) << hw.type << ' ' << setw(27) << hw.filename << " : " 
			<< hw.cno << ' ' << hw.hw_no << ' ' << hw.chapter << ' ' << hw.week << ' ' << hw.score << endl;
	}
	cout << endl << right;
	return;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：构造及析构函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
hw_check_stu::hw_check_stu(const string& _action, const string& _cno, const string& _stu, const string& _hwfile, const string& _cfgfile, int _chapter, int _week)
	: cno(_cno), hwfile(_hwfile), hw_check_info(_cfgfile), chapter(_chapter), week(_week) {
	// 设置检查行为
	if ("base" == _action)
		action = CHECK_BASE;
	else if ("firstline" == _action)
		action = CHECK_FIRSTLINE;
	else if ("secondline" == _action)
		action = CHECK_SECONDLINE;
	else
		action = CHECK_ERROR_TYPE;
	// 从数据库获取信息
	hw_check_info::load_stu_info(cno);
	hw_check_info::load_hw_info(cno, chapter, week);
	// 设置学生检查类型
	if ("all" == _stu)
		stu_type = STU_ALL;
	else {
		stu_type = STU_ONE;
		for (StuInfo pstu : stu_list) {
			if (pstu.sno == _stu) {
				cur_stu = pstu;
				break;
			}
		}
	}
	// display 默认全显示
	display = "11111";
}
hw_check_stu::~hw_check_stu() {};

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：参数设置函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hw_check_stu::set_stu(const string& _stu) {
	if ("all" == _stu) {
		cerr << "注意：hw_check_stu类不支持对所有学生的检查" << endl;
		return;
	}
	// 设置文件夹名
	foldername = src_rootdir + cno + '-' + _stu;
	bool found = false;
	for (StuInfo pstu : stu_list) {
		if (pstu.sno == _stu) {
			cur_stu = pstu;
			found = true;
			break;
		}
	}
	if (!found)
		cur_stu.sno = "";
	cur_res = 0;//结果置零
}
void hw_check_stu::set_display(const string& _display) {
	bool correct = (5 == _display.size());
	if (correct)
		for (char ch : _display)
			if (ch != '0' && ch != '1') {
				correct = false;
				break;
			}
	if (correct) display = _display;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：检查文件是否是有效的UTF-8编码
  输入参数：
  返 回 值：是返回true，否/打不开文件返回false
  说    明：
***************************************************************************/
bool hw_check_stu::isValidUTF8(const string& str) const
{
	int encodingBytesCount = 0;
	bool allTextsAreASCIIChars = true;

	for (char current : str)
	{
		//判断是否都是ASCII字符，当包括中文字符时allTextsAreASCIIChars为false
		if ((current & 0x80) == 0x80)
			allTextsAreASCIIChars = false;

		//判断是否为一个字符的开始字节
		if (encodingBytesCount == 0)
		{
			if ((current & 0x80) == 0)
			{
				// ASCII chars, from 0x00-0x7F
				continue;
			}

			if ((current & 0xC0) == 0xC0)	///大于等于两字节
			{
				encodingBytesCount = 1;
				current <<= 2;

				// More than two bytes used to encoding a unicode char.
				// Calculate the real length.
				while ((current & 0x80) == 0x80)
				{
					current <<= 1;	//判断下一位是否为1
					encodingBytesCount++;	//当前字符编码字节数
				}
			}
			else
			{
				// Invalid bits structure for UTF8 encoding rule.
				return false;
			}
		}
		else
		{
			// Following bytes, must start with 10.
			if ((current & 0xC0) == 0x80)	///当前字节是否以10开头
			{
				encodingBytesCount--;
			}
			else
			{
				return false;
			}
		}
	}
	if (encodingBytesCount != 0)
	{
		// Invalid bits structure for UTF8 encoding rule.
		// Wrong following bytes count.
		return false;
	}
	return !allTextsAreASCIIChars;
}
/*{
	int c, i, ix, n, j;
	for (i = 0, ix = str.length(); i < ix; i++) {
		c = (unsigned char)str[i];
		// 单字节或ASCII字符
		if (c <= 0x7f) continue;
		// 非起始字节
		if ((c & 0xC0) == 0x80) return false;
		// 多字节字符
		if ((c & 0xE0) == 0xC0) n = 1; // 2字节字符
		else if ((c & 0xF0) == 0xE0) n = 2; // 3字节字符
		else if ((c & 0xF8) == 0xF0) n = 3; // 4字节字符
		else return false;
		// 检查多字节字符的后续字节
		for (j = 0; j < n; j++) {
			if ((++i == ix) || ((str[i] & 0xC0) != 0x80))
				return false;
		}
	}
	return true;
}*/

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：检查文件是否是GB编码
  输入参数：
  返 回 值：是返回true，否/打不开文件返回false
  说    明：
***************************************************************************/
bool hw_check_stu::isGBEncoded(ifstream& file) const {
	if (!file) {
		return false; // 文件无法打开或已经到达文件末尾
	}
	// 获取当前文件指针的位置
	streampos currentPosition = file.tellg();

	//将文件指针移到开头
	file.seekg(3, ios::beg);
	// 读取文件内容
	string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	// 重新将文件指针定位到之前存储的位置
	file.seekg(currentPosition);
	// 不满足UTF-8编码，则认为是GB编码
	return !isValidUTF8(content);
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：检查文件是否是PDF格式
  输入参数：
  返 回 值：是返回true，否/打不开文件返回false
  说    明：
***************************************************************************/
bool hw_check_stu::isPDF(std::ifstream& file) const {
	if (!file) {
		return false; // 文件无法打开或已经到达文件末尾
	}
	// 获取当前文件指针的位置
	streampos currentPosition = file.tellg();
	
	//将文件指针移到开头
	file.seekg(0, ios::beg);
	// 读取文件的前4个字节
	char buffer[5] = { 0 };
	file.read(buffer, 4);

	// 重新将文件指针定位到之前存储的位置
	file.seekg(currentPosition);
	// 检查是否为 "%PDF"
	return string(buffer) == "%PDF";
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：isComment
  功    能：判断改行是不是注释
  输入参数：
  返 回 值：
  说    明：顺便去掉注释符
***************************************************************************/
bool hw_check_stu::isComment(string& line) const {
	line.erase(0, line.find_first_not_of(" \t")); // 去除开头空格和制表符
	line.erase(line.find_last_not_of(" \t") + 1); // 去除结尾空格和制表符
	if ('/' == line[0] && '*' == line[1]) {
		line.erase(0, 2); // 删除前两个字符
		line.erase(line.find_last_not_of("/* \t") + 1); // 去除结尾注释符（如果有的话）
		return true;
	}
	else if ('/' == line[0] && '/' == line[1]) {
		line.erase(0, 2); // 删除前两个字符
		return true;
	}
	return false;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：judge_firstline
  功    能：判断第一行
  输入参数：
  返 回 值：0:正确，1:首行不是注释行，2:首行不是三项，3:首行检查出错
  说    明：
***************************************************************************/
int hw_check_stu::judge_firstline(const string& _line) {
	string line = _line;
	if (!isComment(line)) {
		cout << "首行不是注释行 [" << _line << "]" << endl;
		cur_res.fir_notComment++;
		return 1;
	}
	istringstream sin(line);
	string buffer;
	bool name = false, sno = false, dept = false;
	for (int i = 0; i < 3; ++i) {
		sin >> buffer;
		if (!sin) {
			cout << "首行不是三项 [" << _line << "]" << endl;
			cur_res.fir_notThreeItem++;
			return 2;
		}
		if (!name && buffer == cur_stu.name) {
			name = true;
			continue;
		}
		if (!sno && buffer == cur_stu.sno) {
			sno = true;
			continue;
		}
		if (!dept && (buffer == cur_stu.major || buffer == cur_stu.bmajor)) {
			dept = true;
			continue;
		}
	}
	if (!name || !sno || !dept) {
		cout << "首行" << (sno ? "" : "学号不匹配 ") << (name ? "" : "姓名不匹配 ") << (dept ? "" : "班级不匹配 ") << " [" << _line << "]" << endl;
		cur_res.fir_error++;
		return 3;
	}
	cout << "正确" << endl;
	cur_res.correct++;
	return 0;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：judge_secondline
  功    能：判断第一行
  输入参数：
  返 回 值：0:正确，1:次行不是注释行，2:后续内容忽略，3:写了自己
  说    明：
***************************************************************************/
int hw_check_stu::judge_secondline(const string& _line) {
	string line = _line;
	if (!isComment(line)) {
		cout << "次行不是注释行" << endl;// [" << _line << "]" << endl;
		cur_res.sec_notComment++;
		return 1;
	}
	cur_res.correct++;
	istringstream sin(line);
	string buffer, buffer2;
	int i = 0;
	while (sin >> buffer) {
		sin >> buffer2;
		if (!sin) {
			cout << "第[" << i << "]个学生后面的信息不全(只读到一项)，后续内容忽略" << endl;
			return 2;
		}
		else if (buffer.size() != 7) {
			cout << "第" << i + 1 << "位同学的学号[" << buffer << "]不是7位，后续内容忽略" << endl;
			return 2;
		}
		else if (buffer == cur_stu.sno) {
			cout << "第[" << i + 1 << "]项写了自己" << endl;
			return 3;
		}
	}
	cout << "正确" << endl;
	return 0;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：.cpp/.hpp/.h/.c结尾的文件数量
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
int hw_check_stu::codefile_num() {
	int num = 0;
	for (HwInfo hw : hw_list) {
		string thisfile = hw.filename;
		string suffix = ""; //获得文件名后缀
		if (thisfile.length() >= 3)
			suffix = thisfile.substr(thisfile.length() - 3);
		// 找出需要查行的.cpp/.h/.hpp/.c文件
		if (("cpp" == suffix || "hpp" == suffix || 'h' == suffix[2] || 'c' == suffix[2]))
			num++;
	}
	return num;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：用表格显示统计结果
  输入参数：prompt：提示词 _res：统计结果 cutline：分割线字符('-'或'=')
  返 回 值：
  说    明：
***************************************************************************/
void hw_check_stu::print_result(const char* prompt, const CheckResult& _res, char cutline) const {
	// 找到最长结果设定宽度
	int width = max(max(max((_res.correct ? 2 + 4 + 3 : 0), (_res.notSubmitted ? 2 + 6 + 3 : 0)), max((_res.notGBEncoder ? 2 + 26 + 3 : 0), (_res.notPDF ? 2 + 11 + 3 : 0))),
		max(max((_res.fir_notComment ? 2 + 12 + 3 : 0), (_res.fir_notThreeItem ? 2 + 12 + 3 : 0)), max((_res.fir_error ? 2 + 12 + 3 : 0), (_res.sec_notComment ? 2 + 12 + 3 : 0))));
	//打印表格
	cout << setw(width + 7) << setfill(cutline) << cutline << endl << setfill(' ');
	if (prompt) cout << prompt << endl;
	cout << setw(width + 7) << setfill(cutline) << cutline << endl << setfill(' ');
	if (_res.correct) cout << setw(width) << "正确 : " << _res.correct << endl;
	if (_res.notSubmitted) cout << setw(width) << "未提交 : " << _res.notSubmitted << endl;
	if (_res.notGBEncoder) cout << setw(width) << "源文件格式不正确(非GB编码) : " << _res.notGBEncoder << endl;
	if (_res.notPDF) cout << setw(width) << "不是PDF文件 : " << _res.notPDF << endl;
	if (_res.fir_notComment) cout << setw(width) << "首行不是注释 : " << _res.fir_notComment << endl;
	if (_res.fir_notThreeItem) cout << setw(width) << "首行不是三项 : " << _res.fir_notThreeItem << endl;
	if (_res.fir_error) cout << setw(width) << "首行检查出错 : " << _res.fir_error << endl;
	if (_res.sec_notComment) cout << setw(width) << "次行不是注释 : " << _res.sec_notComment << endl;
	cout << setw(width + 7) << setfill(cutline) << cutline << endl << setfill(' ');
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：基础检查（是否提交、pdf文件格式是否正确、源程序文件是否GB编码）
  输入参数：
  返 回 值：成功返回0，失败返回-1
  说    明：内部函数
***************************************************************************/
int hw_check_stu::check_base_inner(const string& _filename) {
	cur_res.total++;
	string thisfile = _filename;
	string suffix = ""; //获得文件名后缀
	if (thisfile.length() >= 3)
		suffix = thisfile.substr(thisfile.length() - 3);
	else {
		cout << "文件名错误：长度不足三个字符" << endl;
		return -1;
	}
	//打印结果
	if (hwfile == "all")
		cout << "  " << left << setw(27) << thisfile << " : " << right;
	//打开文件
	string filename = foldername + '/' + thisfile;
	ifstream file(filename, ios::in);
	if (!file.is_open()) {
		cout << "未提交";
		cur_res.notSubmitted++;
	}
	else if ("pdf" == suffix && !isPDF(file)) {
		cout << "不是PDF格式";
		cur_res.notPDF++;
	}
	else if (("cpp" == suffix || "hpp" == suffix || 'h' == suffix[2] || 'c' == suffix[2]) && !isGBEncoded(file)) {
		cout << "源文件格式不正确（非GB编码）";
		cur_res.notGBEncoder++;
	}
	else {
		cout << "正确";
		cur_res.correct++;
	}
	cout << endl;
	return 0;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：基础检查（是否提交、pdf文件格式是否正确、源程序文件是否GB编码）
  输入参数：
  返 回 值：成功返回0，失败返回-1
  说    明：
***************************************************************************/
int hw_check_stu::check_base() {
	if ("all" == hwfile) {
		cout << "学号-" << cur_stu.sno << " 姓名-" << cur_stu.name << " 课号-" << cur_stu.cno << " 文件数量-" << hw_list.size() << endl;
		string filename = "";
		for (unsigned int i = 0; i < hw_list.size(); ++i) {
			if (check_base_inner(hw_list[i].filename))
				return -1;
		}
	}
	else {
		cout << cur_stu.sno << '/' << left << setw(8) << cur_stu.name << " ： " << right;
		if (check_base_inner(hwfile))
			return -1;
	}
	// 打印统计结果
	//print_total();
	return 0;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：首行检查（首行是否按学号/姓名/专业简称三项，空格分隔，次序不限）
  输入参数：
  返 回 值：成功返回0，失败返回-1
  说    明：
***************************************************************************/
int hw_check_stu::check_line_inner(const string& _filename, int what_line) {
	string thisfile = _filename;
	string suffix = ""; //获得文件名后缀
	if (thisfile.length() >= 3)
		suffix = thisfile.substr(thisfile.length() - 3);
	else {
		cout << "文件名错误：长度不足三个字符" << endl;
		return -1;
	}
	// 找出需要查行的.cpp/.h/.hpp/.c文件
	if (("cpp" == suffix || "hpp" == suffix || 'h' == suffix[2] || 'c' == suffix[2])) {
		cur_res.total++;
		//打印结果
		if (hwfile == "all")
			cout << "  " << left << setw(27) << thisfile << " : " << right;
		//打开文件
		string filename = foldername + '/' + thisfile;
		ifstream file(filename, ios::in);
		if (!file.is_open()) {
			cout << "未提交" << endl;
			cur_res.notSubmitted++;
			return -1;
		}
		if (!isGBEncoded(file)) {
			cout << "源文件格式不正确（非GB编码）" << endl;
			cur_res.notGBEncoder++;
			return -2;
		}
		else {
			file.seekg(0, ios::beg);
			string line;
			do {
				getline(file, line);// 读第一行
			} while (trim(line) == "");
			if (1 == what_line) {
				return judge_firstline(line);
			}
			else {
				do {
					getline(file, line);// 读第二行
				} while (trim(line) == "");
				return judge_secondline(line);
			}
		}
	}
	return 0;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：首行检查（首行是否按学号/姓名/专业简称三项，空格分隔，次序不限）
  输入参数：
  返 回 值：成功返回0，失败返回-1
  说    明：
***************************************************************************/
int hw_check_stu::check_firstline() {
	if ("all" == hwfile) {
		cout << "学号-" << cur_stu.sno << " 姓名-" << cur_stu.name << " 课号-" << cur_stu.cno << " 文件数量-" << codefile_num() << endl;
		string filename = "";
		for (unsigned int i = 0; i < hw_list.size(); ++i) {
			check_line_inner(hw_list[i].filename, 1);
		}
	}
	else {
		cout << cur_stu.sno << '/' << left << setw(8) << cur_stu.name << " ： " << right;
		check_line_inner(hwfile, 1);
	}
	// 打印统计结果
	return 0;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：次行检查（次行是否按“学号 姓名”的格式给出了互验名单）
  输入参数：
  返 回 值：成功返回0，失败返回-1
  说    明：
***************************************************************************/
int hw_check_stu::check_secondline() {
	if ("all" == hwfile) {
		cout << "学号-" << cur_stu.sno << " 姓名-" << cur_stu.name << " 课号-" << cur_stu.cno << " 文件数量-" << codefile_num() << endl;
		string filename = "";
		for (unsigned int i = 0; i < hw_list.size(); ++i) {
			check_line_inner(hw_list[i].filename, 2);
		}
	}
	else {
		cout << cur_stu.sno << '/' << left << setw(8) << cur_stu.name << " ： " << right;
		check_line_inner(hwfile, 2);
	}
	// 打印统计结果
	return 0;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：综合检查函数
  输入参数：
  返 回 值：成功返回0，失败返回-1
  说    明：
***************************************************************************/
int hw_check_stu::check_stu(const string& stu) {
	set_stu(stu);//找到该学生信息
	if ("none" == cur_stu.sno)
		return -1;
	int ret = -1;
	switch (action) {
		case CHECK_BASE: 
			ret = check_base();
			break;
		case CHECK_FIRSTLINE:
			ret = check_firstline();
			break;
		case CHECK_SECONDLINE:
			ret = check_secondline();
			break;
		case CHECK_ERROR_TYPE:
		default:
			cerr << "hw_check_stu类：检查类型错误." << endl;
	}
	return 0;
}

/***************************************************************************
  隶属类名：hw_check_stu
  函数名称：
  功    能：单学生互验检查函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
vector<string> hw_check_stu::get_checklist_of(const string& sno, const string& file) const {
	vector<string> checklist;
	//打开文件
	string filename = src_rootdir + cno + '-' + sno + '/' + file;
	ifstream infile(filename, ios::in);
	if (!infile.is_open())
		return checklist;
	string line;
	getline(infile, line);
	getline(infile, line);//获得第二行
	if (!isComment(line) || isValidUTF8(line))
		return checklist;
	istringstream sin(line);
	string buf_sno, buf_name;
	int i = 0;
	while (sin >> buf_sno && sin >> buf_name) {
		if (buf_sno.size() != 7 || buf_sno == sno)
			return checklist; //后面忽略
		//加入数组
		checklist.push_back(buf_sno);
		checklist.push_back(buf_name);
	}
	return checklist;
}
int hw_check_stu::check_mutual(const string& stu, const string& file) const {
	cout << stu << ' ' << get_stu_name(stu) << endl;
	vector<string> checklist = get_checklist_of(stu, file);
	for (int i = 0; 2 * i + 1 < (int)checklist.size(); ++i) {
		cout << '\t' << checklist[2 * i] << ' ' << checklist[2 * i + 1];
		const string itsNo = checklist[2 * i];
		const string itsName = get_stu_name(checklist[2 * i]);
		if (itsName == "") {
			cout << "\t对方学号不存在" << endl;
			continue;
		}
		if (checklist[2 * i + 1] != itsName) {
			cout << "\t对方姓名不正确" << endl;
			continue;
		}
		bool checked = false;
		vector<string> itslist = get_checklist_of(itsNo, file);
		for (int j = 0; 2 * j + 1 < (int)itslist.size(); ++j) {
			if (itslist[2 * j] == stu) {
				checked = true;
				if (itslist[2 * j + 1] != get_stu_name(stu))
					cout << "\t没写对你名字";
				break;
			}
		}
		if (!checked)
			cout << "\t抛弃了你";
		cout << endl;
	}
	return 0;
}

/***************************************************************************
  隶属类名：hw_check
  函数名称：
  功    能：构造及析构函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
hw_check::hw_check(const string& _action, const string& _cno, const string& _stu, const string& _hwfile, const string& _cfgfile, int _chapter, int _week) :
	hw_check_stu(_action, _cno, _stu, _hwfile, _cfgfile, _chapter, _week) {};
hw_check::~hw_check() {};

/***************************************************************************
  隶属类名：hw_check
  函数名称：
  功    能：作业检查函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
void hw_check::checkAllFile() {
	CheckResult total_res;
	total_res = 0;
	int i = 1;
	vector<StuInfo> checklist;//检查队列
	if (STU_ALL == stu_type) checklist = stu_list;
	else checklist.push_back(cur_stu);
	for (StuInfo stu : checklist) {
		cout << left << setw(2) << i++ << " : " << right;
		check_stu(stu.sno);
		cout << "检查通过" << cur_res.correct << '/' << cur_res.total << "个文件，本次通过" << cur_res.correct << "个" << endl;
		print_result("学生详细信息", cur_res, '-');
		cout << endl;
		total_res += cur_res;
	}
	cout << "共完成" << stu_list.size() << "个学生的检查，文件总数:" << total_res.total << "，通过总数:" << total_res.correct << "，本次通过" << total_res.correct << "个" << endl;
	print_result("整体详细信息", total_res, '=');
	cout << endl;
}
void hw_check::checkOneFile() {
	cout << "课号 : " << cno << " 学生数量 : " << stu_list.size() << " 源文件名 : " << hwfile << endl;
	CheckResult total_res;
	total_res = 0;
	int i = 1;
	vector<StuInfo> checklist;//检查队列
	if (STU_ALL == stu_type) checklist = stu_list;
	else checklist.push_back(cur_stu);
	for (StuInfo stu : checklist) {
		cout << left << setw(2) << i++ << " : " << right;
		check_stu(stu.sno);
		total_res += cur_res;
	}
	cout << endl;
	cout << "检查通过" << total_res.correct << '/' << total_res.total << "个学生，本次通过" << total_res.correct << "个" << endl;
	print_result("详细信息", total_res, '=');
	cout << endl;
	if (CHECK_SECONDLINE == action) {
		cout << "交叉检查结果：" << endl;
		i = 1;
		for (StuInfo stu : stu_list) {
			cout << left << setw(2) << i++ << " : " << right;
			check_mutual(stu.sno, hwfile);
		}
	}
}
int hw_check::check() {
	if (CHECK_SECONDLINE == action && STU_ONE == stu_type) {
		cout << "HW_Check_SecondLine只能针对全体学生" << endl;
		return -1;
	}
	if (STU_ONE == stu_type && "" == cur_stu.sno) {
		cout << "本次操作的学生数量为0/文件数量为0，未执行" << endl;
		return -1;
	}
	if ("all" == hwfile)
		checkAllFile();
	else
		checkOneFile();
	return 0;
}