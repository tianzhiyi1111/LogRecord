#include "MyLoggerUtil.h"
#include <chrono>
#include <cstdarg>

namespace Logger
{

	// 定义日志级别对应的字符串
	std::map<LogLevel, std::string> MapLevel2Str = {
		std::pair<LogLevel, std::string>(LogLevel::DEBUG, "Debug"),
		std::pair<LogLevel, std::string>(LogLevel::INFO, "Info"),
		std::pair<LogLevel, std::string>(LogLevel::WARNING, "Warning"),
		std::pair<LogLevel, std::string>(LogLevel::ERROR, "Error"),
		std::pair<LogLevel, std::string>(LogLevel::FATAL, "Fatal")
	};
	MyLoggerUtil* MyLoggerUtil::pLoggerInstance = nullptr;
	MyLoggerUtil::MyLoggerUtil() : m_level(LogLevel::DEBUG),m_maxFileBytes(0), m_FileLength(0)
	{

	}

	MyLoggerUtil::~MyLoggerUtil()
	{

	}

	void MyLoggerUtil::OpenLogFile(const std::string& logFilePath)
	{
		m_logFilePath = logFilePath;
		m_fOut.open(logFilePath.c_str(), std::ios::app); // 以追加的方式打开文件
		if (m_fOut.fail())
		{
			return;
		}
		m_fOut.seekp(0, std::ios::end);
		m_FileLength = m_fOut.tellp();
	}

	void MyLoggerUtil::CloseLogFile()
	{
		if (!m_fOut.fail())
		{
			m_fOut.close();
		}
	}

	void MyLoggerUtil::SetLogLevel(LogLevel level)
	{
		m_level = level;
	}

	void MyLoggerUtil::SetFileMaxLength(const int& maxBytes)
	{
		m_maxFileBytes = maxBytes;
	}

	void MyLoggerUtil::LogFileRotate()
	{
		// 先关闭文件流
		CloseLogFile();
		// 获取当前的时间戳，命名备份的文件
		time_t timeStamp;
		time(&timeStamp);
		tm timeInfo;
		localtime_s(&timeInfo, &timeStamp);
		std::string strCurTime = std::to_string(timeInfo.tm_year + 1900) + "_" + std::to_string(timeInfo.tm_mon + 1) +
			"_" + std::to_string(timeInfo.tm_mday) +"_"+ std::to_string(timeInfo.tm_hour) + "_" +
			std::to_string(timeInfo.tm_min) + "_" + std::to_string(timeInfo.tm_sec);

		FileHelper::FileUtil fileHelper;
		std::string path = fileHelper.GetFilePathFromPath(m_logFilePath);
		std::string extendName = fileHelper.GetExtendNameFromPath(m_logFilePath);

		std::string fileName = path + strCurTime + "." + extendName;
		// fileName = "se111.log";
		if (rename(m_logFilePath.c_str(), fileName.c_str()) != 0)
		{
			return;
		}
		OpenLogFile(m_logFilePath);


	}

	void MyLoggerUtil::LogRecord(LogLevel level, const std::string& fileName, const int& lineNum,
		const char* contentFormat, ...)
	{
		if (level < m_level)
		{
			return;
		}
		if (m_fOut.fail())
		{
			return;
		}

		// 先获取当前时间戳并转换为字符串，写入日志文件
		time_t timeStamp;

		time(&timeStamp);
		tm timeInfo;
		localtime_s(&timeInfo, &timeStamp);
		std::string strCurTime = std::to_string(timeInfo.tm_year + 1900) + "-" + std::to_string(timeInfo.tm_mon + 1) +
			"-" + std::to_string(timeInfo.tm_mday) + " " + std::to_string(timeInfo.tm_hour) + ":" +
			std::to_string(timeInfo.tm_min) + ":" + std::to_string(timeInfo.tm_sec);


		// 合并文件名及行号
		std::string combineStr = strCurTime + " " + MapLevel2Str[level] + " " + fileName + " " + std::to_string(lineNum);
		combineStr += " ";
		// 打印日志内容，可变参数
		
		va_list pArgs;
		va_start(pArgs, contentFormat);
		int size = vsnprintf(NULL, 0, contentFormat, pArgs);
		va_end(pArgs);
		if (size > 0)
		{
			char* buf = new char[(size_t)size + 1];
			memset(buf, 0, (size_t)size + 1);
			va_start(pArgs, contentFormat);
			vsnprintf(buf, (size_t)size + 1, contentFormat, pArgs);
			va_end(pArgs);
			
			combineStr += std::string(buf);
			
		}

		m_fOut << combineStr;
		m_FileLength += combineStr.length();
		m_fOut << "\n";
		m_fOut.flush();

		if (m_FileLength >= m_maxFileBytes && m_maxFileBytes > 0)
		{
			LogFileRotate();
		}
	}
}

namespace FileHelper
{
	FileUtil::FileUtil()
	{
	
	}

	FileUtil::~FileUtil()
	{
	
	}

	// 从文件路径中获取文件名（不带后缀）
	std::string FileUtil::GetFileNameFromPath(const std::string& strPath)
	{
		std::string strFileNameWithEx = GetFileNameWithExFromPath(strPath);
		for (int i = strFileNameWithEx.size() - 1; i >= 0; --i)
		{
			if (strPath[i] == '.')
			{
				return strPath.substr(0, (size_t)i + 1);
			}
		}
		return "";
	}

	// 从文件路径中获取文件名（带后缀）
	std::string FileUtil::GetFileNameWithExFromPath(const std::string& strPath)
	{
		for (int i = strPath.size() - 1; i >= 0; --i)
		{
			if (strPath[i] == '\\' || strPath[i] == '/')
			{
				return strPath.substr((size_t)i + 1);
			}
		}
		return "";
	}

	// 从文件路径中获取路径（不带文件名）
	std::string FileUtil::GetFilePathFromPath(const std::string& strPath)
	{
		for (int i = strPath.size() - 1; i >= 0; --i)
		{
			if (strPath[i] == '\\' || strPath[i] == '/')
			{
				return strPath.substr(0, (size_t)i + 1);
			}
		}
		return "";
	}

	// 从文件路径中获取文件拓展名
	std::string FileUtil::GetExtendNameFromPath(const std::string& strPath)
	{
		for (int i = strPath.size() - 1; i >= 0; --i)
		{
			if (strPath[i] == '.')
			{
				return strPath.substr((size_t)i + 1);
			}
		}
		return "";
	}
}


