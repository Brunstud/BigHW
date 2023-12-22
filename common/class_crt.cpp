/* 2252201 ������ �ƿƶ��� */
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "../include/class_crt.h"
using namespace std;

/* ���������Ҫ��ͷ�ļ������� cfgfile_read_tools���ʵ�� */ 
#include <fstream>
#include <sstream>

/***************************************************************************
  �������ƣ�trim
  ��    �ܣ�����������ȥ���ַ�����β�ո���Ʊ��
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
static void trim(string& str) {
    str.erase(0, str.find_first_not_of(" \t")); // ȥ����ͷ�ո���Ʊ��
    str.erase(str.find_last_not_of(" \t") + 1); // ȥ����β�ո���Ʊ��
}

/***************************************************************************
  �������ƣ�
  ��    �ܣ��������������
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
// ���캯�� - char* ��ʽ���ļ���
cfgfile_read_tools::cfgfile_read_tools(const char* _cfgname) : cfgname(_cfgname ? _cfgname : "") {
    // ��ʼ���������ݳ�Ա
}

// ���캯�� - string ��ʽ���ļ���
cfgfile_read_tools::cfgfile_read_tools(const std::string& _cfgname) : cfgname(_cfgname) {
    // ��ʼ���������ݳ�Ա
}

// ��������
cfgfile_read_tools::~cfgfile_read_tools() {
    // �����������ͷ���Դ������ر��ļ���
    // �������Ҫ�����������������
}

/***************************************************************************
  �������ƣ�is_open
  ��    �ܣ��ж������ļ��Ƿ�򿪳ɹ�
  ���������
  �� �� ֵ��
  ˵    ����
***************************************************************************/
bool cfgfile_read_tools::is_open() {
    ifstream file(cfgname.c_str());
    bool open = file.is_open();
    file.close();
    return open;
}

/***************************************************************************
  �������ƣ�is_group
  ��    �ܣ���ȡline���ж��Ƿ�Ϊ���������޸�lineΪ��׼������ʽ
  ���������string& line
  �� �� ֵ���Ƿ�Ϊ����
  ˵    �������ڲ����ܺ���
***************************************************************************/
bool cfgfile_read_tools::is_group(string& line) {
    // ȥ��β��ע�Ͳ��ּ��س�
    line = line.substr(0, line.find_first_of("#;\r"));
    //ȥ��ǰ��ո��Ʊ��
    trim(line);
    // ��������
    size_t start = line.find_first_of("[");
    size_t end = line.find_last_of("]");
    if (!line.empty() && '[' == line.front() && ']' == line.back()) {
        //�ҵ�����
        string group_name = line.substr(start + 1, end - start - 1); // ������ [ �� ] ������
        trim(group_name); // ȥ������ǰ��Ŀո���Ʊ��
        line = '[' + group_name + ']';
        return true;
    }
    return false;
}

/***************************************************************************
  �������ƣ�is_item
  ��    �ܣ���ȡline���ж��Ƿ�Ϊ��Ŀ�����޸�lineΪ��׼��Ŀ��ʽ
  ���������string& line
  �� �� ֵ���Ƿ�Ϊ��Ŀ
  ˵    �������ڲ����ܺ���
***************************************************************************/
bool cfgfile_read_tools::is_item(string& line) {
    // ȥ��β��ע�Ͳ��ּ��س�
    line = line.substr(0, line.find_first_of("#;\r"));
    trim(line);
    // ��ȡ��Ŀ
    size_t equal_pos = line.find("=");
    if (equal_pos != string::npos)
        return true;
    return false;
}

/***************************************************************************
  �������ƣ�get_item
  ��    �ܣ����item����Ŀ��key����Ŀֵvalue
  ���������const string& line : item����
            string& key        : ��Ŀ��
            string& value      : ��Ŀֵ
  �� �� ֵ����
  ˵    �������ڲ����ܺ�������is_item()��ʹ��
***************************************************************************/
inline void cfgfile_read_tools::get_item(const string& line, string& key, string& value) {
    // ���а��յȺŷָ�Ϊ����ֵ
    istringstream iss(line);
    getline(iss, key, '=');
    getline(iss, value);

    // ȥ������ֵ����β�ո���Ʊ��
    trim(key);
    trim(value);
    return;
}

