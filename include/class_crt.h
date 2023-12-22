/* 2252201 ������ �ƿƶ��� */ 
#pragma once

/* �����������Ҫ��ͷ�ļ��� */ 
#include <string>
#include <vector>

/* ����������ļ��� */
class cfgfile_read_tools {
protected:
	std::string        cfgname; //�����ļ��� 

	/* �����������Ҫ�����ݳ�Ա����Ա���� */ 
	bool is_group(std::string& line);//��ȡline���ж��Ƿ�Ϊ���������޸�lineΪ��׼������ʽ
	bool is_item(std::string& line);//��ȡline���ж��Ƿ�Ϊ��Ŀ�����޸�lineΪ��׼��Ŀ��ʽ
	inline void get_item(const std::string& line, std::string& key, std::string& value);//���item����Ŀ��key����Ŀֵvalue
	inline bool is_equal(const char* const aSTR, const std::string& bSTR, const bool ignore_lower_upper_case);
	inline bool is_equal(const std::string& aSTR, const std::string& bSTR, const bool ignore_lower_upper_case);
	//�ҵ���Ӧ��Ŀ��ͨ��value����
	int item_get_value(const char* const group_name, const char* const item_name, std::string& _value, const bool ignore_lower_upper_case);
public:
	/* ������������� */ 
	cfgfile_read_tools(const char* _cfgname = "");
	cfgfile_read_tools(const std::string& _cfgname);
	virtual ~cfgfile_read_tools();

	/* �ж������ļ��Ƿ�򿪳ɹ� */
	bool is_open();

	/* ���ȡ�������������еģ�����string�͵�vector�� */
	int get_all_group(std::vector <std::string>& ret);

	/* ���ȡ���� - ȡĳ��group������item������string�͵�vector�� */
	int get_all_item(const char* const group_name, std::vector <std::string>& ret, const bool ignore_lower_upper_case = false);	//���Ժ�����ȱʡ���ִ�Сд
	int get_all_item(const std::string& group_name, std::vector <std::string>& ret, const bool ignore_lower_upper_case = false);

	/* ���ȡ���� - ȡĳ��group������item������string�� */
	int get_one_item(const char* const group_name, const char* const item_name, std::string &ret, const bool ignore_lower_upper_case = false);
	int get_one_item(const std::string& group_name, const std::string& item_name, std::string &ret, const bool ignore_lower_upper_case = false);

	/* ���ȡ���� - ���������Ͷ�ȡĳ��group��ĳ��item�����ڶ�Ӧ�������͵ı�����
		�������Ͱ�����int/double/string/char * /char */
	int item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, char* str, const int str_maxlen, const bool ignore_lower_upper_case = true);
	int item_get_value(const char* const group_name, const char* const item_name, const char* const def_str, std::string& value, const bool ignore_lower_upper_case = true);
	int item_get_value(const char* const group_name, const char* const item_name, char &ch, const bool ignore_lower_upper_case = true);
	int item_get_value(const char* const group_name, const char* const item_name, const int min_value, const int max_value, const int def_value, int& value, const bool ignore_lower_upper_case = true);
	int item_get_value(const char* const group_name, const char* const item_name, const double min_value, const double max_value, const double def_value, double& value, const bool ignore_lower_upper_case = true);
	
	/* �����������Ҫ����Ԫ������ */ 
	
};

