/* 2252201 ������ �ƿ� */
#pragma once

/* �����������Ҫ��ͷ�ļ��� */
#include <string>
#include <vector>
#include "../include/class_crt.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql����

/* ѧ����Ϣ�ṹ�� */
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

/* ��ҵ��Ϣ�ṹ�� */
struct HwInfo {
	std::string type;
	std::string cno;
	std::string hw_no;
	int chapter;
	int week;
	std::string filename;
	std::string score;
};

/* �����ݿ��ȡѧ������ҵ��Ϣ���� */
class hw_check_info : private cfgfile_read_tools {
protected:
	/* ������Ϣ */
	std::string src_rootdir;   // = "D:\Program\OOP\BigHW\hw_check\23241 - source\"
	std::string db_hw_logfile; // = hw_check.log  # ָ����ֹ�ļ���
	int	db_hw_logsize;         // = 1048576       # �����趨ֵ����־���
	
	std::string db_host;	    // = 10.80.42.244          # ���ݿ��������IP��ַ��У԰���ڣ�
	std::string db_port;		// = 3306			# ���ݿ�������Ķ˿ں�
	std::string db_name;		// = homework              # ���ݿ�����
	std::string	db_username;	// = hwcheck               # �û���
	std::string db_passwd;		// = hw_CheCk - For23241 * oOP # �û�����
	std::string db_currterm;	// = 2023 / 2024 / 1           # ��ǰѧ��

	/* ���ݿ� */
	MYSQL* mysql;
	MYSQL_RES* result;
	MYSQL_ROW  row;

	/* ѧ������ҵ��Ϣ */
	std::vector<StuInfo> stu_list;
	std::vector<HwInfo> hw_list;

	/* ���ó�ʼ������ */
	void read_cfgfile(); //�������ļ�
	void init_mysql();   //��ʼ�����������ݿ�
	//void check_floder_existed(); //�ж��ļ����Ƿ����
public:
	/* ������������� */
	hw_check_info(const char* _cfgname = "");
	hw_check_info(const std::string& _cfgname);
	virtual ~hw_check_info();

	/* ���ݿ���Һ��� */
	bool is_connected() const;	// �ж��Ƿ��������ݿ�
	const std::vector<StuInfo>& load_stu_info(const std::string& stu_cno);
	const std::vector<HwInfo>& load_hw_info(const std::string& hw_cno, int hw_chapter, int hw_week);

	/* �ӵ�ǰ��ѧ���������ҵ��ض�ѧ����Ϣ */
	const std::string get_stu_name(const std::string& stu_sno) const;

	/* Debug���� */
	void show_stu_list() const;
	void show_hw_list() const;
};


/* ������͵�ö�� */
enum CHECK_ACTION {
	CHECK_BASE = 0,
	CHECK_FIRSTLINE,
	CHECK_SECONDLINE,
	CHECK_ERROR_TYPE
};

/* ѧ������ö�� */
enum STU_TYPE {
	STU_ALL = 0,
	STU_ONE
};

/* ��¼��������� */
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

	/* ��������� */
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

/* ����ض�ѧ���ض��ļ����� */
class hw_check_stu : public hw_check_info {
protected:
	/* ��ҵ������ */
	CHECK_ACTION action;	//�����Ϊ
	const std::string cno;		//�κ�
	STU_TYPE stu_type;			//ѧ������
	const std::string hwfile;	//��ҵ�ļ���
	int chapter;	//�½�
	int week;		//�ܴ�
	std::string display;		//��ʾ����
	
	/* ��ǰ�����ѧ����Ϣ */
	StuInfo cur_stu;//��¼��ǰѧ����Ϣ
	std::string foldername;//��ǰѧ�����ļ���
	CheckResult cur_res;//��¼��ǰѧ�������

	/* �ļ���麯�� */
	bool isValidUTF8(const std::string& content) const;
	bool isGBEncoded(std::ifstream& file) const;
	bool isPDF(std::ifstream& file) const;
	bool isComment(std::string& line) const;
	int judge_firstline(const std::string& content);
	int judge_secondline(const std::string& content);

	/* �������ú��� */
	void set_stu(const std::string& _stu);

	/* �ñ����ʾͳ�ƽ�� */
	int codefile_num();//.cpp.hpp.h.c��β���ļ�����
	void print_result(const char* prompt, const CheckResult& _res, char cutline = '-') const;

	/* ��ѧ����麯�� */
	int check_base_inner(const std::string& _filename);
	int check_base();
	int check_line_inner(const std::string& _filename, int line);
	int check_firstline();
	int check_secondline();

	/* ��ѧ�������鸨������ */
	std::vector<std::string> get_checklist_of(const std::string& sno, const std::string& file) const;

public:
	/* ������������� */
	hw_check_stu(const std::string& action, const std::string& cno, const std::string& stu, const std::string& hwfile, const std::string& cfgfile, int _chapter = -1, int _week = -1);
	virtual ~hw_check_stu();

	/* �������ú��� */
	void set_display(const std::string& display);

	/* �ۺϼ�麯�� */
	int check_stu(const std::string& stu);
	int check_mutual(const std::string& stu, const std::string& file) const;
};

/* �ۺϼ����ҵ���� */
class hw_check : public hw_check_stu {
protected:
	/* ��ҵ��麯�� */
	void checkAllFile();
	void checkOneFile();
public:
	/* ������������� */
	hw_check(const std::string& action, const std::string& cno, const std::string& stu, const std::string& hwfile, const std::string& cfgfile, int _chapter = -1, int _week = -1);
	virtual ~hw_check();

	/* ��ҵ��麯�� */
	int check();
};