/***************************************************************************
  �������ƣ�is_equal
  ��    �ܣ����Ƿ���Դ�Сд�Ƚ�����/��Ŀ��
  ���������const std::string& aSTR
            const std::string& bSTR
            const bool ignore_lower_upper_case
  �� �� ֵ���Ƿ���ͬ
  ˵    �������ڲ����ܺ���������һ��char*�������
***************************************************************************/
inline bool cfgfile_read_tools::is_equal(const char* const aSTR, const string& bSTR, const bool ignore_lower_upper_case) {
    if (ignore_lower_upper_case) {
        // ��Сд�����бȽ�����
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
        // ��Сд�����бȽ�����
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
  �������ƣ�item_get_value
  ��    �ܣ��ҵ���Ӧ��Ŀ��ͨ��value����
  ���������const char* const group_name, 
            const char* const item_name, 
            std::string& value, 
            const bool ignore_lower_upper_case
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1
  ˵    �������ڲ����ܺ���
***************************************************************************/
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, string& _value, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // �ļ�δ�ɹ���
    }

    string line;
    bool group_found = !group_name;//��group_nameΪ�գ���Ϊ��ͨ�����ļ�
    bool item_found = false;

    while (getline(file, line)) {
        if (group_found) {
            if (is_item(line)) {
                // ���а��յȺŷָ�Ϊ����ֵ
                string key, value;
                get_item(line, key, value);
                //�ж��Ƿ�Ϊ������Ŀ
                if (is_equal(item_name, key, ignore_lower_upper_case)) {
                    _value = value;
                    item_found = true;
                    break;// �Ѿ��ҵ�Ŀ����Ŀ������ѭ��
                }
            }
            else if (group_name && is_group(line))
                break;// δ�ҵ�Ŀ����Ŀ������ѭ��
        }
        else if (is_group(line)) {
            //�жϵ�ǰ���Ƿ�ΪҪ����
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
    }

    file.close();
    return (item_found ? 0 : -1); // ���� 0 ��ʾ�ɹ��ҵ�Ŀ����Ŀ�����򷵻� -1 ��ʾδ�ҵ�
}

/***************************************************************************
  �������ƣ�get_all_group
  ��    �ܣ����ȡ�������������еģ�����string�͵�vector��
  ���������vector<string>& ret
  �� �� ֵ��
  ˵    ����
***************************************************************************/
int cfgfile_read_tools::get_all_group(vector<string>& ret) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // �ļ�δ�ɹ���
    }
    ret.clear(); // ��� ret ��ԭ����
    string line;
    while (getline(file, line)) {
        // ����ÿһ�У�����������������ret��
        if (is_group(line)) {
            ret.push_back(line);
        }
    }

    file.close();
    return ret.size(); // �����������
}

