#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
//���������Ҫ��ͷ�ļ� 
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
  �������ƣ�
  ��    �ܣ�
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

	msg << "Usage: " << procname << " --file1 xxx --file2 xxx [ --trim none/left/rigth/all | --lineskip 0..100 | --lineoffset -100..100 | --ignore_blank | --max_diff 0..100 | --display none/normal/detailed ]" << endl;

	msg << setw(wkey) << ' ' << "��ѡ�" << endl;
	msg << setw(wopt) << ' ' << "--file1 name1              : ָ��Ҫ�Ƚϵĵ�1���ļ�������ѡ��" << endl;
	msg << setw(wopt) << ' ' << "--file2 name2              : ָ��Ҫ�Ƚϵĵ�2���ļ�������ѡ��" << endl;
	msg << endl;

	msg << setw(wkey) << ' ' << "��ѡ�" << endl;
	msg << setw(wopt) << ' ' << "--trim none/left/right/all     : ָ��ÿ�еĿո�/tab�ĺ��Է�ʽ���޴�����Ĭ��Ϊnone��" << endl;
	msg << setw(wopt) << ' ' << "\tnone   : ÿ�о��ϸ�ƥ��" << endl;
	msg << setw(wopt) << ' ' << "\tleft   : ÿ�о�����ͷ���ո�" << endl;
	msg << setw(wopt) << ' ' << "\tright  : ÿ�о�����β���ո�" << endl;
	msg << setw(wopt) << ' ' << "\tall    : ÿ�о�����ͷβ�ո�" << endl;
	msg << setw(wopt) << ' ' << "--lineskip s                   : ָ���ļ���ǰs�в��Ƚϣ��޴�����sΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..100]����ʾͬʱ���������ļ���ǰs��" << endl;
	msg << setw(wopt) << ' ' << "--lineoffset n                 : ָ���ļ���λ���������޴�����nΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[-100..100]��������ʾ����file1��ǰn�У�������ʾ����file2��ǰn��" << endl;
	msg << setw(wopt) << ' ' << "--ignore_blank                 : �����ļ���--trim������п���(�޴����򲻺���)" << endl;
	msg << setw(wopt) << ' ' << "--CR_CRLF_not_equal            : ������Windows/Linux�ļ��Ļ��з�����(�޴�������Բ���)" << endl;
	msg << setw(wopt) << ' ' << "--max_diff m                   : ָ���ļ���λ���������޴�����mΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..100]����ʾ��m����ͬ����������У����ٽ��к����ıȽ�" << endl;
	msg << setw(wopt) << ' ' << "--max_line x                   : ָ���ļ��Ƚϵ�����������޴�����xΪ0��" << endl;
	msg << setw(wopt) << ' ' << "\tȡֵ[0..10000]����ʾ�Ƚ�x�к���������У����ٽ��к����ıȽ�" << endl;
	msg << setw(wopt) << ' ' << "--display none/normal/detailed : ָ����ʾ����Ϣ���޴�����Ĭ��Ϊnone��" << endl;
	msg << setw(wopt) << ' ' << "\tnone     : ��һ��ƥ��������ʾ" << endl;
	msg << setw(wopt) << ' ' << "\tnormal   : ÿ�������и���������Ϣ" << endl;
	msg << setw(wopt) << ' ' << "\tdetailed : ÿ�������и�������ϸ�Ĳ�����Ϣ" << endl;
	msg << endl;

	msg << "e.g.   " << procname << " --file1 my.txt --file2 std.txt" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim all" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ÿ�о�ȥ��ͷβ�ո񣬽�һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineoffset -2" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt������my.txt��ǰ2��(��my.txt�ĵ�3����std.txt�ĵ�1�н��бȽϣ�ÿ���ϸ�ƥ��)����һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --trim left --lineoffset 3" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt������std.txt��ǰ3��(��my.txt�ĵ�1����std.txt�ĵ�4�н��бȽ�)��ÿ��ȥ��ͷ���ո񣬽�һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt�������ļ��е����п��У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --lineskip 1" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt����һ�в��Ƚ�" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --ignore_blank --trim right" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt�������ļ���ȥ��ͷβ�ո������п��У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_diff 3" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��3�в�ƥ���������У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --max_line 7" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬���Ƚ�ǰ7�У���һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --CR_CRLF_not_queal" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬������Windows/Linux���ļ����죬��һ�����" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display normal" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��ƥ�������ʾ��Ϣ" << endl;
	msg << "       " << procname << " --file1 my.txt --file2 std.txt --display detailed" << endl;
	msg << setw(wopt) << ' ' << " : �Ƚ�my.txt��std.txt��ȫ��ƥ�䣬��ƥ�������ʾ��ϸ��Ϣ" << endl;
	msg << endl;

	/* һ��ȫ����ʾ���� */ 
	cout << msg.str() << endl;
}

