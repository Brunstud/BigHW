/* 2252201 ������ �ƿƶ��� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../include/class_aat.h"
//���б�Ҫ�����Լ�������ͷ�ļ�
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS //��ʹ��lib����Ч

/* ---------------------------------------------------------------
	 �������������Ҫstatic�������ڲ������ã�
   ---------------------------------------------------------------- */
//�����Ͳ�������
static const char* ST_EXTARGS_TYPE_NAME[] = { "NULL", "Bool", "IntWithDefault", "IntWithError", "IntSETWithDefault", "IntSETWithError" , 
											"String", "StringSETWithDefault", "StringSETWithError", "IPAddrWithDefault", "IPAddrWithError", "TMAX", NULL};
// ��������ʽ�� IP ��ַת��Ϊ�ַ�����ʽ�����罫 0x7f000001 ת��Ϊ "127.0.0.1"
static const string ipaddr2str(unsigned int ip_value)
{
	std::ostringstream oss;
	oss << ((ip_value >> 24) & 0xFF) << "."
		<< ((ip_value >> 16) & 0xFF) << "."
		<< ((ip_value >> 8) & 0xFF) << "."
		<< (ip_value & 0xFF);

	return oss.str(); //�����ַ�����ʽIP
}
// ���ַ�����ʽ�� IP ��ַת��Ϊ������ʽ�����罫 "127.0.0.1" ת��Ϊ 0x7f000001�����ip��Ч������0
static unsigned int ipaddr2int(std::string ip_value) {
	unsigned int ip = 0;
	int shift = 24;
	size_t start = 0;
	size_t pos = ip_value.find('.', start);
	int octetCount = 0; // ��������ȷ��ֻ��4���������IP��ַ

	while (pos != std::string::npos && shift >= 0 && octetCount < 4) {
		unsigned int octet = 0;
		try {
			octet = std::stoi(ip_value.substr(start, pos - start));
		}
		catch (...) {
			// �쳣��������޷������ַ���ת��Ϊ����������ΪIP��ַ��Ч
			return 0;
		}

		if (octet > 255) {
			// ����Ӵ�ת��Ϊ�����󳬳��˺Ϸ���Χ����ΪIP��ַ��Ч
			return 0;
		}

		ip |= (octet << shift);
		shift -= 8;
		start = pos + 1;
		pos = ip_value.find('.', start);
		++octetCount;
	}

	if (octetCount == 3) { // ����Ƿ����ҽ���4���������IP��ַ
		unsigned int octet = 0;
		try {
			octet = std::stoi(ip_value.substr(start));
		}
		catch (...) {
			return 0;
		}

		if (octet > 255) {
			return 0;
		}

		ip |= (octet << shift);
	}
	else {
		return 0; // ������Ч��IP��ַ��ʽ
	}

	return ip; // ����������ʽIP
}
//��ӡint_set��Χ
static inline void print_int_set(const int* const int_set)
{
	for (const int* pint = int_set; ;) {
		std::cout << *pint;
		if (*(++pint) != INVALID_INT_VALUE_OF_SET)
			std::cout << '/';
		else
			break;
	}
}
//��ӡstring_int��Χ
static inline void print_string_set(const string* const int_set)
{
	for (const string* pstr = int_set; ;) {
		std::cout << *pstr;
		if (*(++pstr) != "")
			std::cout << '/';
		else
			break;
	}
}
//�������Ƿ��ظ�����������0���ظ�����-1
static int check_reArg(string args_name, int& args_existed)
{
	if (args_existed) {
		std::cout << "����[" << args_name << "]�ظ�." << endl;
		return -1;
	}
	else
		args_existed = 1;
	return 0;
}
//����ַ����Ƿ�Ϊ�������Ƿ���true�����Ƿ���false
static inline bool str_is_int(const char* const str)
{
	for (const char* pc = str; !*pc; pc++) {
		if (*pc < '0' || *pc > '9')
			return false;
	}
	return true;
}
//���int�Ƿ���set��Χ�ڣ��Ƿ���true�����Ƿ���false
static inline bool int_in_set(const int value, const int* const int_set)
{
	for (const int* pint = int_set; *pint != INVALID_INT_VALUE_OF_SET; pint++) {
		if (value == *pint)
			return true;
	}
	return false;
}
//����ַ����Ƿ���set��Χ�ڣ��Ƿ���true�����Ƿ���false
static inline bool str_in_set(const string str, const string* const string_set)
{
	for (const string* pstr = string_set; *pstr != ""; pstr++) {
		if (str == *pstr)
			return true;
	}
	return false;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	// �����Ҫ��ִ�г�ʼ������������
	args_name = ""; // ��ʼ���ַ�����Ա����
	extargs_type = ST_EXTARGS_TYPE::null; // ��ʼ��ö�ٳ�Ա����
	extargs_num = 0; // ������Ҫ��ʼ��������Ա����
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
	//�����������
	if (type != ST_EXTARGS_TYPE::boolean)
		cout << "args_analyse���캯�����ʹ�1(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//�������ƣ������� "--help��ʽ"
	extargs_type = type;	//�������Ķ������������
	extargs_num = ext_num;	//��������ĸ�����0-1��������δ����

	extargs_bool_default = def;	//bool�Ͷ��������ȱʡֵ��default��

	args_existed = 0;	//�������Ƿ���ֹ�����ֹ��ͬ���� -n ** -n ** �ظ����֣�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_default��int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
	//�����������
	if (type != ST_EXTARGS_TYPE::int_with_default && type != ST_EXTARGS_TYPE::int_with_error)
		cout << "args_analyse���캯�����ʹ�2(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//�������ƣ������� "--help��ʽ"
	extargs_type = type;	//�������Ķ������������
	extargs_num = ext_num;	//��������ĸ�����0-1��������δ����

	extargs_int_value = extargs_int_default = def;	//int�Ͷ��������ȱʡֵ��default��
	extargs_int_min = _min;		//int�Ͷ����������Сֵ��min��
	extargs_int_max = _max;		//int�Ͷ�����������ֵ��max��

	args_existed = 0;	//�������Ƿ���ֹ�����ֹ��ͬ���� -n ** -n ** �ظ����֣�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����int_with_set_default��int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
	//�����������
	if (type != ST_EXTARGS_TYPE::int_with_set_default && type != ST_EXTARGS_TYPE::int_with_set_error)
		cout << "args_analyse���캯�����ʹ�3(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//�������ƣ������� "--help��ʽ"
	extargs_type = type;	//�������Ķ������������
	extargs_num = ext_num;	//��������ĸ�����0-1��������δ����

	extargs_int_value = extargs_int_default = set[def_of_set_pos];	//int�Ͷ��������ȱʡֵ��default��
	extargs_int_set = set;		//int�Ͷ�������ļ��ϣ���ʾ��ֻ��һ��ָ��Ŷ��

	args_existed = 0;	//�������Ƿ���ֹ�����ֹ��ͬ���� -n ** -n ** �ظ����֣�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str��ipaddr_with_default��ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
	//�����������
	if (type != ST_EXTARGS_TYPE::str && type != ST_EXTARGS_TYPE::ipaddr_with_default && type != ST_EXTARGS_TYPE::ipaddr_with_error)
		cout << "args_analyse���캯�����ʹ�4(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//�������ƣ������� "--help��ʽ"
	extargs_type = type;	//�������Ķ������������
	extargs_num = ext_num;	//��������ĸ�����0-1��������δ����

	extargs_string_value = extargs_string_default = def;	//string�Ͷ��������ȱʡֵ(��ipaddr)
	if (ST_EXTARGS_TYPE::ipaddr_with_default == type)
		extargs_ipaddr_default = ipaddr2int(def);

	args_existed = 0;	//�������Ƿ���ֹ�����ֹ��ͬ���� -n ** -n ** �ظ����֣�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����str_with_set_default��str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
	//�����������
	if (type != ST_EXTARGS_TYPE::str_with_set_default && type != ST_EXTARGS_TYPE::str_with_set_error)
		cout << "args_analyse���캯�����ʹ�5(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//�������ƣ������� "--help��ʽ"
	extargs_type = type;	//�������Ķ������������
	extargs_num = ext_num;	//��������ĸ�����0-1��������δ����

	extargs_string_value = extargs_string_default = set[def_of_set_pos];	//string�Ͷ��������ȱʡֵ(��ipaddr)
	extargs_string_set = set;		//string�Ͷ�������ļ��ϣ���ʾ��ֻ��һ��ָ��Ŷ��

	args_existed = 0;	//�������Ƿ���ֹ�����ֹ��ͬ���� -n ** -n ** �ظ����֣�
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ����
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
}

