/* 2252201 ������ �ƿ� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//���������Ҫ��ͷ�ļ� 
#include <sstream>
#include <fstream>
#include <iomanip>
#include <climits>
#include "hw_check.h"
#include "../include_mariadb_x86/mysql/mysql.h"      // mysql����
using namespace std;

/***************************************************************************
  �������ƣ�trim
  ��    �ܣ�����������ȥ���ַ�����β�ո���Ʊ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static inline string& trim(string& str) {
	str.erase(0, str.find_first_not_of(" \t")); // ȥ����ͷ�ո���Ʊ��
	str.erase(str.find_last_not_of(" \t") + 1); // ȥ����β�ո���Ʊ��
	return str;
}

/***************************************************************************
  ����������hw_check_info
  �������ƣ�
  ��    �ܣ��������������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
// ���캯��
hw_check_info::hw_check_info(const char* _cfgname) : cfgfile_read_tools(_cfgname) {
	read_cfgfile();//��ȡ�����ļ�
	init_mysql();//��ʼ�����ݿ�
}
hw_check_info::hw_check_info(const std::string& _cfgname) : cfgfile_read_tools(_cfgname) {
	read_cfgfile();//��ȡ�����ļ�
	init_mysql();//��ʼ�����ݿ�
}
// ��������
hw_check_info::~hw_check_info() {
	/* �ر����ݿ����� */
	mysql_close(mysql);
}

/***************************************************************************
  ����������hw_check_info
  �������ƣ�
  ��    �ܣ���ȡ�����ļ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hw_check_info::read_cfgfile() {
	if (cfgfile_read_tools::is_open()) {
		if (item_get_value("[�ļ�Ŀ¼]", "src_rootdir", ".\\23241 - source\\", src_rootdir))
			cerr << "�����ļ�[�ļ�Ŀ¼]��ȡ����" << endl;
		if (item_get_value("[��־Ŀ¼]", "db_hw_logfile", "hw_check.log", db_hw_logfile) ||
			item_get_value("[��־Ŀ¼]", "db_hw_logsize", 1048576, 0, INT_MAX, db_hw_logsize))
			cerr << "�����ļ�[Ŀ¼��־]��ȡ����" << endl;
		if (item_get_value("[���ݿ�]", "db_host", "", db_host) ||				// = 10.80.42.244			# ���ݿ��������IP��ַ��У԰���ڣ�
			item_get_value("[���ݿ�]", "db_port", "", db_port) ||				// = 3306					# ���ݿ�������Ķ˿ں�
			item_get_value("[���ݿ�]", "db_name", "1", db_name) ||				// = homework				# ���ݿ�����
			item_get_value("[���ݿ�]", "db_username", "", db_username) ||		// = hwcheck				# �û���
			item_get_value("[���ݿ�]", "db_passwd", "", db_passwd) ||			// = hw_CheCk - For23241 * oOP # �û�����
			item_get_value("[���ݿ�]", "db_currterm", "", db_currterm))			// = 2023 / 2024 / 1           # ��ǰѧ��
			cerr << "�����ļ�[���ݿ�]��ȡ����" << endl;
	}
	else
		cerr << "�޷��������ļ���" << cfgname << endl;
}

/***************************************************************************
  ����������hw_check_info
  �������ƣ�
  ��    �ܣ���ʼ�����������ݿ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hw_check_info::init_mysql() {
	/* ���ó�ֵ */
	result = NULL;
	row = NULL;

	/* ��ʼ�� mysql ������ʧ�ܷ���NULL */
	if ((mysql = mysql_init(NULL)) == NULL) {
		cerr << "mysql_init failed" << endl;
		return;
	}

	/* �������ݿ⣬ʧ�ܷ���NULL
		1��mysqldû����
		2��û��ָ�����Ƶ����ݿ���� */
	if (mysql_real_connect(mysql, db_host.c_str(), db_username.c_str(), db_passwd.c_str(), db_name.c_str(), 0, NULL, 0) == NULL) {
		cout << "mysql_real_connect failed(" << mysql_error(mysql) << ")" << endl;
		return;
	}

	/* �����ַ���������������ַ����� */
	mysql_set_character_set(mysql, "gbk");
}

