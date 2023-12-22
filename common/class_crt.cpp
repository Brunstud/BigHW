/* 2252201 胡世成 计科二班 */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "../include/class_crt.h"
using namespace std;

/* 允许加入需要的头文件，给出 cfgfile_read_tools类的实现 */ 
#include <fstream>
#include <sstream>

/***************************************************************************
  函数名称：trim
  功    能：辅助函数：去除字符串首尾空格和制表符
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
static void trim(string& str) {
    str.erase(0, str.find_first_not_of(" \t")); // 去除开头空格和制表符
    str.erase(str.find_last_not_of(" \t") + 1); // 去除结尾空格和制表符
}

/***************************************************************************
  函数名称：
  功    能：构造和析构函数
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
// 构造函数 - char* 形式的文件名
cfgfile_read_tools::cfgfile_read_tools(const char* _cfgname) : cfgname(_cfgname ? _cfgname : "") {
    // 初始化其他数据成员
}

// 构造函数 - string 形式的文件名
cfgfile_read_tools::cfgfile_read_tools(const std::string& _cfgname) : cfgname(_cfgname) {
    // 初始化其他数据成员
}

// 析构函数
cfgfile_read_tools::~cfgfile_read_tools() {
    // 可以在这里释放资源，比如关闭文件等
    // 如果有需要，进行其他清理操作
}

/***************************************************************************
  函数名称：is_open
  功    能：判断配置文件是否打开成功
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bool cfgfile_read_tools::is_open() {
    ifstream file(cfgname.c_str());
    bool open = file.is_open();
    file.close();
    return open;
}

/***************************************************************************
  函数名称：is_group
  功    能：读取line，判断是否为组名，并修改line为标准组名格式
  输入参数：string& line
  返 回 值：是否为组名
  说    明：类内部功能函数
***************************************************************************/
bool cfgfile_read_tools::is_group(string& line) {
    // 去除尾部注释部分及回车
    line = line.substr(0, line.find_first_of("#;\r"));
    //去除前后空格及制表符
    trim(line);
    // 查找组名
    size_t start = line.find_first_of("[");
    size_t end = line.find_last_of("]");
    if (!line.empty() && '[' == line.front() && ']' == line.back()) {
        //找到组名
        string group_name = line.substr(start + 1, end - start - 1); // 不包含 [ 和 ] 的组名
        trim(group_name); // 去除组名前后的空格和制表符
        line = '[' + group_name + ']';
        return true;
    }
    return false;
}

/***************************************************************************
  函数名称：is_item
  功    能：读取line，判断是否为项目，并修改line为标准项目格式
  输入参数：string& line
  返 回 值：是否为项目
  说    明：类内部功能函数
***************************************************************************/
bool cfgfile_read_tools::is_item(string& line) {
    // 去除尾部注释部分及回车
    line = line.substr(0, line.find_first_of("#;\r"));
    trim(line);
    // 读取项目
    size_t equal_pos = line.find("=");
    if (equal_pos != string::npos)
        return true;
    return false;
}

/***************************************************************************
  函数名称：get_item
  功    能：获得item的项目名key及项目值value
  输入参数：const string& line : item内容
            string& key        : 项目名
            string& value      : 项目值
  返 回 值：无
  说    明：类内部功能函数，在is_item()后使用
***************************************************************************/
inline void cfgfile_read_tools::get_item(const string& line, string& key, string& value) {
    // 将行按照等号分割为键和值
    istringstream iss(line);
    getline(iss, key, '=');
    getline(iss, value);

    // 去除键和值的首尾空格和制表符
    trim(key);
    trim(value);
    return;
}

/***************************************************************************
  函数名称：is_equal
  功    能：按是否忽略大小写比较组名/项目名
  输入参数：const std::string& aSTR
            const std::string& bSTR
            const bool ignore_lower_upper_case
  返 回 值：是否相同
  说    明：类内部功能函数，存在一个char*类的重载
***************************************************************************/
inline bool cfgfile_read_tools::is_equal(const char* const aSTR, const string& bSTR, const bool ignore_lower_upper_case) {
    if (ignore_lower_upper_case) {
        // 大小写不敏感比较组名
        if (bSTR.size() == strlen(aSTR) && equal(bSTR.begin(), bSTR.end(), aSTR, [](char a, char b) {
            return tolower(a) == tolower(b);
            })) {
            return true;
        }
    }
    else {
        if (bSTR == aSTR) {
            return true;
        }
    }
    return false;
}
inline bool cfgfile_read_tools::is_equal(const string& aSTR, const string& bSTR, const bool ignore_lower_upper_case) {
    if (ignore_lower_upper_case) {
        // 大小写不敏感比较组名
        if (bSTR.size() == aSTR.size() && equal(bSTR.begin(), bSTR.end(), aSTR.begin(), [](char a, char b) {
            return tolower(a) == tolower(b);
            })) {
            return true;
        }
    }
    else {
        if (bSTR == aSTR) {
            return true;
        }
    }
    return false;
}

