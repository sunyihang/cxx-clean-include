///<------------------------------------------------------------------------------
//< @file:   tool.h
//< @author: ������
//< @date:   2016��2��22��
//< @brief:  �������õ��ĸ��ֻ����ӿ�
//< Copyright (c) 2016. All rights reserved.
///<------------------------------------------------------------------------------

#ifndef _cxx_clean_tool_h_
#define _cxx_clean_tool_h_

#include <iterator>
#include <vector>

namespace llvm
{
	class raw_ostream;
}

using namespace std;

namespace strtool
{
	// �Ƿ�Ϊ�հ��ַ�
	inline bool is_blank(char c)
	{
		return (c == ' ' || c == '\t');
	}

	// �Ƿ�Ϊб�ܷ��ţ�\��/
	inline bool is_slash(char c)
	{
		return (c == '\\' || c == '/');
	}

	std::string itoa(int n);

	// �滻�ַ�����������ַ��������޸�
	// ���磺replace("this is an expmple", "is", "") = "th  an expmple"
	// ����: replace("acac", "ac", "ca") = "caca"
	string& replace(string &str, const char *old, const char* to);

	// ���ַ������ݷָ����ָ�Ϊ�ַ�������
	void split(const std::string src, std::vector<std::string> &strvec, char cut = ';');

	// �����ļ���·�������ؽ��ĩβ��/��\
	// ���磺get_dir(../../xxxx.txt) = ../../
	string get_dir(const string &path);

	// �Ƶ�·����ֻ�����ļ�����
	// ���磺../../xxxx.txt -> xxxx.txt
	string strip_dir(const string &path);

	// ��������ֱ��ָ���ָ������ַ���
	// ���磺r_trip_at("123_456", '_') = 123
	string trip_at(const string &str, char delimiter);

	// ��������ֱ��ָ���ָ������ַ���
	// ���磺r_trip_at("123_456", '_') = 456
	string r_trip_at(const string &str, char delimiter);

	// ��ȡ�ļ���׺
	// ���磺get_ext("../../abc.txt", '_') = txt
	string get_ext(const string &path);

	// ���ݴ���ĸ�ʽ�ı��Ͳ����������յ��ı���
	const char* get_text(const char* fmt, ...);

	// �Ƿ���ָ���ַ�����ͷ
	inline bool start_with(const string &text, const char *prefix)
	{
		int prefix_len	= strlen(prefix);
		int text_len	= text.length();

		if (prefix_len > text_len)
		{
			return false;
		}

		return 0 == strncmp(text.c_str(), prefix, prefix_len);
	}

	// �Ƿ���ָ���ַ�����β
	inline bool end_with(const string& text, const char* suffix)
	{
		int suffix_len	= strlen(suffix);
		int text_len	= text.length();

		if (suffix_len > text_len)
		{
			return false;
		}

		return 0 == strncmp(text.c_str() + text_len - suffix_len, suffix, suffix_len);
	}

	// �Ƿ����ָ���ַ�
	inline bool contain(const char *text, char x)
	{
		while (*text && *text != x) { ++text; }
		return *text == x;
	}

	// �Ƿ����ָ���ַ���
	inline bool contain(const std::string &text, const char *x)
	{
		return text.find(x) != std::string::npos;
	}

	// ����ָ��ǰ׺��ͷ�����Ƴ�ǰ׺������ʣ�µ��ַ���
	inline bool try_strip_left(string& str, const string& prefix)
	{
		if (strtool::start_with(str, prefix.c_str()))
		{
			str = str.substr(prefix.length());
			return true;
		}

		return false;
	}

	// ����ָ����׺��ͷ�����Ƴ���׺������ʣ�µ��ַ���
	inline bool try_strip_right(string& str, const string& suffix)
	{
		if (strtool::end_with(str, suffix.c_str()))
		{
			str = str.substr(0, str.length() - suffix.length());
			return true;
		}

		return false;
	}
}

using namespace strtool;

namespace pathtool
{
	// ��·��ת��linux·����ʽ����·���е�ÿ��'\'�ַ����滻Ϊ'/'
	string to_linux_path(const char *path);

	// ǿ�ƽ�·����/��β����·���е�ÿ��'\'�ַ����滻Ϊ'/'
	string fix_path(const string& path);