/***************************************************************************
  ����������hw_check_info
  �������ƣ�
  ��    �ܣ��ж��Ƿ��������ݿ�
  ���������
  �� �� ֵ��
  ˵    �������ݿ���Һ���
***************************************************************************/
bool hw_check_info::is_connected() const {
	return (mysql != NULL);
}

/***************************************************************************
  ����������hw_check_info
  �������ƣ�
  ��    �ܣ���ȡѧ����Ϣ
  ���������
  �� �� ֵ��
  ˵    �������ݿ���Һ���
***************************************************************************/
const vector<StuInfo>& hw_check_info::load_stu_info(const string& stu_cno) {
	// ���ԭѧ����Ϣ�б�
	stu_list.clear();
	// �ж��Ƿ��������ݿ�
	if (!mysql)
		return stu_list;

	/* ��ѯ���
			select * from view_hwcheck_stulist where stu_cno = "xxx"
		���壺��ʾ��ѯĳ�γ̵�����ѧ����Ϣ
		����ֵ���ɹ�����0�����ɹ���0
		���ɹ��ĳ��������ԣ�
			1����ѯ�ַ��������﷨����
			2����ѯ�����ڵ����ݱ� */
	//if (mysql_query(mysql, ("select * from view_hwcheck_stulist where stu_cno = \"" + stu_cno + "\"").c_str())) {
	if (mysql_query(mysql, "select * from view_hwcheck_stulist where stu_cno = \"10108001\"")) {
		cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
		return stu_list;//���ؿձ�
	}

	/* ����ѯ����洢���������ִ����򷵻�NULL
		ע�⣺��ѯ���ΪNULL�����᷵��NULL */
	if ((result = mysql_store_result(mysql)) == NULL) {
		cout << "mysql_store_result failed" << endl;
		return stu_list;//���ؿձ�
	}

	/* ��ӡ��ǰ��ѯ���ļ�¼������ */
	//cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

	/* ѭ����ȡ�������������ļ�¼
		2��row������˳����select���ص���˳����ͬ��������
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

	/* �ͷ�result������ᶪʧ�ڴ� */
	mysql_free_result(result);

	return stu_list;
}

/***************************************************************************
  ����������hw_check_info
  �������ƣ�
  ��    �ܣ���ȡ��ҵ��Ϣ
  ���������
  �� �� ֵ��
  ˵    �������ݿ���Һ���
***************************************************************************/
const vector<HwInfo>& hw_check_info::load_hw_info(const string& hw_cno, int hw_chapter, int hw_week) {
	// ���ԭ��ҵ��Ϣ�б�
	hw_list.clear();
	// �ж��Ƿ��������ݿ�
	if (!mysql)
		return hw_list;

	/* ��ѯ���
			select * from view_hwcheck_hwlist where hw_cno = "xxx" and hw_chapter = "xxx" and hw_week = "xxx"
		���壺��ʾ��ѯ��ҵ��Ϣ
		����ֵ���ɹ�����0�����ɹ���0
		���ɹ��ĳ��������ԣ�
			1����ѯ�ַ��������﷨����
			2����ѯ�����ڵ����ݱ� */
	if (-1 == hw_chapter && -1 == hw_week && mysql_query(mysql, ("select * from view_hwcheck_hwlist where hw_cno = \"" + hw_cno + "\"").c_str()) ||
		-1 != hw_chapter && -1 == hw_week && mysql_query(mysql, ("select * from view_hwcheck_hwlist where hw_cno = \"" + hw_cno + "\" and hw_chapter = \"" + to_string(hw_chapter) + "\"").c_str()) ||
		-1 == hw_chapter && -1 != hw_week && mysql_query(mysql, ("select * from view_hwcheck_hwlist where hw_cno = \"" + hw_cno + "\" and hw_week = \"" + to_string(hw_week) + "\"").c_str()) ||
		-1 != hw_chapter && -1 != hw_week && mysql_query(mysql, ("select * from view_hwcheck_hwlist where hw_cno = \"" + hw_cno + "\" and hw_chapter = \"" + to_string(hw_chapter) + "\" and hw_week = \"" + to_string(hw_week) + "\"").c_str())
		) {
		cout << "mysql_query failed(" << mysql_error(mysql) << ")" << endl;
		return hw_list;//���ؿձ�
	}

	/* ����ѯ����洢���������ִ����򷵻�NULL
		ע�⣺��ѯ���ΪNULL�����᷵��NULL */
	if ((result = mysql_store_result(mysql)) == NULL) {
		cout << "mysql_store_result failed" << endl;
		return hw_list;//���ؿձ�
	}

	/* ��ӡ��ǰ��ѯ���ļ�¼������ */
	//cout << "select return " << (int)mysql_num_rows(result) << " records" << endl;

	/* ѭ����ȡ�������������ļ�¼
		2��row������˳����select���ص���˳����ͬ��������
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

	/* �ͷ�result������ᶪʧ�ڴ� */
	mysql_free_result(result);

	return hw_list;
}

