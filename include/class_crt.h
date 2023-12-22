/* 2252201 胡世成 计科二班 */ 
#pragma once

/* 允许按需加入需要的头文件等 */ 
#include <string>
#include <vector>

/* 定义读配置文件类 */
class cfgfile_read_tools {
protected:
	std::string        cfgname; //配置文件名 

	/* 允许按需加入需要的数据成员及成员函数 */ 
	bool is_group(std::string& line);//读取line，判断是否为组名，并修改line为标准组名格式
	bool is_item(std::string& line);//读取line，判断是否为项目，并修改line为标准项目格式
	inline void get_item(const std::string& line, std::string& key, std::string& value);//获得item的项目名key及项目值value
	inline bool is_equal(const char* const aSTR, const std::string& bSTR, const bool ignore_lower_upper_case);
	inline bool is_equal(const std::string& aSTR, const std::string& bSTR, const bool ignore_lower_upper_case);
	//找到对应项目，通过value返回
	int item_get_value(const char* const group_name, const char* const item_name, std::string& _value, const bool ignore_lower_upper_case);
public:
	/* 构造和析构函数 */ 
	cfgfile_read_tools(const char* _cfgname = "");
	cfgfile_read_tools(const std::string& _cfgname);
	virtual ~cfgfile_read_tools();

	/* 判断配置文件是否打开成功 */
	bool is_open();

	/* 组读取函数，读出所有的，放在string型的vector中 */
	int get_all_group(std::vector <std::string>& ret);

	/* 项读取函数 - 取某个group的所有item，放在string型的vector中 */
	int get_all_item(const char* const group_name, std::vector <std::string>& ret, const bool ignore_lower_upper_case = false);	//测试函数中缺省区分大小写
	int get_all_item(const std::string& group_name, std::vector <std::string>& ret, const bool ignore_lower_upper_case = false);

	/* 项读取函数 - 取某个group的所有item，放在string中 */
	int get_one_item(const char* const group_name, const char* const item_name, std::string &ret, const bool ignore_lower_upper_case = false);
	int get_one_item(const std::string& group_name, const std::string& item_name, std::string &ret, const bool ignore_lower_upper_case = false);

	/* 项读取函数 - 按数据类型读取某个group的某个item，放在对应数据类型的变量中
		数据类型包括：int/double/string/char * /char */
	int item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, char* str, const int str_maxlen, const bool ignore_lower_upper_case = true);
	int item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, std::string& value, const bool ignore_lower_upper_case = true);
	int item_get_value(const char* const group_name, const char* const item_name, char &ch, const bool ignore_lower_upper_case = true);
	int item_get_value(const char* const group_name, const char* const item_name, const int min_value, const int max_value, const int def_value, int& value, const bool ignore_lower_upper_case = true);
	int item_get_value(const char* const group_name, const char* const item_name, const double min_value, const double max_value, const double def_value, double& value, const bool ignore_lower_upper_case = true);
	
	/* 允许按需加入需要的友元声明等 */ 
	
};