/***************************************************************************
  �������ƣ�trim
  ��    �ܣ����ַ�����Ҫ�����trim����ͷȥβ������
  ���������string& str : ��Ҫtrim������ַ���
			const string& trim_type : trim����
			const bool CR_CRLF_not_equal : �Ƿ�����Windows/Linux��ʽ
  �� �� ֵ��������str
  ˵    ����
***************************************************************************/
string trim(string& str, const string &trim_type, const bool CR_CRLF_not_equal = false) {
	unsigned int first = 0, last = str.length();
	// �ҵ���һ���ǿո��ַ���λ��
	if (trim_type == "left" || trim_type == "all") {
		first = str.find_first_not_of(" \t");
		if (!CR_CRLF_not_equal)
			first = str.find_last_not_of(" \t\r");
		if (first == string::npos) {
			// ����ַ���ȫ�ǿո��򷵻ؿ��ַ���
			return (str = "");
		}
	}
	//ȥ��\r
	if (!CR_CRLF_not_equal)
		last = str.find_last_not_of("\r");
	// �ҵ����һ���ǿո��ַ���λ��
	if (trim_type == "right" || trim_type == "all")
		last = str.find_last_not_of(" \t\r");
	// ����ȥ��ǰ��ո�����ַ���
	return (str = str.substr(first, last - first + 1));
}

/***************************************************************************
  �������ƣ�findFirstDifference
  ��    �ܣ��Ƚ������ַ������ҳ�����֮���ڵڼ����ַ���ʼ���ֲ���
  ���������onst string& str1, const string& str2 : ��Ҫ�ȶԵ��ַ���
  �� �� ֵ�����ص�һ�������ַ���λ�ã���1��ʼ��
  ˵    ������ĳ���ַ���ĩβ������ʱ��
			str1�ϳ�������-1
			str2�ϳ�������-2
			������ȣ�����0����Ӧ�ô��ڣ�
***************************************************************************/
int findFirstDifference(const string& str1, const string& str2) {
	// ��ȡ�����ַ����н϶̵ĳ���
	unsigned int minLength = min(str1.length(), str2.length());

	// ���������ַ�����Ѱ�ҵ�һ����ͬ���ַ�λ��
	for (unsigned int i = 0; i < minLength; ++i) {
		if (str1[i] != str2[i]) {
			return (int)(i + 1); // ���ص�һ����ͬ�ַ���λ�ã���1��ʼ��
		}
	}

	// ������˴���˵��ǰ����ַ�����ͬ�������Ǹ��ַ����ϳ�
	if (str1.length() == str2.length())
		return 0;
	return (str1.length() > str2.length() ? -1 : -2);
}