/***************************************************************************
  ����������hw_check_info
  �������ƣ�
  ��    �ܣ��ӵ�ǰ��ѧ���������ҵ��ض�ѧ������
  ���������
  �� �� ֵ��
  ˵    ����δ�ҵ��򷵻ؿմ�����""
***************************************************************************/
const string hw_check_info::get_stu_name(const string& stu_sno) const {
	for (StuInfo stu : stu_list)
		if (stu.sno == stu_sno)
			return stu.name;
	// δ�ҵ������ؿմ�
	return string("");
}

/***************************************************************************
  ����������hw_check_info
  �������ƣ�
  ��    �ܣ���ӡѧ���б���Ϣ
  ���������
  �� �� ֵ��
  ˵    ����Debug����
***************************************************************************/
void hw_check_info::show_stu_list() const {
	cout << "ѧ���б�" << endl << left;
	for (StuInfo stu : stu_list) {
		cout << "#" << stu.sno << ' ' << setw(8) << stu.name << " : " << stu.grade << ' ' << stu.cno << ' ' 
			<< stu.sex << ' ' << stu.term << ' ' << stu.major << "��" << stu.bmajor << "�� " << endl;
	}
	cout << endl << right;
	return;
}

/***************************************************************************
  ����������hw_check_info
  �������ƣ�
  ��    �ܣ���ӡ��ҵ�б���Ϣ
  ���������
  �� �� ֵ��
  ˵    ����Debug����
***************************************************************************/
void hw_check_info::show_hw_list() const {
	cout << "��ҵ�б�" << endl << left;
	for (HwInfo hw : hw_list) {
		cout << "#" << setw(2) << hw.type << ' ' << setw(27) << hw.filename << " : " 
			<< hw.cno << ' ' << hw.hw_no << ' ' << hw.chapter << ' ' << hw.week << ' ' << hw.score << endl;
	}
	cout << endl << right;
	return;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ����켰��������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
hw_check_stu::hw_check_stu(const string& _action, const string& _cno, const string& _stu, const string& _hwfile, const string& _cfgfile, int _chapter, int _week)
	: cno(_cno), hwfile(_hwfile), hw_check_info(_cfgfile), chapter(_chapter), week(_week) {
	// ���ü����Ϊ
	if ("base" == _action)
		action = CHECK_BASE;
	else if ("firstline" == _action)
		action = CHECK_FIRSTLINE;
	else if ("secondline" == _action)
		action = CHECK_SECONDLINE;
	else
		action = CHECK_ERROR_TYPE;
	// �����ݿ��ȡ��Ϣ
	hw_check_info::load_stu_info(cno);
	hw_check_info::load_hw_info(cno, chapter, week);
	// ����ѧ���������
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
	// display Ĭ��ȫ��ʾ
	display = "11111";
}
hw_check_stu::~hw_check_stu() {};

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ��������ú���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hw_check_stu::set_stu(const string& _stu) {
	if ("all" == _stu) {
		cerr << "ע�⣺hw_check_stu�಻֧�ֶ�����ѧ���ļ��" << endl;
		return;
	}
	// �����ļ�����
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
	cur_res = 0;//�������
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
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ�����ļ��Ƿ�����Ч��UTF-8����
  ���������
  �� �� ֵ���Ƿ���true����/�򲻿��ļ�����false
  ˵    ����
***************************************************************************/
bool hw_check_stu::isValidUTF8(const string& str) const
{
	int encodingBytesCount = 0;
	bool allTextsAreASCIIChars = true;

	for (char current : str)
	{
		//�ж��Ƿ���ASCII�ַ��������������ַ�ʱallTextsAreASCIICharsΪfalse
		if ((current & 0x80) == 0x80)
			allTextsAreASCIIChars = false;

		//�ж��Ƿ�Ϊһ���ַ��Ŀ�ʼ�ֽ�
		if (encodingBytesCount == 0)
		{
			if ((current & 0x80) == 0)
			{
				// ASCII chars, from 0x00-0x7F
				continue;
			}

			if ((current & 0xC0) == 0xC0)	///���ڵ������ֽ�
			{
				encodingBytesCount = 1;
				current <<= 2;

				// More than two bytes used to encoding a unicode char.
				// Calculate the real length.
				while ((current & 0x80) == 0x80)
				{
					current <<= 1;	//�ж���һλ�Ƿ�Ϊ1
					encodingBytesCount++;	//��ǰ�ַ������ֽ���
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
			if ((current & 0xC0) == 0x80)	///��ǰ�ֽ��Ƿ���10��ͷ
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
		// ���ֽڻ�ASCII�ַ�
		if (c <= 0x7f) continue;
		// ����ʼ�ֽ�
		if ((c & 0xC0) == 0x80) return false;
		// ���ֽ��ַ�
		if ((c & 0xE0) == 0xC0) n = 1; // 2�ֽ��ַ�
		else if ((c & 0xF0) == 0xE0) n = 2; // 3�ֽ��ַ�
		else if ((c & 0xF8) == 0xF0) n = 3; // 4�ֽ��ַ�
		else return false;
		// �����ֽ��ַ��ĺ����ֽ�
		for (j = 0; j < n; j++) {
			if ((++i == ix) || ((str[i] & 0xC0) != 0x80))
				return false;
		}
	}
	return true;
}*/

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ�����ļ��Ƿ���GB����
  ���������
  �� �� ֵ���Ƿ���true����/�򲻿��ļ�����false
  ˵    ����
***************************************************************************/
bool hw_check_stu::isGBEncoded(ifstream& file) const {
	if (!file) {
		return false; // �ļ��޷��򿪻��Ѿ������ļ�ĩβ
	}
	// ��ȡ��ǰ�ļ�ָ���λ��
	streampos currentPosition = file.tellg();

	//���ļ�ָ���Ƶ���ͷ
	file.seekg(3, ios::beg);
	// ��ȡ�ļ�����
	string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());

	// ���½��ļ�ָ�붨λ��֮ǰ�洢��λ��
	file.seekg(currentPosition);
	// ������UTF-8���룬����Ϊ��GB����
	return !isValidUTF8(content);
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ�����ļ��Ƿ���PDF��ʽ
  ���������
  �� �� ֵ���Ƿ���true����/�򲻿��ļ�����false
  ˵    ����
***************************************************************************/
bool hw_check_stu::isPDF(std::ifstream& file) const {
	if (!file) {
		return false; // �ļ��޷��򿪻��Ѿ������ļ�ĩβ
	}
	// ��ȡ��ǰ�ļ�ָ���λ��
	streampos currentPosition = file.tellg();
	
	//���ļ�ָ���Ƶ���ͷ
	file.seekg(0, ios::beg);
	// ��ȡ�ļ���ǰ4���ֽ�
	char buffer[5] = { 0 };
	file.read(buffer, 4);

	// ���½��ļ�ָ�붨λ��֮ǰ�洢��λ��
	file.seekg(currentPosition);
	// ����Ƿ�Ϊ "%PDF"
	return string(buffer) == "%PDF";
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�isComment
  ��    �ܣ��жϸ����ǲ���ע��
  ���������
  �� �� ֵ��
  ˵    ����˳��ȥ��ע�ͷ�
***************************************************************************/
bool hw_check_stu::isComment(string& line) const {
	line.erase(0, line.find_first_not_of(" \t")); // ȥ����ͷ�ո���Ʊ��
	line.erase(line.find_last_not_of(" \t") + 1); // ȥ����β�ո���Ʊ��
	if ('/' == line[0] && '*' == line[1]) {
		line.erase(0, 2); // ɾ��ǰ�����ַ�
		line.erase(line.find_last_not_of("/* \t") + 1); // ȥ����βע�ͷ�������еĻ���
		return true;
	}
	else if ('/' == line[0] && '/' == line[1]) {
		line.erase(0, 2); // ɾ��ǰ�����ַ�
		return true;
	}
	return false;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�judge_firstline
  ��    �ܣ��жϵ�һ��
  ���������
  �� �� ֵ��0:��ȷ��1:���в���ע���У�2:���в������3:���м�����
  ˵    ����
***************************************************************************/
int hw_check_stu::judge_firstline(const string& _line) {
	string line = _line;
	if (!isComment(line)) {
		cout << "���в���ע���� [" << _line << "]" << endl;
		cur_res.fir_notComment++;
		return 1;
	}
	istringstream sin(line);
	string buffer;
	bool name = false, sno = false, dept = false;
	for (int i = 0; i < 3; ++i) {
		sin >> buffer;
		if (!sin) {
			cout << "���в������� [" << _line << "]" << endl;
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
		cout << "����" << (sno ? "" : "ѧ�Ų�ƥ�� ") << (name ? "" : "������ƥ�� ") << (dept ? "" : "�༶��ƥ�� ") << " [" << _line << "]" << endl;
		cur_res.fir_error++;
		return 3;
	}
	cout << "��ȷ" << endl;
	cur_res.correct++;
	return 0;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�judge_secondline
  ��    �ܣ��жϵ�һ��
  ���������
  �� �� ֵ��0:��ȷ��1:���в���ע���У�2:�������ݺ��ԣ�3:д���Լ�
  ˵    ����
***************************************************************************/
int hw_check_stu::judge_secondline(const string& _line) {
	string line = _line;
	if (!isComment(line)) {
		cout << "���в���ע����" << endl;// [" << _line << "]" << endl;
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
			cout << "��[" << i << "]��ѧ���������Ϣ��ȫ(ֻ����һ��)���������ݺ���" << endl;
			return 2;
		}
		else if (buffer.size() != 7) {
			cout << "��" << i + 1 << "λͬѧ��ѧ��[" << buffer << "]����7λ���������ݺ���" << endl;
			return 2;
		}
		else if (buffer == cur_stu.sno) {
			cout << "��[" << i + 1 << "]��д���Լ�" << endl;
			return 3;
		}
	}
	cout << "��ȷ" << endl;
	return 0;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ�.cpp/.hpp/.h/.c��β���ļ�����
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int hw_check_stu::codefile_num() {
	int num = 0;
	for (HwInfo hw : hw_list) {
		string thisfile = hw.filename;
		string suffix = ""; //����ļ�����׺
		if (thisfile.length() >= 3)
			suffix = thisfile.substr(thisfile.length() - 3);
		// �ҳ���Ҫ���е�.cpp/.h/.hpp/.c�ļ�
		if (("cpp" == suffix || "hpp" == suffix || 'h' == suffix[2] || 'c' == suffix[2]))
			num++;
	}
	return num;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ��ñ����ʾͳ�ƽ��
  ���������prompt����ʾ�� _res��ͳ�ƽ�� cutline���ָ����ַ�('-'��'=')
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hw_check_stu::print_result(const char* prompt, const CheckResult& _res, char cutline) const {
	// �ҵ������趨���
	int width = max(max(max((_res.correct ? 2 + 4 + 3 : 0), (_res.notSubmitted ? 2 + 6 + 3 : 0)), max((_res.notGBEncoder ? 2 + 26 + 3 : 0), (_res.notPDF ? 2 + 11 + 3 : 0))),
		max(max((_res.fir_notComment ? 2 + 12 + 3 : 0), (_res.fir_notThreeItem ? 2 + 12 + 3 : 0)), max((_res.fir_error ? 2 + 12 + 3 : 0), (_res.sec_notComment ? 2 + 12 + 3 : 0))));
	//��ӡ���
	cout << setw(width + 7) << setfill(cutline) << cutline << endl << setfill(' ');
	if (prompt) cout << prompt << endl;
	cout << setw(width + 7) << setfill(cutline) << cutline << endl << setfill(' ');
	if (_res.correct) cout << setw(width) << "��ȷ : " << _res.correct << endl;
	if (_res.notSubmitted) cout << setw(width) << "δ�ύ : " << _res.notSubmitted << endl;
	if (_res.notGBEncoder) cout << setw(width) << "Դ�ļ���ʽ����ȷ(��GB����) : " << _res.notGBEncoder << endl;
	if (_res.notPDF) cout << setw(width) << "����PDF�ļ� : " << _res.notPDF << endl;
	if (_res.fir_notComment) cout << setw(width) << "���в���ע�� : " << _res.fir_notComment << endl;
	if (_res.fir_notThreeItem) cout << setw(width) << "���в������� : " << _res.fir_notThreeItem << endl;
	if (_res.fir_error) cout << setw(width) << "���м����� : " << _res.fir_error << endl;
	if (_res.sec_notComment) cout << setw(width) << "���в���ע�� : " << _res.sec_notComment << endl;
	cout << setw(width + 7) << setfill(cutline) << cutline << endl << setfill(' ');
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ�������飨�Ƿ��ύ��pdf�ļ���ʽ�Ƿ���ȷ��Դ�����ļ��Ƿ�GB���룩
  ���������
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1
  ˵    �����ڲ�����
***************************************************************************/
int hw_check_stu::check_base_inner(const string& _filename) {
	cur_res.total++;
	string thisfile = _filename;
	string suffix = ""; //����ļ�����׺
	if (thisfile.length() >= 3)
		suffix = thisfile.substr(thisfile.length() - 3);
	else {
		cout << "�ļ������󣺳��Ȳ��������ַ�" << endl;
		return -1;
	}
	//��ӡ���
	if (hwfile == "all")
		cout << "  " << left << setw(27) << thisfile << " : " << right;
	//���ļ�
	string filename = foldername + '/' + thisfile;
	ifstream file(filename, ios::in);
	if (!file.is_open()) {
		cout << "δ�ύ";
		cur_res.notSubmitted++;
	}
	else if ("pdf" == suffix && !isPDF(file)) {
		cout << "����PDF��ʽ";
		cur_res.notPDF++;
	}
	else if (("cpp" == suffix || "hpp" == suffix || 'h' == suffix[2] || 'c' == suffix[2]) && !isGBEncoded(file)) {
		cout << "Դ�ļ���ʽ����ȷ����GB���룩";
		cur_res.notGBEncoder++;
	}
	else {
		cout << "��ȷ";
		cur_res.correct++;
	}
	cout << endl;
	return 0;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ�������飨�Ƿ��ύ��pdf�ļ���ʽ�Ƿ���ȷ��Դ�����ļ��Ƿ�GB���룩
  ���������
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1
  ˵    ����
***************************************************************************/
int hw_check_stu::check_base() {
	if ("all" == hwfile) {
		cout << "ѧ��-" << cur_stu.sno << " ����-" << cur_stu.name << " �κ�-" << cur_stu.cno << " �ļ�����-" << hw_list.size() << endl;
		string filename = "";
		for (unsigned int i = 0; i < hw_list.size(); ++i) {
			if (check_base_inner(hw_list[i].filename))
				return -1;
		}
	}
	else {
		cout << cur_stu.sno << '/' << left << setw(8) << cur_stu.name << " �� " << right;
		if (check_base_inner(hwfile))
			return -1;
	}
	// ��ӡͳ�ƽ��
	//print_total();
	return 0;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ����м�飨�����Ƿ�ѧ��/����/רҵ�������ո�ָ��������ޣ�
  ���������
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1
  ˵    ����
***************************************************************************/
int hw_check_stu::check_line_inner(const string& _filename, int what_line) {
	string thisfile = _filename;
	string suffix = ""; //����ļ�����׺
	if (thisfile.length() >= 3)
		suffix = thisfile.substr(thisfile.length() - 3);
	else {
		cout << "�ļ������󣺳��Ȳ��������ַ�" << endl;
		return -1;
	}
	// �ҳ���Ҫ���е�.cpp/.h/.hpp/.c�ļ�
	if (("cpp" == suffix || "hpp" == suffix || 'h' == suffix[2] || 'c' == suffix[2])) {
		cur_res.total++;
		//��ӡ���
		if (hwfile == "all")
			cout << "  " << left << setw(27) << thisfile << " : " << right;
		//���ļ�
		string filename = foldername + '/' + thisfile;
		ifstream file(filename, ios::in);
		if (!file.is_open()) {
			cout << "δ�ύ" << endl;
			cur_res.notSubmitted++;
			return -1;
		}
		if (!isGBEncoded(file)) {
			cout << "Դ�ļ���ʽ����ȷ����GB���룩" << endl;
			cur_res.notGBEncoder++;
			return -2;
		}
		else {
			file.seekg(0, ios::beg);
			string line;
			do {
				getline(file, line);// ����һ��
			} while (trim(line) == "");
			if (1 == what_line) {
				return judge_firstline(line);
			}
			else {
				do {
					getline(file, line);// ���ڶ���
				} while (trim(line) == "");
				return judge_secondline(line);
			}
		}
	}
	return 0;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ����м�飨�����Ƿ�ѧ��/����/רҵ�������ո�ָ��������ޣ�
  ���������
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1
  ˵    ����
***************************************************************************/
int hw_check_stu::check_firstline() {
	if ("all" == hwfile) {
		cout << "ѧ��-" << cur_stu.sno << " ����-" << cur_stu.name << " �κ�-" << cur_stu.cno << " �ļ�����-" << codefile_num() << endl;
		string filename = "";
		for (unsigned int i = 0; i < hw_list.size(); ++i) {
			check_line_inner(hw_list[i].filename, 1);
		}
	}
	else {
		cout << cur_stu.sno << '/' << left << setw(8) << cur_stu.name << " �� " << right;
		check_line_inner(hwfile, 1);
	}
	// ��ӡͳ�ƽ��
	return 0;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ����м�飨�����Ƿ񰴡�ѧ�� �������ĸ�ʽ�����˻���������
  ���������
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1
  ˵    ����
***************************************************************************/
int hw_check_stu::check_secondline() {
	if ("all" == hwfile) {
		cout << "ѧ��-" << cur_stu.sno << " ����-" << cur_stu.name << " �κ�-" << cur_stu.cno << " �ļ�����-" << codefile_num() << endl;
		string filename = "";
		for (unsigned int i = 0; i < hw_list.size(); ++i) {
			check_line_inner(hw_list[i].filename, 2);
		}
	}
	else {
		cout << cur_stu.sno << '/' << left << setw(8) << cur_stu.name << " �� " << right;
		check_line_inner(hwfile, 2);
	}
	// ��ӡͳ�ƽ��
	return 0;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ��ۺϼ�麯��
  ���������
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1
  ˵    ����
***************************************************************************/
int hw_check_stu::check_stu(const string& stu) {
	set_stu(stu);//�ҵ���ѧ����Ϣ
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
			cerr << "hw_check_stu�ࣺ������ʹ���." << endl;
	}
	return 0;
}

/***************************************************************************
  ����������hw_check_stu
  �������ƣ�
  ��    �ܣ���ѧ�������麯��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
vector<string> hw_check_stu::get_checklist_of(const string& sno, const string& file) const {
	vector<string> checklist;
	//���ļ�
	string filename = src_rootdir + cno + '-' + sno + '/' + file;
	ifstream infile(filename, ios::in);
	if (!infile.is_open())
		return checklist;
	string line;
	getline(infile, line);
	getline(infile, line);//��õڶ���
	if (!isComment(line) || isValidUTF8(line))
		return checklist;
	istringstream sin(line);
	string buf_sno, buf_name;
	int i = 0;
	while (sin >> buf_sno && sin >> buf_name) {
		if (buf_sno.size() != 7 || buf_sno == sno)
			return checklist; //�������
		//��������
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
			cout << "\t�Է�ѧ�Ų�����" << endl;
			continue;
		}
		if (checklist[2 * i + 1] != itsName) {
			cout << "\t�Է���������ȷ" << endl;
			continue;
		}
		bool checked = false;
		vector<string> itslist = get_checklist_of(itsNo, file);
		for (int j = 0; 2 * j + 1 < (int)itslist.size(); ++j) {
			if (itslist[2 * j] == stu) {
				checked = true;
				if (itslist[2 * j + 1] != get_stu_name(stu))
					cout << "\tûд��������";
				break;
			}
		}
		if (!checked)
			cout << "\t��������";
		cout << endl;
	}
	return 0;
}

/***************************************************************************
  ����������hw_check
  �������ƣ�
  ��    �ܣ����켰��������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
hw_check::hw_check(const string& _action, const string& _cno, const string& _stu, const string& _hwfile, const string& _cfgfile, int _chapter, int _week) :
	hw_check_stu(_action, _cno, _stu, _hwfile, _cfgfile, _chapter, _week) {};
hw_check::~hw_check() {};

/***************************************************************************
  ����������hw_check
  �������ƣ�
  ��    �ܣ���ҵ��麯��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void hw_check::checkAllFile() {
	CheckResult total_res;
	total_res = 0;
	int i = 1;
	vector<StuInfo> checklist;//������
	if (STU_ALL == stu_type) checklist = stu_list;
	else checklist.push_back(cur_stu);
	for (StuInfo stu : checklist) {
		cout << left << setw(2) << i++ << " : " << right;
		check_stu(stu.sno);
		cout << "���ͨ��" << cur_res.correct << '/' << cur_res.total << "���ļ�������ͨ��" << cur_res.correct << "��" << endl;
		print_result("ѧ����ϸ��Ϣ", cur_res, '-');
		cout << endl;
		total_res += cur_res;
	}
	cout << "�����" << stu_list.size() << "��ѧ���ļ�飬�ļ�����:" << total_res.total << "��ͨ������:" << total_res.correct << "������ͨ��" << total_res.correct << "��" << endl;
	print_result("������ϸ��Ϣ", total_res, '=');
	cout << endl;
}
void hw_check::checkOneFile() {
	cout << "�κ� : " << cno << " ѧ������ : " << stu_list.size() << " Դ�ļ��� : " << hwfile << endl;
	CheckResult total_res;
	total_res = 0;
	int i = 1;
	vector<StuInfo> checklist;//������
	if (STU_ALL == stu_type) checklist = stu_list;
	else checklist.push_back(cur_stu);
	for (StuInfo stu : checklist) {
		cout << left << setw(2) << i++ << " : " << right;
		check_stu(stu.sno);
		total_res += cur_res;
	}
	cout << endl;
	cout << "���ͨ��" << total_res.correct << '/' << total_res.total << "��ѧ��������ͨ��" << total_res.correct << "��" << endl;
	print_result("��ϸ��Ϣ", total_res, '=');
	cout << endl;
	if (CHECK_SECONDLINE == action) {
		cout << "����������" << endl;
		i = 1;
		for (StuInfo stu : stu_list) {
			cout << left << setw(2) << i++ << " : " << right;
			check_mutual(stu.sno, hwfile);
		}
	}
}
int hw_check::check() {
	if (CHECK_SECONDLINE == action && STU_ONE == stu_type) {
		cout << "HW_Check_SecondLineֻ�����ȫ��ѧ��" << endl;
		return -1;
	}
	if (STU_ONE == stu_type && "" == cur_stu.sno) {
		cout << "���β�����ѧ������Ϊ0/�ļ�����Ϊ0��δִ��" << endl;
		return -1;
	}
	if ("all" == hwfile)
		checkAllFile();
	else
		checkOneFile();
	return 0;
}