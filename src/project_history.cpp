///<------------------------------------------------------------------------------
//< @file:   project_history.cpp
//< @author: ������
//< @date:   2016��2��22��
//< @brief:  �����������ߵ���ʷͳ��
//< Copyright (c) 2016. All rights reserved.
///<------------------------------------------------------------------------------

#include "project_history.h"

#include "parsing_cpp.h"
#include "project.h"
#include "html_log.h"
#include "tool.h"

namespace cxxcleantool
{
	ProjectHistory ProjectHistory::instance;

	// ��ӡ��־
	void ProjectHistory::Print() const
	{
		if (m_files.empty())
		{
			return;
		}

		int canCleanFileCount = 0;

		for (auto itr : m_files)
		{
			const FileHistory &fileHistory = itr.second;

			if (fileHistory.IsNeedClean() && !fileHistory.m_compileErrorHistory.HaveFatalError())
			{
				canCleanFileCount += 1;
			}
		}

		HtmlLog::instance.AddBigTitle(cn_project_history_title);

		HtmlDiv &div = HtmlLog::instance.m_newDiv;
		div.Clear();

		div.AddTitle(strtool::get_text(cn_project_history_clean_count,	htmltool::get_number_html(canCleanFileCount).c_str()), 50);
		div.AddTitle(strtool::get_text(cn_project_history_src_count,	htmltool::get_number_html(Project::instance.m_cpps.size()).c_str()), 49);

		int i = 0;

		for (auto itr : m_files)
		{
			const FileHistory &history = itr.second;
			if (!history.IsNeedClean())
			{
				continue;
			}

			const char *tip = (history.m_compileErrorHistory.HaveFatalError() ? cn_file_history_compile_error : cn_file_history);

			div.AddRow(strtool::get_text(tip, htmltool::get_number_html(++i).c_str(), htmltool::get_file_html(history.m_filename).c_str()), 1);

			ParsingFile::PrintCompileError(history.m_compileErrorHistory);
			ParsingFile::PrintFileHistory(history);
		}

		HtmlLog::instance.AddDiv(div);
		HtmlLog::instance.EndLog();
	}

	// ��ӡ���� + 1
	std::string ProjectHistory::AddPrintIdx() const
	{
		return strtool::itoa(++m_printIdx);
	}

	void ProjectHistory::AddFile(ParsingFile *newFile)
	{
		newFile->MergeTo(m_files);
	}

	// ��ӡ�ļ���ʹ�ô�����δ���ƣ�
	void ProjectHistory::PrintCount() const
	{
		if (m_files.empty())
		{
			return;
		}

		cxx::log() << "\n    " << 3 << ". file be #include and be use count : file count = " << m_files.size() << "";

		for (auto fileItr : m_files)
		{
			const string &file		= fileItr.first;
			FileHistory &eachFile	= fileItr.second;

			if (!Project::instance.CanClean(file))
			{
				continue;
			}

			cxx::log() << "\n        file = {" << file << "} be use num = " << eachFile.m_beUseCount
			             << ", old be include num = " << eachFile.m_oldBeIncludeCount
			             << ", new be include num = " << eachFile.m_newBeIncludeCount;

			if (eachFile.m_beUseCount * 2 < eachFile.m_newBeIncludeCount)
			{
				cxx::log() << " -------> can move to other c++ file\n";
			}
			else
			{
				cxx::log() << "\n";
			}
		}
	}
}