/***************************************************************************
  �������ƣ�str2hex
  ��    �ܣ�16���Ʋ鿴�ַ���
  ���������const string &str : ��Ҫת�����ַ���
			const bool eof : ���eof
			const string &prompt : ��ʾ�ʣ�Ĭ����
  �� �� ֵ�����ص�һ�������ַ���λ�ã���1��ʼ��
  ˵    ������ĳ���ַ���ĩβ������ʱ��
			str1�ϳ�������-1
			str2�ϳ�������-2
			������ȣ�����0����Ӧ�ô��ڣ�
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
	int end = 0;//��ע�ĵ�����
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
  �������ƣ�show_both_line
  ��    �ܣ�����gotoxyͬʱ�������
  ���������const string& line1 : ��ʾ����
			const string& line2 : �Աȸ���
			const int eof : ���eof
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void show_both_line(const string& prompt, const string& line1, const string& line2, const int eof)
{
	int x, y;//���λ��
	cout << "�ļ�1 : ";
	cct_getxy(x, y);
	cct_gotoxy(0, y + 1);
	cout << "�ļ�2 : ";
	x++;
	unsigned int len = min(line1.length(), line2.length());
	for (unsigned int i = 0; i < len; ++i) {
		if (line1[i] != line2[i]) {
			cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
			cct_gotoxy(x, y);
			cout << (line1[i] != '\r' ? line1[i] : 'X');
			cct_gotoxy(x, y + 1);
			cout << (line2[i] != '\r' ? line2[i] : 'X');
			cct_setcolor();
		}
		else if (line1[i] < 0 && line1[i + 1] != line2[i + 1]) {
			// ������ַ�����������һ������ռ�����ַ�λ��
			char ch1 = line1[i++];
			cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
			cct_gotoxy(x, y);
			cout << ch1 << line1[i];
			cct_gotoxy(x, y + 1);
			cout << ch1 << line2[i];
			cct_setcolor();
		}
		else {  //�޲��죬��ͨ��ӡ
			cct_gotoxy(x, y);
			cout << (line1[i] != '\r' ? line1[i] : 'X');
			cct_gotoxy(x, y + 1);
			cout << (line2[i] != '\r' ? line2[i] : 'X');
		}
		x++;//���һλ��ӡ
	}
	cct_gotoxy(x, y);
	if (line1.length() == 0) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
		if (eof == 1)
			cout << "<EOF>";
		else {
			cout << "<EMPTY>";
		}
		cct_setcolor();
	}
	if (line1.length() > line2.length()) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
		for (int i = len; i < line1.length(); i++)
			cout << (line1[i] != '\r' ? line1[i] : 'X');
		//cout << line1.substr(len);
		cct_setcolor();
	}
	cout << (eof == 1 ? "" : "<CR>");
	cct_gotoxy(x, y + 1);
	if (line2.length() == 0) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
		if (eof == 2)
			cout << "<EOF>";
		else {
			cout << "<EMPTY>";
		}
		cct_setcolor();
	}
	if (line2.length() > line1.length()) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
		for (int i = len; i < line2.length(); i++)
			cout << (line2[i] != '\r' ? line2[i] : 'X');
		//cout << line2.substr(len);
		cct_setcolor();
	}
	cout << (eof == 2 ? "" : "<CR>") << endl;
}

/***************************************************************************
  �������ƣ�show_line
  ��    �ܣ���ǲ����ַ���ʾһ��
  ���������const string &prompt : ��ͷ��ʾ
			const string& line1 : ��ʾ����
			const string& line2 : �Աȸ���
			const bool eof : ���eof
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void show_line(const string &prompt ,const string& line1, const string& line2, const bool eof)
{
	cout << prompt;
	unsigned int len = min(line1.length(), line2.length());
	for (unsigned int i = 0; i < len; ++i) {
		if (line1[i] != line2[i]) {
			cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
			cout << (line1[i] != '\r' ? line1[i] : 'X');
			cct_setcolor();
		}
		else if (line1[i] > 0xc2 && line1[i] < 0xdf && line1[i + 1] != line2[i + 1]) {
			// ������ַ�����������һ������ռ�����ַ�λ��
			char ch1 = line1[i++];
			cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
			cout << ch1 << (line1[i] != '\r' ? line1[i] : 'X');
			cct_setcolor();
		}
		else {  //�޲��죬��ͨ��ӡ
			cout << (line1[i] != '\r' ? line1[i] : 'X');
		}
	}
	if (line1.length() == 0) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
		if (eof == 1)
			cout << "<EOF>";
		else {
			cout << "<EMPTY>";
		}
		cct_setcolor();
	}
	if (line1.length() > line2.length()) {
		cct_setcolor(COLOR_HYELLOW, COLOR_HRED);//������ǲ����ַ�
		for (int i = len; i < line1.length(); i++)
			cout << (line1[i] != '\r' ? line1[i] : 'X');
		//cout << line1.substr(len);
		cct_setcolor();
	}
	cout << (eof ? "" : "<CR>") << endl;
	return;
}

/***************************************************************************
  �������ƣ�display_line
  ��    �ܣ���ʾһ��
  ���������const string& line1, const string& line2 : �����ַ�
			const int lineNum1, const int lineNum2   : �ڼ���
			const int eof : ���eof���ļ�
			const string& display : ��ʾģʽ
  �� �� ֵ�����ַ�������ȷ���true
  ˵    ����
***************************************************************************/
bool display_line(const string& line1, const string& line2, const int lineNum1, const int lineNum2, const int eof, const string& display = "none", const string& trim_type = "none")
{
	const int diff_pos = findFirstDifference(line1, line2);
	if (!diff_pos && !eof) //�������ֱ�ӽ���
		return false;
	if (display == "none")//����ʾֱ�ӷ��ؽ��
		return true;
	//�������
	cout << "��[" << lineNum1 << " / " << lineNum2 << "]�� - ";
	if (!diff_pos && eof) {
		cout << "�ļ�1" << (eof == 1 ? "�ѽ���" : "��������") << "/�ļ�2" << (eof == 2 ? "�ѽ���" : "��������") << (trim_type != "none" ? "." : "��") << endl;
		if (trim_type != "none")
			return true;
	}
	else if (diff_pos > 0)
		cout << "��[" << diff_pos - 1 << "]���ַ���ʼ�в��죺" << endl;
	else
		cout << "�ļ�" << -diff_pos << "��β���ж����ַ���" << endl;
	//�����ӡ�̶ȳ�
	if (display == "detailed") {//��ϸ��ӡ
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
	//�����ɫ��ӡ
	show_line("�ļ�1 : ", line1, line2, eof == 1);
	show_line("�ļ�2 : ", line2, line1, eof == 2);
	//�����ӡhex
	if (display == "detailed") {
		str2hex(line1, (eof == 1), "�ļ�1(HEX) : ");
		str2hex(line2, (eof == 2), "�ļ�2(HEX) : ");
	}
	//��ɴ�ӡ
	cout << endl;
	return true;
}

/***************************************************************************
  �������ƣ�txt_compare
  ��    �ܣ��ļ��ȶԺ��ĺ���
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
void txt_compare(args_analyse_tools args[])
{
	ifstream infile1(args[OPT_ARGS_FILE1].get_string(), ios::in | ios::binary);
	ifstream infile2(args[OPT_ARGS_FILE2].get_string(), ios::in | ios::binary);
	if (!infile1.is_open() || !infile2.is_open()) {
		cerr << "���ļ�ʧ��" << endl;
		return;
	}
	//��������
	const int lineskip = args[OPT_ARGS_LINE_SKIP].get_int();//ok
	const int lineoffset = args[OPT_ARGS_LINE_OFFSET].get_int();//ok
	const string trim_type = args[OPT_ARGS_TRIM].get_string();//ok
	const bool ignore_blank = args[OPT_ARGS_IGNORE_BLANK].existed();//ok
	const bool CR_CRLF_not_equal = args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed();
	const int max_diff = args[OPT_ARGS_MAX_DIFFNUM].get_int();//ok
	const int max_line = args[OPT_ARGS_MAX_LINENUM].get_int();//ok
	const string display = args[OPT_ARGS_DISPLAY].get_string();//ok
	//��ʼ�ȶ�
	string line1, line2;
	int lineNum1 = 0, lineNum2 = 0;
	bool areDifferent = false;
	//������ͷ
	for (int i = 0; i < lineskip + (lineoffset < 0 ? -lineoffset : 0); ) {
		getline(infile1, line1);
		trim(line1, trim_type);//����trim
		lineNum1++;
		if (ignore_blank && !line1[0])
			continue;
		i++;
	}
	for (int i = 0; i < lineskip + (lineoffset > 0 ? lineoffset : 0); ) {
		getline(infile2, line2);
		trim(line2, trim_type);//����trim
		lineNum2++;
		if (ignore_blank && !line2[0])
			continue;
		i++;
	}
	//��ӡ��ʾ
	if (display != "none") {
		cout << "�ȽϽ�������" << endl;
		cout << "====================================================================================================" << endl;
	}
	//��ʼ�ȶ�
	int times = 0, difftimes = 0, eof = 0;
	while (1) {
		if (eof)
			break;
		eof = (infile1.eof() && !infile2.eof() ? 1 : (infile2.eof() && !infile1.eof() ? 2 : 0));
		//����
		do {
			getline(infile1, line1);
			trim(line1, trim_type, CR_CRLF_not_equal);//����trim
			lineNum1++;
		} while (ignore_blank && !line1.length() && !infile1.eof());
		if (infile1.eof() && !eof && !line1.length())
			eof = 1;
		do {
			getline(infile2, line2);
			trim(line2, trim_type, CR_CRLF_not_equal);//����trim
			lineNum2++;
		} while (ignore_blank && !line2.length() && !infile2.eof());
		if (infile2.eof() && !eof && !line2.length())
			eof = 2;
		if (infile1.eof() && infile2.eof())
			break;
		//������
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
	//�����ж�
	if (display == "none") {
		if (areDifferent)
			cout << "�ļ���ͬ." << endl;
		else
			cout << "�ļ���ͬ." << endl;
	}
	else {
		if (areDifferent) {
			cout << "====================================================================================================" << endl;
			cout << "��ָ����������¹�" << difftimes << "���в���." << endl;
			cout << "�Ķ���ʾ��" << endl;
			cout << "	1��������<EMPTY>���" << endl;
			cout << "	2���ļ�������<EOF>���" << endl;
			cout << "	3��������ͬ��λ�õĲ����ַ�����ɫ���" << endl;
			cout << "	4��ÿ���е�CR/LF/VT/BS/BEL��X���(���㿴�������ַ�)" << endl;
			cout << "	5��ÿ��β�Ķ�����ַ�����ɫ�����CR/LF/VT/BS/BEL����ɫX���(���㿴�������ַ�)" << endl;
			cout << "	6��ÿ�������<CR>���(���㿴�������ַ�)" << endl;
			cout << "	7��������Ϊ�������⣬����λ�ÿ��ܱ��ں��������ϣ����������ֶ���ɫ���" << endl;
			if (display != "detailed")
				cout << "	8����--display detailed���Եõ�����ϸ����Ϣ" << endl;
		}
		else
			cout << "��ָ�������������ȫһ��." << endl;
		cout << "====================================================================================================" << endl;
	}
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

	if (args[OPT_ARGS_FILE1].existed() + args[OPT_ARGS_FILE2].existed() != 2) {
		usage(argv[0]);
		cout << "����ָ������[" << args[OPT_ARGS_FILE1].get_name() << "��"<< args[OPT_ARGS_FILE2].get_name() << "]" << endl;
		return -1;
	}

	string trim_type = args[OPT_ARGS_TRIM].get_string();
	if (args[OPT_ARGS_CR_CRLF_NOT_EQUAL].existed() && (trim_type == "right" || trim_type == "all")) {
		usage(argv[0]);
		cout << "����[" << args[OPT_ARGS_CR_CRLF_NOT_EQUAL].get_name() << "]���ܺ�[" << args[OPT_ARGS_TRIM].get_name() << " right/all]ͬʱ����." << endl;
		return -1;
	}

	if (args[OPT_ARGS_DEBUG].existed())
		args_analyse_print(args);

	/* ����ʵ�ʵĹ��ܵ��ã������Ӧ�Ĺ��� */ 
	txt_compare(args);

	return 0;
}