/***************************************************************************
  �������ƣ�get_all_item
  ��    �ܣ����ȡ���� - ȡĳ��group������item������string�͵�vector��
  ���������const char* const group_name, 
            vector <string>& ret, 
            const bool ignore_lower_upper_case = false
  �� �� ֵ���ɹ����ظ��������������ʧ�ܷ���-1
  ˵    ��������һ�����غ������ʺ�string���͵�group_name
***************************************************************************/
int cfgfile_read_tools::get_all_item(const char* const group_name, vector <string>& ret, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // �ļ�δ�ɹ���
    }

    string line;
    bool group_found = !group_name;//��group_nameΪ�գ���Ϊ��ͨ�����ļ�
    ret.clear(); // ��� ret ��ԭ����

    while (getline(file, line)) {
        if (group_name && is_group(line)) {
            //������һ�飬������ȡ
            if (group_found && group_name)
                break;
            //�жϵ�ǰ���Ƿ�ΪҪ����
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
        else if (group_found && is_item(line)) {
            ret.push_back(line);
        }
    }

    file.close();
    return (group_found ? ret.size() : -1); // ������Ŀ����
}
int cfgfile_read_tools::get_all_item(const string& group_name, vector <string>& ret, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // �ļ�δ�ɹ���
    }

    string line;
    bool group_found = false;
    ret.clear(); // ��� ret ��ԭ����

    while (getline(file, line)) {
        if (is_group(line)) {
            //������һ�飬������ȡ
            if (group_found)
                break;
            //�жϵ�ǰ���Ƿ�ΪҪ����
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
        else if (group_found && is_item(line)) {
            ret.push_back(line);
        }
    }

    file.close();
    return (group_found ? ret.size() : -1); // ������Ŀ����
}

/***************************************************************************
  �������ƣ�get_one_item
  ��    �ܣ����ȡ���� - ȡĳ��group������item������string�͵�vector��
  ���������const char* const group_name, 
            const char* const item_name, 
            string& ret, 
            const bool ignore_lower_upper_case = false
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1
  ?	    ������ʽ�ǡ����� = ֵ����������ʽ�����������д���
  ˵    ��������һ�����غ������ʺ�string���͵�group_name/item_name
 ***************************************************************************/
int cfgfile_read_tools::get_one_item(const char* const group_name, const char* const item_name, string& ret, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // �ļ�δ�ɹ���
    }

    string line;
    bool group_found = !group_name;//��group_nameΪ�գ���Ϊ��ͨ�����ļ�
    bool item_found = false;

    while (getline(file, line)) {
        if (group_found) {
            if (is_item(line)) {
                // ���а��յȺŷָ�Ϊ����ֵ
                string key, value;
                get_item(line, key, value);
                    //�ж��Ƿ�Ϊ������Ŀ
                    if (is_equal(item_name, key, ignore_lower_upper_case)) {
                        ret = line; // ������ʽΪ������ = ֵ����������ʽ
                        item_found = true;
                        break;// �Ѿ��ҵ�Ŀ����Ŀ������ѭ��
                    }
             }
            else if (group_name && is_group(line))
                break;// δ�ҵ�Ŀ����Ŀ������ѭ��
        }
        else if (is_group(line)) {
            //�жϵ�ǰ���Ƿ�ΪҪ����
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
    }

    file.close();
    return (item_found ? 0 : -1); // ���� 0 ��ʾ�ɹ��ҵ�Ŀ����Ŀ�����򷵻� -1 ��ʾδ�ҵ�
}
int cfgfile_read_tools::get_one_item(const string& group_name, const string& item_name, string& ret, const bool ignore_lower_upper_case) {
    ifstream file(cfgname.c_str());
    if (!file.is_open()) {
        return -1; // �ļ�δ�ɹ���
    }

    string line;
    bool group_found = false;
    bool item_found = false;

    while (getline(file, line)) {
        if (group_found) {
            if (is_item(line)) {
                // ���а��յȺŷָ�Ϊ����ֵ
                string key, value;
                get_item(line, key, value);
                //�ж��Ƿ�Ϊ������Ŀ
                if (is_equal(item_name, key, ignore_lower_upper_case)) {
                    ret = line; // ������ʽΪ������ = ֵ����������ʽ
                    item_found = true;
                    break;// �Ѿ��ҵ�Ŀ����Ŀ������ѭ��
                }
            }
            else if (is_group(line))
                break;// δ�ҵ�Ŀ����Ŀ������ѭ��
        }
        else if (is_group(line)) {
            //�жϵ�ǰ���Ƿ�ΪҪ����
            if (is_equal(group_name, line, ignore_lower_upper_case))
                group_found = true;
        }
    }

    file.close();
    return (item_found ? 0 : -1); // ���� 0 ��ʾ�ɹ��ҵ�Ŀ����Ŀ�����򷵻� -1 ��ʾδ�ҵ�
}