	// ����·����ȡ�ļ���
	// ���磺/a/b/foo.txt    => foo.txt
	string get_file_name(const string& path);

	// ��·��
	// ���磺d:/a/b/c/../../d/ -> d:/d/
	std::string simplify_path(const char* path);

	std::string append_path(const char* a, const char* b);

	/*
		��path_1Ϊ��ǰ·��������path_2�����·��
		���磺
			get_relative_path("d:/a/b/c/hello1.cpp", "d:/a/b/c/d/e/f/g/hello2.cpp") = d/e/f/g/hello2.cpp
			get_relative_path("d:/a/b/c/d/e/f/g/hello2.cpp", "d:/a/b/c/hello1.cpp") = ../../../../hello1.cpp

	*/
	std::string get_relative_path(const char *path_1, const char *path_2);

	/*
		���ؼ򻯺�ľ���·�������������·�������� = �򻯣���ǰ·�� + ���·���������������·������� = �򻯺�ľ���·��
		���磺
			���赱ǰ·��Ϊ��d:/a/b/c/
			get_absolute_path("../../d/e/hello2.cpp") = "d:/a/b/d/e/hello2.cpp"
			get_absolute_path("d:/a/b/c/../../d/") = "d:/a/d/"

	*/
	string get_absolute_path(const char *path);

	/*
		���ؼ򻯺�ľ���·������� = �򻯣�����·�� + ���·����
		���磺
			get_absolute_path("d:/a/b/c/", "../../d/") = "d:/a/d/"
	*/
	string get_absolute_path(const char *base_path, const char* relative_path);

	// ���ص�ǰ·��
	std::string get_current_path();

	// �ı䵱ǰ�ļ���
	bool cd(const char *path);

	// ָ��·���Ƿ����
	// ���磺dir = "../../example"
	bool is_dir_exist(const std::string &dir);

	// ָ��·���Ƿ���ڣ���Ϊ�ļ�·�������ļ���·��
	// ���磺path = "../../example"
	// ���磺path = "../../abc.xml"
	// ���磺path = "../../"
	bool exist(const std::string &path);

	// �г�ָ���ļ����µ��ļ����б������ļ��н������ԣ���������windows�������µ�dir
	// ���磺path = ../../*.*,   �� files = { "a.txt", "b.txt", "c.exe" }
	// ���磺path = ../../*.txt, �� files = { "a.txt", "b.txt" }
	typedef std::vector<string> filevec_t;
	bool dir(const std::string &path, /* out */filevec_t &files);

	// �ļ��Ƿ���ָ���ļ����£������ļ��У�
	bool is_at_folder(const char* folder, const char *file);

	// �г�ָ���ļ����µ��ļ����б��������ļ����µ��ļ���
	// ���磬����../../�����ļ�"a", "b", "c", "a.txt", "b.txt", "c.exe"
	//     ��path = ../../*.*,   �� files = { "a.txt", "b.txt", "c.exe" }
	//     ��path = ../../*.txt, �� files = { "a.txt", "b.txt" }
	typedef std::vector<string> FileVec;
	bool ls(const string &path, FileVec &files);
}

namespace cpptool
{
	// �Ƿ���c++ͷ�ļ���׺
	inline bool is_header(const std::string &ext)
	{
		// c++ͷ�ļ��ĺ�׺��h��hpp��hh
		return (ext == "h" || ext == "hpp" || ext == "hh");
	}

	// �Ƿ���c++Դ�ļ���׺
	inline bool is_cpp(const std::string &ext)
	{
		// c++Դ�ļ��ĺ�׺��c��cc��cpp��c++��cxx��m��mm
		return (ext == "c" || ext == "cc" || ext == "cpp" || ext == "c++" || ext == "cxx" || ext == "m" || ext == "mm");
	}
}

namespace htmltool
{
	std::string escape_html(const char* html);

	std::string escape_html(const std::string &html);

	std::string get_file_html(const std::string &filename);

	std::string get_include_html(const std::string &text);

	std::string get_number_html(int num);

	std::string get_warn_html(const char *text);

	std::string get_pre_html(const char *text);
}

namespace timetool
{
	std::string get_now(const char* format = "%04d/%02d/%02d-%02d:%02d:%02d");
}

namespace cxx
{
	void init_log(std::string log_path);

	llvm::raw_ostream& log();
}

#endif // _cxx_clean_tool_h_