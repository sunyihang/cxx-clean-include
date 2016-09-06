///<------------------------------------------------------------------------------
//< @file:   project.cpp
//< @author: 洪坤安
//< @brief:  本次清理c++的任务内容
//< Copyright (c) 2016. All rights reserved.
///<------------------------------------------------------------------------------

#include "project.h"

#include <llvm/Support/raw_ostream.h>

#include "tool.h"
#include "parser.h"
#include "html_log.h"

namespace cxxclean
{
	Project Project::instance;

	// 打印本次清理的文件列表
	void Project::Print() const
	{
		HtmlDiv div;
		div.AddTitle(cn_project_text);

		// 允许清理的c++文件列表
		if (!m_allowCleanList.empty())
		{
			div.AddRow(AddPrintIdx() + ". " + strtool::get_text(cn_project_allow_files, htmltool::get_number_html(m_allowCleanList.size()).c_str()));

			for (const string &file : m_allowCleanList)
			{
				div.AddRow(strtool::get_text(cn_project_allow_file, htmltool::get_file_html(file).c_str()), 2);
			}

			div.AddRow("");
		}

		// 允许清理的文件夹路径
		if (!m_allowCleanDir.empty())
		{
			div.AddRow(AddPrintIdx() + ". " + cn_project_allow_dir + m_allowCleanDir);
			div.AddRow("");
		}

		// 待清理的c++源文件列表
		if (!m_cpps.empty())
		{
			div.AddRow(AddPrintIdx() + ". " + strtool::get_text(cn_project_source_list, htmltool::get_number_html(m_cpps.size()).c_str()));

			for (const string &file : m_cpps)
			{
				const string absoluteFile = pathtool::get_absolute_path(file.c_str());
				div.AddRow(strtool::get_text(cn_project_source, htmltool::get_file_html(absoluteFile).c_str()), 2);
			}

			div.AddRow("");
		}

		HtmlLog::instance.AddDiv(div);
	}

	// 打印索引 + 1
	std::string Project::AddPrintIdx() const
	{
		return strtool::itoa(++m_printIdx);
	}

	// 生成允许清理文件列表
	void Project::GenerateAllowCleanList()
	{
		if (!m_allowCleanDir.empty())
		{
			return;
		}

		// 将待清理的.cpp文件存入可清理列表
		for (const string &cpp : m_cpps)
		{
			string absolutePath = pathtool::get_absolute_path(cpp.c_str());
			m_allowCleanList.insert(tolower(absolutePath.c_str()));
		}
	}

	// 指定的清理选项是否开启
	bool Project::IsCleanModeOpen(CleanMode mode)
	{
		if (mode <= 0 || mode > (int)m_cleanModes.size())
		{
			return false;
		}

		return m_cleanModes[mode - 1];
	}

	// 该文件是否允许被清理
	bool Project::CanClean(const char* filename) const
	{
		if (!m_allowCleanDir.empty())
		{
			return pathtool::is_at_folder(m_allowCleanDir.c_str(), filename);
		}
		else
		{
			return m_allowCleanList.find(tolower(filename)) != m_allowCleanList.end();
		}

		return false;
	}

	// 移除非c++后缀的源文件
	void Project::Fix()
	{
		// 遍历清理目标，将非c++后缀的文件从列表中移除
		for (int i = 0, size = m_cpps.size(); i < size;)
		{
			std::string &cpp = m_cpps[i];
			string ext = strtool::get_ext(cpp);

			if (cpptool::is_cpp(ext))
			{
				++i;
			}
			else
			{
				m_cpps[i] = m_cpps[--size];
				m_cpps.pop_back();
			}
		}

		// 附：
		//     对于允许清理的文件列表，则不检测后缀，因为很可能在某个cpp文件内有这样的语句：
		//         #include "common.def"
		//     甚至是:
		//		   #include "common.cpp"
	}
}