/* ---------------------------------------------------------------
	 ����AAT���Զ����Ա������ʵ�֣�private��
   ---------------------------------------------------------------- */

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return this->args_name;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������!!��ֻ����0/1
			��ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!this->args_existed;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return this->extargs_int_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return this->extargs_string_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ʵ�֣���Ҫ��
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return this->extargs_ipaddr_value;
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������ extargs_ipaddr_value ��ֵ�� 0x7f000001 תΪ "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	return ipaddr2str(extargs_ipaddr_value); //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������follow_up_args���Ƿ��к�������
			0  ���޺�������
			1  ���к�������
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_process(const int argc, const char* const *const argv, args_analyse_tools* const args, const int follow_up_args)
{
	// �˴�������Ӵ��������������в�������������Ҫ���к�������
	const char* const* pcom = argv + 1;//��¼��ǰ������λ��
	for (; pcom - argv < argc; pcom++) {
		int vaild = 0;
		for (args_analyse_tools* parg = args; parg->extargs_type != ST_EXTARGS_TYPE::null; parg++) {
			if (string(*pcom) == parg->args_name) {
				if (check_reArg(parg->args_name, parg->args_existed)) return -1;//����
				vaild = 1;//��ʾָ����Ч
				switch (parg->extargs_type) {
					case ST_EXTARGS_TYPE::boolean:				//����Ҫ����0/1����ʾ�Ƿ���ڣ����磺--help
						break;
					case ST_EXTARGS_TYPE::int_with_default:		//���Ӳ���Ϊ���ͣ����� min..max֮�䣬��ȱʡֵ�����ó��������ޣ�����ȱʡֵ
					case ST_EXTARGS_TYPE::int_with_error:	    //���Ӳ���Ϊ���ͣ����� min..max֮�䣬��ȱʡֵ�����ó��������ޣ��򷵻ش���
						if (!str_is_int(*(++pcom))) {
							cout << "����[" << parg->args_name << "]�ĸ��Ӳ�����������. (����:int, ��Χ[" << parg->extargs_int_min << ".." << parg->extargs_int_max << "]";
							if (ST_EXTARGS_TYPE::int_with_default == parg->extargs_type)
								cout << " ȱʡ:" << parg->extargs_int_default;
							cout << ")" << endl;
							return -1;
						}
						parg->extargs_int_value = atoi(*pcom);
						if (parg->extargs_int_value < parg->extargs_int_min || parg->extargs_int_value > parg->extargs_int_max) {
							if (ST_EXTARGS_TYPE::int_with_default == parg->extargs_type)
								parg->extargs_int_value = parg->extargs_int_default;
							else {
								cout << "����[" << parg->args_name << "]�ĸ��Ӳ���ֵ(" << *pcom << ")�Ƿ�. (����:int, ��Χ[" << parg->extargs_int_min << ".." << parg->extargs_int_max << "])" << endl;
								return -1;
							}
						}
						break;
					case ST_EXTARGS_TYPE::int_with_set_default:	//���Ӳ���Ϊ���ͣ�ֻ������һ��������Ԫ�صļ���[11/22/33]����ȱʡֵ����ֵ���ڼ����У�����ȱʡֵ
					case ST_EXTARGS_TYPE::int_with_set_error:	//���Ӳ���Ϊ���ͣ�ֻ������һ��������Ԫ�صļ���[11/22/33]����ȱʡֵ����ֵ���ڼ����У��򷵻ش���
						if (!str_is_int(*(++pcom))) {
							cout << "����[" << parg->args_name << "]�ĸ��Ӳ�����������. (����:int, ��Χ[" << parg->extargs_int_min << ".." << parg->extargs_int_max << "]";
							if (ST_EXTARGS_TYPE::int_with_set_default == parg->extargs_type)
								cout << " ȱʡ:" << parg->extargs_int_default;
							cout << ")" << endl;
							return -1;
						}
						parg->extargs_int_value = atoi(*pcom);
						if (!int_in_set(parg->extargs_int_value, parg->extargs_int_set)) {
							if (ST_EXTARGS_TYPE::int_with_set_default == parg->extargs_type)
								parg->extargs_int_value = parg->extargs_int_default;
							else {
								cout << "����[" << parg->args_name << "]�ĸ��Ӳ���ֵ(" << *pcom << ")�Ƿ�. (����:int, ��ȡֵ[";
								print_int_set(parg->extargs_int_set);
								cout << "])" << endl;
								return -1;
							}
						}
						break;
					case ST_EXTARGS_TYPE::str:					//���Ӳ���Ϊ�ַ���
						parg->extargs_string_value = *(++pcom);
						break;
					case ST_EXTARGS_TYPE::str_with_set_default:	//���Ӳ���Ϊ�ַ�����ֻ������һ��������Ԫ�صļ���["11"/"22"/"33"]����ȱʡֵ����ֵ���ڼ����У�����ȱʡֵ
					case ST_EXTARGS_TYPE::str_with_set_error:	//���Ӳ���Ϊ�ַ�����ֻ������һ��������Ԫ�صļ���["11"/"22"/"33"]����ȱʡֵ����ֵ���ڼ����У��򷵻ش���
						parg->extargs_string_value = string(*(++pcom));
						if (!str_in_set(parg->extargs_string_value, parg->extargs_string_set)) {
							if (ST_EXTARGS_TYPE::str_with_set_default == parg->extargs_type)
								parg->extargs_string_value = parg->extargs_string_default;
							else {
								cout << "����[" << parg->args_name << "]�ĸ��Ӳ���ֵ(" << *pcom << ")�Ƿ�. (����:string, ��ȡֵ[";
								print_string_set(parg->extargs_string_set);
								cout << "])" << endl;
								return -1;
							}
						}
						break;
					case ST_EXTARGS_TYPE::ipaddr_with_default:	//���Ӳ���ΪIP��ַ����ȱʡֵ������һ��u_int32
					case ST_EXTARGS_TYPE::ipaddr_with_error:	//���Ӳ���ΪIP��ַ����ȱʡֵ�����ش���
						parg->extargs_ipaddr_value = ipaddr2int(string(*(++pcom)));
						if (!parg->extargs_ipaddr_value) {
							if (ST_EXTARGS_TYPE::ipaddr_with_default == parg->extargs_type)
								parg->extargs_ipaddr_value = parg->extargs_ipaddr_default;
							else {
								cout << "����[" << parg->args_name << "]�ĸ��Ӳ���ֵ(" << *pcom << ")�Ƿ�. (����:IP��ַ)" << endl;
								return -1;
							}
						}
						break;
					default:					//���ֵ�����Ƴ���
						cout << "����[" << parg->args_name << "]������[" << ST_EXTARGS_TYPE_NAME[(int)parg->extargs_type] << "]������!!";
						return -1;
				}
			}
			if (vaild)
				break;
		}
		if (!vaild)
			break;
	}

	// ���� follow_up_args ��ֵ�ж��Ƿ��к�������
	if (follow_up_args == 0) {
		// �޺��������Ĵ����߼�
		// �ɸ�������������ִ���ض��Ĳ���
		if (pcom - argv < argc) {
			cout << "����[" << *pcom << "]�Ƿ�." << endl;
			return -1;
		}
	}
	else if (follow_up_args == 1) {
		// �к��������Ĵ����߼�
		// ���ص�ǰ����ָ��λ��
		return pcom - argv;
	}

	// ����ֵΪ0��ʾ����ɹ�������ʵ��������ز�ͬ��ֵ����ʾ��ͬ�Ĵ�����
	return 0;  //�˾������Ҫ�޸�
}