/***************************************************************************
  函数名称：item_get_value
  功    能：找到对应项目，通过value返回
  输入参数：const char* const group_name, 
            const char* const item_name, 
            std::string& value, 
            const bool ignore_lower_upper_case
  返 回 值：成功返回0，失败返回-1
  说    明：类内部功能函数
***************************************************************************/
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, string& _value, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // 文件未成功打开
    }

    string line;
    bool group_found = !group_name;//若group_name为空，视为普通配置文件
    bool item_found = false;

    while (getline(file, line)) {
        if (group_found) {
            if (is_item(line)) {
                // 将行按照等号分割为键和值
                string key, value;
                get_item(line, key, value);
                //判断是否为所求项目
                if (is_equal(item_name, key, ignore_lower_upper_case)) {
                    _value = value;
                    item_found = true;
                    break;// 已经找到目标项目，结束循环
                }
            }
            else if (group_name && is_group(line))
                break;// 未找到目标项目，结束循环
        }
        else if (is_group(line)) {
            //判断当前组是否为要求组
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
    }

    file.close();
    return (item_found ? 0 : -1); // 返回 0 表示成功找到目标项目，否则返回 -1 表示未找到
}

/***************************************************************************
  函数名称：get_all_group
  功    能：组读取函数，读出所有的，放在string型的vector中
  输入参数：vector<string>& ret
  返 回 值：
  说    明：
***************************************************************************/
int cfgfile_read_tools::get_all_group(vector<string>& ret) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // 文件未成功打开
    }
    ret.clear(); // 清空 ret 的原内容
    string line;
    while (getline(file, line)) {
        // 处理每一行，如果是组名，则放入ret中
        if (is_group(line)) {
            ret.push_back(line);
        }
    }

    file.close();
    return ret.size(); // 返回组的数量
}

/***************************************************************************
  函数名称：get_all_item
  功    能：项读取函数 - 取某个group的所有item，放在string型的vector中
  输入参数：const char* const group_name, 
            vector <string>& ret, 
            const bool ignore_lower_upper_case = false
  返 回 值：成功返回该组中项的数量，失败返回-1
  说    明：还有一个重载函数，适合string类型的group_name
***************************************************************************/
int cfgfile_read_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // 文件未成功打开
    }

    string line;
    bool group_found = !group_name;//若group_name为空，视为普通配置文件
    ret.clear(); // 清空 ret 的原内容

    while (getline(file, line)) {
        if (group_name && is_group(line)) {
            //遇到下一组，结束读取
            if (group_found && group_name)
                break;
            //判断当前组是否为要求组
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
        else if (group_found && is_item(line)) {
            ret.push_back(line);
        }
    }

    file.close();
    return (group_found ? ret.size() : -1); // 返回项目数量
}
int cfgfile_read_tools::get_all_item(const string& group_name, vector <string>& ret, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // 文件未成功打开
    }

    string line;
    bool group_found = false;
    ret.clear(); // 清空 ret 的原内容

    while (getline(file, line)) {
        if (is_group(line)) {
            //遇到下一组，结束读取
            if (group_found)
                break;
            //判断当前组是否为要求组
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
        else if (group_found && is_item(line)) {
            ret.push_back(line);
        }
    }

    file.close();
    return (group_found ? ret.size() : -1); // 返回项目数量
}

/***************************************************************************
  函数名称：get_one_item
  功    能：项读取函数 - 取某个group的所有item，放在string型的vector中
  输入参数：const char* const group_name, 
            const char* const item_name, 
            string& ret, 
            const bool ignore_lower_upper_case = false
  返 回 值：成功返回0，失败返回-1
  ?	    返回形式是“项名 = 值”的完整形式（供后续自行处理）
  说    明：还有一个重载函数，适合string类型的group_name/item_name
 ***************************************************************************/
int cfgfile_read_tools::get_one_item(const char* const group_name, const char* const item_name, string& ret, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // 文件未成功打开
    }

    string line;
    bool group_found = !group_name;//若group_name为空，视为普通配置文件
    bool item_found = false;

    while (getline(file, line)) {
        if (group_found) {
            if (is_item(line)) {
                // 将行按照等号分割为键和值
                string key, value;
                get_item(line, key, value);
                    //判断是否为所求项目
                    if (is_equal(item_name, key, ignore_lower_upper_case)) {
                        ret = line; // 返回形式为“项名 = 值”的完整形式
                        item_found = true;
                        break;// 已经找到目标项目，结束循环
                    }
             }
            else if (group_name && is_group(line))
                break;// 未找到目标项目，结束循环
        }
        else if (is_group(line)) {
            //判断当前组是否为要求组
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
    }

    file.close();
    return (item_found ? 0 : -1); // 返回 0 表示成功找到目标项目，否则返回 -1 表示未找到
}
int cfgfile_read_tools::get_one_item(const string& group_name, const string& item_name, string& ret, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // 文件未成功打开
    }

    string line;
    bool group_found = false;
    bool item_found = false;

    while (getline(file, line)) {
        if (group_found) {
            if (is_item(line)) {
                // 将行按照等号分割为键和值
                string key, value;
                get_item(line, key, value);
                //判断是否为所求项目
                if (is_equal(item_name, key, ignore_lower_upper_case)) {
                    ret = line; // 返回形式为“项名 = 值”的完整形式
                    item_found = true;
                    break;// 已经找到目标项目，结束循环
                }
            }
            else if (is_group(line))
                break;// 未找到目标项目，结束循环
        }
        else if (is_group(line)) {
            //判断当前组是否为要求组
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
    }

    file.close();
    return (item_found ? 0 : -1); // 返回 0 表示成功找到目标项目，否则返回 -1 表示未找到
}