/***************************************************************************
  �������ƣ�item_get_value
  ��    �ܣ����ȡ���� - ���������Ͷ�ȡĳ��group��ĳ��item�����ڶ�Ӧ�������͵ı�����
  ���������const char* const group_name,
            const char* const item_name,
            const char* const def_str,
            char* str,
            const int str_maxlen,
            const bool ignore_lower_upper_case = true
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1
            ����ֵ����str�У��޶���󳤶ȣ���β�㣩Ϊstr_maxlen��������Ҫ��֤�����㹻�ռ䣬��Խ�磩
  ˵    ����ȡgroup_name����item_name��ֵ������Ϊchar *��ʽ
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
    return ret; // �ɹ�����0��ʧ�ܷ���-1
}

/***************************************************************************
  �������ƣ�item_get_value
  ��    �ܣ����ȡ���� - ���������Ͷ�ȡĳ��group��ĳ��item�����ڶ�Ӧ�������͵ı�����
  ���������const char* const group_name,
            const char* const item_name,
            const char* const def_str,
            string& value,
            const bool ignore_lower_upper_case = true
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1��
            ����ֵ����value��
  ˵    ����ȡgroup_name����item_name��ֵ������Ϊstring��ʽ
 ***************************************************************************/
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, string& value, const bool ignore_lower_upper_case) {
    string _value;
    int ret = item_get_value(group_name, item_name, _value, ignore_lower_upper_case);
    if (!ret && !_value.empty())
        value = _value;
    else
        value = def_str;
    return ret; // �ɹ�����0��ʧ�ܷ���-1
}

/***************************************************************************
  �������ƣ�item_get_value
  ��    �ܣ����ȡ���� - ���������Ͷ�ȡĳ��group��ĳ��item�����ڶ�Ӧ�������͵ı�����
  ���������const char* const group_name,
            const char* const item_name,
            char &ch,
            const bool ignore_lower_upper_case = true
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1��
            ����ֵ����ch��
  ˵    ����ȡgroup_name����item_name��ֵ������Ϊchar��ʽ�����ַ���
 ***************************************************************************/
int cfgfile_read_tools::item_get_value(const char* const group_name, const char* const item_name, char& ch, const bool ignore_lower_upper_case) {
    string _value;
    int ret = item_get_value(group_name, item_name, _value, ignore_lower_upper_case);
    if (!ret && !_value.empty())
        ch = _value[0];
    else
        ch = ' ';
    return ret; // �ɹ�����0��ʧ�ܷ���-1
}

/***************************************************************************
  �������ƣ�item_get_value
  ��    �ܣ����ȡ���� - ���������Ͷ�ȡĳ��group��ĳ��item�����ڶ�Ӧ�������͵ı�����
  ���������const char* const group_name,
            const char* const item_name,
            const int min_value,
            const int max_value,
            const int def_value,
            int& value,
            const bool ignore_lower_upper_case = true
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1��
            ����ֵ����value��
  ˵    �����������[min_value..max_value]��Χ��/ȡ����/���ݸ�ʽ����ȷ���򷵻�def_value
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
    return ret; // �ɹ�����0��ʧ�ܷ���-1
}

/***************************************************************************
  �������ƣ�item_get_value
  ��    �ܣ����ȡ���� - ���������Ͷ�ȡĳ��group��ĳ��item�����ڶ�Ӧ�������͵ı�����
  ���������const char* const group_name,
            const char* const item_name,
            const double min_value,
            const double max_value,
            const double def_value,
            double& value,
            const bool ignore_lower_upper_case = true
  �� �� ֵ���ɹ�����0��ʧ�ܷ���-1��
            ����ֵ����value��
  ˵    �����������[min_value..max_value]��Χ��/ȡ����/���ݸ�ʽ����ȷ���򷵻�def_value
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
    return ret; // �ɹ�����0��ʧ�ܷ���-1
}