/***************************************************************************
  �������ƣ�
  ��    �ܣ�
  ���������
  �� �� ֵ��
  ˵    ������Ԫ����
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
	//�ҵ�value��string�������
	unsigned int lenOfValue = 5;//Ĭ��Ϊ5
	for (const args_analyse_tools* parg = args; parg->extargs_type != ST_EXTARGS_TYPE::null; parg++)
		if (parg->existed()) {
			if (ST_EXTARGS_TYPE::str == parg->extargs_type || ST_EXTARGS_TYPE::str_with_set_default == parg->extargs_type || ST_EXTARGS_TYPE::str_with_set_error == parg->extargs_type) {
				if (parg->extargs_string_value.length() > lenOfValue) lenOfValue = parg->extargs_string_value.length();
			}
			else if (ST_EXTARGS_TYPE::ipaddr_with_default == parg->extargs_type || ST_EXTARGS_TYPE::ipaddr_with_error == parg->extargs_type)
				if (parg->get_str_ipaddr().length() > lenOfValue) lenOfValue = parg->get_str_ipaddr().length();
		}

	//��ʼ��ӡ
	std::cout << "==================================================================================================" << setw(lenOfValue) << setfill('=') << '=' << setfill(' ') << endl;
	std::cout << left <<  ' ' << setw(12) << "name" << setw(21) << "type" << setw(15) << "default" << setw(7) << "exists" << setw(lenOfValue + 1) << "value" << "range/set" << endl;
	std::cout << "==================================================================================================" << setw(lenOfValue) << setfill('=') << '=' << setfill(' ') << endl;

	// �ڴ˴����� args_analyse_tools ���󲢴�ӡ��Ӧ��Ϣ
	for (const args_analyse_tools* parg = args; parg->extargs_type != ST_EXTARGS_TYPE::null; parg++) {
		std::cout << ' ' << setw(12) << parg->get_name() << setw(21) << ST_EXTARGS_TYPE_NAME[(int)parg->extargs_type];
		// ������Ҫ����ʵ����ĳ�Ա����ʵ�֣���ȡ���������ơ����͡�Ĭ��ֵ�������Ժ�ֵ�ķ�Χ�򼯺���Ϣ�������д�ӡ���
		//Ĭ��ֵ
		if (ST_EXTARGS_TYPE::boolean == parg->extargs_type)
			std::cout << setw(15) << (parg->extargs_bool_default ? "true" : "false");
		else if (ST_EXTARGS_TYPE::int_with_default == parg->extargs_type || ST_EXTARGS_TYPE::int_with_set_default == parg->extargs_type)
			std::cout << setw(15) << parg->extargs_int_default;
		else if (ST_EXTARGS_TYPE::str == parg->extargs_type || ST_EXTARGS_TYPE::str_with_set_default == parg->extargs_type || ST_EXTARGS_TYPE::ipaddr_with_default == parg->extargs_type)
			std::cout << setw(15) << ((parg->extargs_string_default != string("")) ? parg->extargs_string_default : "/");
		else
			std::cout << setw(15) << '/';
		//������
		std::cout << setw(7) << parg->existed();
		//ֵvalue
		if (parg->existed()) {
			if (ST_EXTARGS_TYPE::boolean == parg->extargs_type)
				std::cout << setw(lenOfValue + 1) << "true";
			else if (ST_EXTARGS_TYPE::int_with_default == parg->extargs_type || ST_EXTARGS_TYPE::int_with_error == parg->extargs_type || ST_EXTARGS_TYPE::int_with_set_default == parg->extargs_type || ST_EXTARGS_TYPE::int_with_set_error == parg->extargs_type)
				std::cout << setw(lenOfValue + 1) << parg->extargs_int_value;
			else if (ST_EXTARGS_TYPE::str == parg->extargs_type || ST_EXTARGS_TYPE::str_with_set_default == parg->extargs_type || ST_EXTARGS_TYPE::str_with_set_error == parg->extargs_type)
				std::cout << setw(lenOfValue + 1) << parg->extargs_string_value;
			else if (ST_EXTARGS_TYPE::ipaddr_with_default == parg->extargs_type || ST_EXTARGS_TYPE::ipaddr_with_error == parg->extargs_type)
				std::cout << setw(lenOfValue + 1) << parg->get_str_ipaddr();
			else
				std::cout << setw(lenOfValue + 1) << '/';
		}
		else
			std::cout << setw(lenOfValue + 1) << '/';
		//ֵ�ķ�Χ�򼯺�
		if (ST_EXTARGS_TYPE::int_with_default == parg->extargs_type || ST_EXTARGS_TYPE::int_with_error == parg->extargs_type)
			std::cout << '[' << parg->extargs_int_min << ".." << parg->extargs_int_max << ']';
		else if (ST_EXTARGS_TYPE::int_with_set_default == parg->extargs_type || ST_EXTARGS_TYPE::int_with_set_error == parg->extargs_type)
			print_int_set(parg->extargs_int_set);
		else if (ST_EXTARGS_TYPE::str_with_set_default == parg->extargs_type || ST_EXTARGS_TYPE::str_with_set_error == parg->extargs_type)
			print_string_set(parg->extargs_string_set);
		else
			std::cout << '/';
		std::cout << endl;
	}

	std::cout << "==================================================================================================" << setw(lenOfValue) << setfill('=') << '=' << setfill(' ') << endl;
	cout << right << endl;
	return 0; //�˾������Ҫ�޸�
}

#endif // !ENABLE_LIB_COMMON_TOOLS