/***************************************************************************
  函数名称：item_get_value
  功    能：项读取函数 - 按数据类型读取某个group的某个item，放在对应数据类型的变量中
  输入参数：const char* const group_name,
            const char* const item_name,
            const char* const def_str,
            char* str,
            const int str_maxlen,
            const bool ignore_lower_upper_case = true
  返 回 值：成功返回0，失败返回-1
            返回值放在str中，限定最大长度（含尾零）为str_maxlen（调用者要保证传入足够空间，不越界）
  说    明：取group_name组中item_name的值，返回为char *形式
 ***************************************************************************/
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, char* str, const int str_maxlen, const bool ignore_lower_upper_case) {
    string _value;
    int ret = item_get_value(group_name, item_name, _value, ignore_lower_upper_case);
    if (!ret && !_value.empty()) {
        istringstream inval(_value);
        inval >> str;
        if (!inval)
            strncpy(str, def_str, str_maxlen);
    }
    else
        strncpy(str, def_str, str_maxlen);
    return ret; // 成功返回0，失败返回-1
}

/***************************************************************************
  函数名称：item_get_value
  功    能：项读取函数 - 按数据类型读取某个group的某个item，放在对应数据类型的变量中
  输入参数：const char* const group_name,
            const char* const item_name,
            const char* const def_str,
            string& value,
            const bool ignore_lower_upper_case = true
  返 回 值：成功返回0，失败返回-1，
            返回值放在value中
  说    明：取group_name组中item_name的值，返回为string形式
 ***************************************************************************/
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, string& value, const bool ignore_lower_upper_case) {
    string _value;
    int ret = item_get_value(group_name, item_name, _value, ignore_lower_upper_case);
    if (!ret && !_value.empty())
        value = _value;
    else
        value = def_str;
    return ret; // 成功返回0，失败返回-1
}

/***************************************************************************
  函数名称：item_get_value
  功    能：项读取函数 - 按数据类型读取某个group的某个item，放在对应数据类型的变量中
  输入参数：const char* const group_name,
            const char* const item_name,
            char &ch,
            const bool ignore_lower_upper_case = true
  返 回 值：成功返回0，失败返回-1，
            返回值放在ch中
  说    明：取group_name组中item_name的值，返回为char形式（单字符）
 ***************************************************************************/
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, char& ch, const bool ignore_lower_upper_case) {
    string _value;
    int ret = item_get_value(group_name, item_name, _value, ignore_lower_upper_case);
    if (!ret && !_value.empty())
        ch = _value[0];
    else
        ch = ' ';
    return ret; // 成功返回0，失败返回-1
}

/***************************************************************************
  函数名称：item_get_value
  功    能：项读取函数 - 按数据类型读取某个group的某个item，放在对应数据类型的变量中
  输入参数：const char* const group_name,
            const char* const item_name,
            const int min_value,
            const int max_value,
            const int def_value,
            int& value,
            const bool ignore_lower_upper_case = true
  返 回 值：成功返回0，失败返回-1，
            返回值放在value中
  说    明：如果不在[min_value..max_value]范围内/取不到/数据格式不正确，则返回def_value
 ***************************************************************************/
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const int min_value, const int max_value, const int def_value, int& value, const bool ignore_lower_upper_case) {
    string _value;
    int ret = item_get_value(group_name, item_name, _value, ignore_lower_upper_case);
    if (!ret && !_value.empty()) {
        istringstream inval(_value);
        inval >> value;
        if (!inval || value < min_value || value > max_value)
            value = def_value;
    }
    else
        value = def_value;
    return ret; // 成功返回0，失败返回-1
}

/***************************************************************************
  函数名称：item_get_value
  功    能：项读取函数 - 按数据类型读取某个group的某个item，放在对应数据类型的变量中
  输入参数：const char* const group_name,
            const char* const item_name,
            const double min_value,
            const double max_value,
            const double def_value,
            double& value,
            const bool ignore_lower_upper_case = true
  返 回 值：成功返回0，失败返回-1，
            返回值放在value中
  说    明：如果不在[min_value..max_value]范围内/取不到/数据格式不正确，则返回def_value
 ***************************************************************************/
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const double min_value, const double max_value, const double def_value, double& value, const bool ignore_lower_upper_case) {
    string _value;
    int ret = item_get_value(group_name, item_name, _value, ignore_lower_upper_case);
    if (!ret && !_value.empty()) {
        istringstream inval(_value);
        inval >> value;
        if (!inval || value < min_value || value > max_value)
            value = def_value;
    }
    else
        value = def_value;
    return ret; // 成功返回0，失败返回-1
}