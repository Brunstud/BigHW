/* 2252201 胡世成 计科 */
#pragma once

/* 允许按需加入需要的头文件等 */
#include <string>
#include <vector>
#include "../include/class_crt.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql特有

/* 学生信息结构体 */
struct StuInfo {
	std::string term;
	std::string grade;
	std::string sno;
	std::string name;
	std::string sex;
	std::string bmajor;
	std::string major;
	std::string cno;
};

/* 作业信息结构体 */
struct HwInfo {
	std::string type;
	std::string cno;
	std::string hw_no;
	int chapter;
	int week;
	std::string filename;
	std::string score;
};

/* 从数据库获取学生及作业信息的类 */
class hw_check_info : private cfgfile_read_tools {
protected:
	/* 配置信息 */
	std::string src_rootdir;   // = "D:\Program\OOP\BigHW\hw_check\23241 - source\"
	std::string db_hw_logfile; // = hw_check.log  # 指定起止文件名
	int	db_hw_logsize;         // = 1048576       # 超过设定值则日志清空
	
	std::string db_host;	    // = 10.80.42.244          # 数据库服务器的IP地址（校园网内）
	std::string db_port;		// = 3306			# 数据库服务器的端口号
	std::string db_name;		// = homework              # 数据库名称
	std::string	db_username;	// = hwcheck               # 用户名
	std::string db_passwd;		// = hw_CheCk - For23241 * oOP # 用户密码
	std::string db_currterm;	// = 2023 / 2024 / 1           # 当前学期

	/* 数据库 */
	MYSQL* mysql;
	MYSQL_RES* result;
	MYSQL_ROW  row;

	/* 学生及作业信息 */
	std::vector<StuInfo> stu_list;
	std::vector<HwInfo> hw_list;

	/* 内置初始化函数 */
	void read_cfgfile(); //读配置文件
	void init_mysql();   //初始化及连接数据库
	//void check_floder_existed(); //判断文件夹是否存在
public:
	/* 构造和析构函数 */
	hw_check_info(const char* _cfgname = "");
	hw_check_info(const std::string& _cfgname);
	virtual ~hw_check_info();

	/* 数据库查找函数 */
	bool is_connected() const;	// 判断是否连接数据库
	const std::vector<StuInfo>& load_stu_info(const std::string& stu_cno);
	const std::vector<HwInfo>& load_hw_info(const std::string& hw_cno, int hw_chapter, int hw_week);

	/* 从当前的学生名单中找到特定学生信息 */
	const std::string get_stu_name(const std::string& stu_sno) const;

	/* Debug函数 */
	void show_stu_list() const;
	void show_hw_list() const;
};


/* 检查类型的枚举 */
enum CHECK_ACTION {
	CHECK_BASE = 0,
	CHECK_FIRSTLINE,
	CHECK_SECONDLINE,
	CHECK_ERROR_TYPE
};

/* 学生类型枚举 */
enum STU_TYPE {
	STU_ALL = 0,
	STU_ONE
};

/* 记录检查结果数量 */
struct CheckResult {
	int total = 0;
	int correct = 0;
	int notSubmitted = 0;
	int notGBEncoder = 0;
	int notPDF = 0;
	int	fir_notComment = 0;
	int fir_notThreeItem = 0;
	int fir_error = 0;
	int sec_notComment = 0;

	/* 重载运算符 */
	inline const CheckResult& operator=(int num) {
		total = correct = num;
		notSubmitted = notGBEncoder = notPDF = num;
		fir_notComment = fir_notThreeItem = fir_error = num;
		sec_notComment = num;
		return *this;
	}
	inline const CheckResult& operator+=(const CheckResult& other) {
		total += other.total;
		correct += other.correct;
		notSubmitted += other.notSubmitted;
		notGBEncoder += other.notGBEncoder;
		notPDF += other.notPDF;
		fir_notComment += other.fir_notComment;
		fir_notThreeItem += other.fir_notThreeItem;
		fir_error += other.fir_error;
		sec_notComment += other.sec_notComment;
		return *this;
	}
};

/* 检查特定学生特定文件的类 */
class hw_check_stu : public hw_check_info {
protected:
	/* 作业检查参数 */
	CHECK_ACTION action;	//检查行为
	const std::string cno;		//课号
	STU_TYPE stu_type;			//学生类型
	const std::string hwfile;	//作业文件名
	int chapter;	//章节
	int week;		//周次
	std::string display;		//显示设置
	
	/* 当前被检查学生信息 */
	StuInfo cur_stu;//记录当前学生信息
	std::string foldername;//当前学生的文件夹
	CheckResult cur_res;//记录当前学生检查结果

	/* 文件检查函数 */
	bool isValidUTF8(const std::string& content) const;
	bool isGBEncoded(std::ifstream& file) const;
	bool isPDF(std::ifstream& file) const;
	bool isComment(std::string& line) const;
	int judge_firstline(const std::string& content);
	int judge_secondline(const std::string& content);

	/* 参数设置函数 */
	void set_stu(const std::string& _stu);

	/* 用表格显示统计结果 */
	int codefile_num();//.cpp.hpp.h.c结尾的文件数量
	void print_result(const char* prompt, const CheckResult& _res, char cutline = '-') const;

	/* 单学生检查函数 */
	int check_base_inner(const std::string& _filename);
	int check_base();
	int check_line_inner(const std::string& _filename, int line);
	int check_firstline();
	int check_secondline();

	/* 单学生互验检查辅助函数 */
	std::vector<std::string> get_checklist_of(const std::string& sno, const std::string& file) const;

public:
	/* 构造和析构函数 */
	hw_check_stu(const std::string& action, const std::string& cno, const std::string& stu, const std::string& hwfile, const std::string& cfgfile, int _chapter = -1, int _week = -1);
	virtual ~hw_check_stu();

	/* 参数设置函数 */
	void set_display(const std::string& display);

	/* 综合检查函数 */
	int check_stu(const std::string& stu);
	int check_mutual(const std::string& stu, const std::string& file) const;
};

/* 综合检查作业的类 */
class hw_check : public hw_check_stu {
protected:
	/* 作业检查函数 */
	void checkAllFile();
	void checkOneFile();
public:
	/* 构造和析构函数 */
	hw_check(const std::string& action, const std::string& cno, const std::string& stu, const std::string& hwfile, const std::string& cfgfile, int _chapter = -1, int _week = -1);
	virtual ~hw_check();

	/* 作业检查函数 */
	int check();
};