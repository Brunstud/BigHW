/* 2252201 胡世成 计科二班 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include "../include/class_aat.h"
//如有必要，可以加入其它头文件
using namespace std;

#if !ENABLE_LIB_COMMON_TOOLS //不使用lib才有效

/* ---------------------------------------------------------------
	 允许加入其它需要static函数（内部工具用）
   ---------------------------------------------------------------- */
//各类型参数名称
static const char* ST_EXTARGS_TYPE_NAME[] = { "NULL", "Bool", "IntWithDefault", "IntWithError", "IntSETWithDefault", "IntSETWithError" , 
											"String", "StringSETWithDefault", "StringSETWithError", "IPAddrWithDefault", "IPAddrWithError", "TMAX", NULL};
// 将整数形式的 IP 地址转换为字符串形式，例如将 0x7f000001 转换为 "127.0.0.1"
static const string ipaddr2str(unsigned int ip_value)
{
	std::ostringstream oss;
	oss << ((ip_value >> 24) & 0xFF) << "."
		<< ((ip_value >> 16) & 0xFF) << "."
		<< ((ip_value >> 8) & 0xFF) << "."
		<< (ip_value & 0xFF);

	return oss.str(); //返回字符串形式IP
}
// 将字符串形式的 IP 地址转换为整型形式，例如将 "127.0.0.1" 转换为 0x7f000001，如果ip无效，返回0
static unsigned int ipaddr2int(std::string ip_value) {
	unsigned int ip = 0;
	int shift = 24;
	size_t start = 0;
	size_t pos = ip_value.find('.', start);
	int octetCount = 0; // 计数器，确保只有4个数字组成IP地址

	while (pos != std::string::npos && shift >= 0 && octetCount < 4) {
		unsigned int octet = 0;
		try {
			octet = std::stoi(ip_value.substr(start, pos - start));
		}
		catch (...) {
			// 异常处理：如果无法将子字符串转换为整数，则认为IP地址无效
			return 0;
		}

		if (octet > 255) {
			// 如果子串转换为整数后超出了合法范围，认为IP地址无效
			return 0;
		}

		ip |= (octet << shift);
		shift -= 8;
		start = pos + 1;
		pos = ip_value.find('.', start);
		++octetCount;
	}

	if (octetCount == 3) { // 检查是否有且仅有4个数字组成IP地址
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
		return 0; // 不是有效的IP地址格式
	}

	return ip; // 返回整型形式IP
}
//打印int_set范围
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
//打印string_int范围
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
//检测参数是否重复，正常返回0，重复返回-1
static int check_reArg(string args_name, int& args_existed)
{
	if (args_existed) {
		std::cout << "参数[" << args_name << "]重复." << endl;
		return -1;
	}
	else
		args_existed = 1;
	return 0;
}
//检查字符串是否为整数，是返回true，不是返回false
static inline bool str_is_int(const char* const str)
{
	for (const char* pc = str; !*pc; pc++) {
		if (*pc < '0' || *pc > '9')
			return false;
	}
	return true;
}
//检查int是否在set范围内，是返回true，不是返回false
static inline bool int_in_set(const int value, const int* const int_set)
{
	for (const int* pint = int_set; *pint != INVALID_INT_VALUE_OF_SET; pint++) {
		if (value == *pint)
			return true;
	}
	return false;
}
//检查字符串是否在set范围内，是返回true，不是返回false
static inline bool str_in_set(const string str, const string* const string_set)
{
	for (const string* pstr = string_set; *pstr != ""; pstr++) {
		if (str == *pstr)
			return true;
	}
	return false;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：null
 ***************************************************************************/
args_analyse_tools::args_analyse_tools()
{
	// 如果需要，执行初始化或设置任务
	args_name = ""; // 初始化字符串成员变量
	extargs_type = ST_EXTARGS_TYPE::null; // 初始化枚举成员变量
	extargs_num = 0; // 根据需要初始化其他成员变量
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：boolean
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const bool def)
{
	//检验参数类型
	if (type != ST_EXTARGS_TYPE::boolean)
		cout << "args_analyse构造函数类型错1(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//参数名称，类似于 "--help形式"
	extargs_type = type;	//本参数的额外参数的类型
	extargs_num = ext_num;	//额外参数的个数，0-1，其它暂未考虑

	extargs_bool_default = def;	//bool型额外参数的缺省值（default）

	args_existed = 0;	//本参数是否出现过（防止相同参数 -n ** -n ** 重复出现）
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_default、int_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def, const int _min, const int _max)
{
	//检验参数类型
	if (type != ST_EXTARGS_TYPE::int_with_default && type != ST_EXTARGS_TYPE::int_with_error)
		cout << "args_analyse构造函数类型错2(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//参数名称，类似于 "--help形式"
	extargs_type = type;	//本参数的额外参数的类型
	extargs_num = ext_num;	//额外参数的个数，0-1，其它暂未考虑

	extargs_int_value = extargs_int_default = def;	//int型额外参数的缺省值（default）
	extargs_int_min = _min;		//int型额外参数的最小值（min）
	extargs_int_max = _max;		//int型额外参数的最大值（max）

	args_existed = 0;	//本参数是否出现过（防止相同参数 -n ** -n ** 重复出现）
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：int_with_set_default、int_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const enum ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const int* const set)
{
	//检验参数类型
	if (type != ST_EXTARGS_TYPE::int_with_set_default && type != ST_EXTARGS_TYPE::int_with_set_error)
		cout << "args_analyse构造函数类型错3(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//参数名称，类似于 "--help形式"
	extargs_type = type;	//本参数的额外参数的类型
	extargs_num = ext_num;	//额外参数的个数，0-1，其它暂未考虑

	extargs_int_value = extargs_int_default = set[def_of_set_pos];	//int型额外参数的缺省值（default）
	extargs_int_set = set;		//int型额外参数的集合（提示：只是一个指针哦）

	args_existed = 0;	//本参数是否出现过（防止相同参数 -n ** -n ** 重复出现）
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str、ipaddr_with_default、ipaddr_with_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const string def)
{
	//检验参数类型
	if (type != ST_EXTARGS_TYPE::str && type != ST_EXTARGS_TYPE::ipaddr_with_default && type != ST_EXTARGS_TYPE::ipaddr_with_error)
		cout << "args_analyse构造函数类型错4(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//参数名称，类似于 "--help形式"
	extargs_type = type;	//本参数的额外参数的类型
	extargs_num = ext_num;	//额外参数的个数，0-1，其它暂未考虑

	extargs_string_value = extargs_string_default = def;	//string型额外参数的缺省值(含ipaddr)
	if (ST_EXTARGS_TYPE::ipaddr_with_default == type)
		extargs_ipaddr_default = ipaddr2int(def);

	args_existed = 0;	//本参数是否出现过（防止相同参数 -n ** -n ** 重复出现）
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：str_with_set_default、str_with_set_error
 ***************************************************************************/
args_analyse_tools::args_analyse_tools(const char* name, const ST_EXTARGS_TYPE type, const int ext_num, const int def_of_set_pos, const string* const set)
{
	//检验参数类型
	if (type != ST_EXTARGS_TYPE::str_with_set_default && type != ST_EXTARGS_TYPE::str_with_set_error)
		cout << "args_analyse构造函数类型错5(type=" << ST_EXTARGS_TYPE_NAME[(int)type] << ")" << endl;

	args_name = name;		//参数名称，类似于 "--help形式"
	extargs_type = type;	//本参数的额外参数的类型
	extargs_num = ext_num;	//额外参数的个数，0-1，其它暂未考虑

	extargs_string_value = extargs_string_default = set[def_of_set_pos];	//string型额外参数的缺省值(含ipaddr)
	extargs_string_set = set;		//string型额外参数的集合（提示：只是一个指针哦）

	args_existed = 0;	//本参数是否出现过（防止相同参数 -n ** -n ** 重复出现）
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：
 ***************************************************************************/
args_analyse_tools::~args_analyse_tools()
{
}

/* ---------------------------------------------------------------
	 允许AAT中自定义成员函数的实现（private）
   ---------------------------------------------------------------- */

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_name() const
{
	return this->args_name;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：加!!后，只能是0/1
			已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::existed() const
{
	return !!this->args_existed;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const int args_analyse_tools::get_int() const
{
	return this->extargs_int_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const string args_analyse_tools::get_string() const
{
	return this->extargs_string_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：已实现，不要动
 ***************************************************************************/
const unsigned int args_analyse_tools::get_ipaddr() const
{
	return this->extargs_ipaddr_value;
}

/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：将 extargs_ipaddr_value 的值从 0x7f000001 转为 "127.0.0.1"
 ***************************************************************************/
const string args_analyse_tools::get_str_ipaddr() const
{
	return ipaddr2str(extargs_ipaddr_value); //此句根据需要修改
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：follow_up_args：是否有后续参数
			0  ：无后续参数
			1  ：有后续参数
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_process(const int argc, const char* const *const argv, args_analyse_tools* const args, const int follow_up_args)
{
	// 此处可以添加代码来解析命令行参数，并根据需要进行后续操作
	const char* const* pcom = argv + 1;//记录当前读到的位置
	for (; pcom - argv < argc; pcom++) {
		int vaild = 0;
		for (args_analyse_tools* parg = args; parg->extargs_type != ST_EXTARGS_TYPE::null; parg++) {
			if (string(*pcom) == parg->args_name) {
				if (check_reArg(parg->args_name, parg->args_existed)) return -1;//查重
				vaild = 1;//表示指令有效
				switch (parg->extargs_type) {
					case ST_EXTARGS_TYPE::boolean:				//仅需要返回0/1，表示是否存在，例如：--help
						break;
					case ST_EXTARGS_TYPE::int_with_default:		//附加参数为整型，介于 min..max之间，有缺省值，若置超过上下限，则置缺省值
					case ST_EXTARGS_TYPE::int_with_error:	    //附加参数为整型，介于 min..max之间，有缺省值，若置超过上下限，则返回错误
						if (!str_is_int(*(++pcom))) {
							cout << "参数[" << parg->args_name << "]的附加参数不是整数. (类型:int, 范围[" << parg->extargs_int_min << ".." << parg->extargs_int_max << "]";
							if (ST_EXTARGS_TYPE::int_with_default == parg->extargs_type)
								cout << " 缺省:" << parg->extargs_int_default;
							cout << ")" << endl;
							return -1;
						}
						parg->extargs_int_value = atoi(*pcom);
						if (parg->extargs_int_value < parg->extargs_int_min || parg->extargs_int_value > parg->extargs_int_max) {
							if (ST_EXTARGS_TYPE::int_with_default == parg->extargs_type)
								parg->extargs_int_value = parg->extargs_int_default;
							else {
								cout << "参数[" << parg->args_name << "]的附加参数值(" << *pcom << ")非法. (类型:int, 范围[" << parg->extargs_int_min << ".." << parg->extargs_int_max << "])" << endl;
								return -1;
							}
						}
						break;
					case ST_EXTARGS_TYPE::int_with_set_default:	//附加参数为整型，只能属于一个有若干元素的集合[11/22/33]，有缺省值，若值不在集合中，则置缺省值
					case ST_EXTARGS_TYPE::int_with_set_error:	//附加参数为整型，只能属于一个有若干元素的集合[11/22/33]，有缺省值，若值不在集合中，则返回错误
						if (!str_is_int(*(++pcom))) {
							cout << "参数[" << parg->args_name << "]的附加参数不是整数. (类型:int, 范围[" << parg->extargs_int_min << ".." << parg->extargs_int_max << "]";
							if (ST_EXTARGS_TYPE::int_with_set_default == parg->extargs_type)
								cout << " 缺省:" << parg->extargs_int_default;
							cout << ")" << endl;
							return -1;
						}
						parg->extargs_int_value = atoi(*pcom);
						if (!int_in_set(parg->extargs_int_value, parg->extargs_int_set)) {
							if (ST_EXTARGS_TYPE::int_with_set_default == parg->extargs_type)
								parg->extargs_int_value = parg->extargs_int_default;
							else {
								cout << "参数[" << parg->args_name << "]的附加参数值(" << *pcom << ")非法. (类型:int, 可取值[";
								print_int_set(parg->extargs_int_set);
								cout << "])" << endl;
								return -1;
							}
						}
						break;
					case ST_EXTARGS_TYPE::str:					//附加参数为字符串
						parg->extargs_string_value = *(++pcom);
						break;
					case ST_EXTARGS_TYPE::str_with_set_default:	//附加参数为字符串，只能属于一个有若干元素的集合["11"/"22"/"33"]，有缺省值，若值不在集合中，则置缺省值
					case ST_EXTARGS_TYPE::str_with_set_error:	//附加参数为字符串，只能属于一个有若干元素的集合["11"/"22"/"33"]，有缺省值，若值不在集合中，则返回错误
						parg->extargs_string_value = string(*(++pcom));
						if (!str_in_set(parg->extargs_string_value, parg->extargs_string_set)) {
							if (ST_EXTARGS_TYPE::str_with_set_default == parg->extargs_type)
								parg->extargs_string_value = parg->extargs_string_default;
							else {
								cout << "参数[" << parg->args_name << "]的附加参数值(" << *pcom << ")非法. (类型:string, 可取值[";
								print_string_set(parg->extargs_string_set);
								cout << "])" << endl;
								return -1;
							}
						}
						break;
					case ST_EXTARGS_TYPE::ipaddr_with_default:	//附加参数为IP地址，有缺省值，返回一个u_int32
					case ST_EXTARGS_TYPE::ipaddr_with_error:	//附加参数为IP地址，无缺省值，返回错误
						parg->extargs_ipaddr_value = ipaddr2int(string(*(++pcom)));
						if (!parg->extargs_ipaddr_value) {
							if (ST_EXTARGS_TYPE::ipaddr_with_default == parg->extargs_type)
								parg->extargs_ipaddr_value = parg->extargs_ipaddr_default;
							else {
								cout << "参数[" << parg->args_name << "]的附加参数值(" << *pcom << ")非法. (类型:IP地址)" << endl;
								return -1;
							}
						}
						break;
					default:					//最大值，控制超限
						cout << "参数[" << parg->args_name << "]的类型[" << ST_EXTARGS_TYPE_NAME[(int)parg->extargs_type] << "]不存在!!";
						return -1;
				}
			}
			if (vaild)
				break;
		}
		if (!vaild)
			break;
	}

	// 根据 follow_up_args 的值判断是否有后续参数
	if (follow_up_args == 0) {
		// 无后续参数的处理逻辑
		// 可根据需求在这里执行特定的操作
		if (pcom - argv < argc) {
			cout << "参数[" << *pcom << "]非法." << endl;
			return -1;
		}
	}
	else if (follow_up_args == 1) {
		// 有后续参数的处理逻辑
		// 返回当前读到指令位置
		return pcom - argv;
	}

	// 返回值为0表示处理成功，根据实际情况返回不同的值来表示不同的处理结果
	return 0;  //此句根据需要修改
}


/***************************************************************************
  函数名称：
  功    能：
  输入参数：
  返 回 值：
  说    明：友元函数
***************************************************************************/
int args_analyse_print(const args_analyse_tools* const args)
{
	//找到value中string类最长长度
	unsigned int lenOfValue = 5;//默认为5
	for (const args_analyse_tools* parg = args; parg->extargs_type != ST_EXTARGS_TYPE::null; parg++)
		if (parg->existed()) {
			if (ST_EXTARGS_TYPE::str == parg->extargs_type || ST_EXTARGS_TYPE::str_with_set_default == parg->extargs_type || ST_EXTARGS_TYPE::str_with_set_error == parg->extargs_type) {
				if (parg->extargs_string_value.length() > lenOfValue) lenOfValue = parg->extargs_string_value.length();
			}
			else if (ST_EXTARGS_TYPE::ipaddr_with_default == parg->extargs_type || ST_EXTARGS_TYPE::ipaddr_with_error == parg->extargs_type)
				if (parg->get_str_ipaddr().length() > lenOfValue) lenOfValue = parg->get_str_ipaddr().length();
		}

	//开始打印
	std::cout << "==================================================================================================" << setw(lenOfValue) << setfill('=') << '=' << setfill(' ') << endl;
	std::cout << left <<  ' ' << setw(12) << "name" << setw(21) << "type" << setw(15) << "default" << setw(7) << "exists" << setw(lenOfValue + 1) << "value" << "range/set" << endl;
	std::cout << "==================================================================================================" << setw(lenOfValue) << setfill('=') << '=' << setfill(' ') << endl;

	// 在此处遍历 args_analyse_tools 对象并打印相应信息
	for (const args_analyse_tools* parg = args; parg->extargs_type != ST_EXTARGS_TYPE::null; parg++) {
		std::cout << ' ' << setw(12) << parg->get_name() << setw(21) << ST_EXTARGS_TYPE_NAME[(int)parg->extargs_type];
		// 这里需要根据实际类的成员函数实现，获取参数的名称、类型、默认值、存在性和值的范围或集合信息，并进行打印输出
		//默认值
		if (ST_EXTARGS_TYPE::boolean == parg->extargs_type)
			std::cout << setw(15) << (parg->extargs_bool_default ? "true" : "false");
		else if (ST_EXTARGS_TYPE::int_with_default == parg->extargs_type || ST_EXTARGS_TYPE::int_with_set_default == parg->extargs_type)
			std::cout << setw(15) << parg->extargs_int_default;
		else if (ST_EXTARGS_TYPE::str == parg->extargs_type || ST_EXTARGS_TYPE::str_with_set_default == parg->extargs_type || ST_EXTARGS_TYPE::ipaddr_with_default == parg->extargs_type)
			std::cout << setw(15) << ((parg->extargs_string_default != string("")) ? parg->extargs_string_default : "/");
		else
			std::cout << setw(15) << '/';
		//存在性
		std::cout << setw(7) << parg->existed();
		//值value
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
		//值的范围或集合
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
	return 0; //此句根据需要修改
}

#endif // !ENABLE_LIB_COMMON_TOOLS