/* 
�Ķ���ʾ��
1�������� <EMPTY> ���
2���ļ������� <EOF> ���
3��������ͬ��λ�õĲ����ַ�����ɫ���
4��ÿ���е�CR/LF/VT/BS/BEL��X���(���㿴�������ַ�)
5��ÿ��β�Ķ�����ַ�����ɫ�����CR/LF/VT/BS/BEL����ɫX���(���㿴�������ַ�)
6��ÿ������� <CR> ���(���㿴�������ַ�)"
7��������Ϊ�������⣬����λ�ÿ��ܱ��ں��������ϣ����������ֶ���ɫ���
8����--display detailed���Եõ�����ϸ����Ϣ
*/ 

#if 0
    //�ָ���===�Ŀ�ȼ��� 
	int width = (this->line_maxlen / 10 + 1) * 10 + 8 + 2; //��---�ı�߶�2����line_maxlen����ȡ10��������8��"�ļ�1 : "

	/* �������hex���������С���Ϊ80 */
	if (this->display_detailed && width < 80)
		width = 80;
#endif

/* �����ļ�׼��
	1.txt �������ļ�
	2.txt ����1.txt�Ļ����ϣ�ĳЩ��ǰ���޿ո�
	3.txt ����1.txt�Ļ����ϣ�ĳЩ�к����޿ո�
	4.txt ����1.txt�Ļ����ϣ����һ��û�лس�
	5.txt ����1.txt�Ļ����ϣ���һЩ����
	6.txt ����1.txt�Ļ����ϣ���һЩ���У�����ĳЩ���пո��tab
	7.txt ����1.txt��ȫ��ͬ
	8.txt ����1.txt��Linux��ʽ��

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

rem ����ͬ������
txt_compare --file1 1.txt --file2 2.txt
txt_compare --file1 1.txt --file2 2.txt --trim right
txt_compare --file1 1.txt --file2 5.txt
txt_compare --file1 1.txt --file2 5.txt --trim right
txt_compare --file1 1.txt --file2 6.txt
txt_compare --file1 1.txt --file2 6.txt --ignore_blank
txt_compare --file1 1.txt --file2 8.txt --CR_CRLF_not_equal

*/
