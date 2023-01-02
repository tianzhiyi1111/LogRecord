#include "MyLoggerUtil.h"
#include <chrono>
#include <cstdarg>

namespace Logger
{

	// ������־�����Ӧ���ַ���
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
		m_fOut.open(logFilePath.c_str(), std::ios::app); // ��׷�ӵķ�ʽ���ļ�
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
		// �ȹر��ļ���
		CloseLogFile();
		// ��ȡ��ǰ��ʱ������������ݵ��ļ�
		time_t timeStamp;
		time(&timeStamp);
		tm timeInfo;
		localtime_s(&timeInfo, &timeStamp);
		std::string strCurTime = std::to_string(timeInfo.tm_year + 1900) + "-" + std::to_string(timeInfo.tm_mon + 1) +
			"-" + std::to_string(timeInfo.tm_mday) + " " + std::to_string(timeInfo.tm_hour) + ":" +
			std::to_string(timeInfo.tm_min) + ":" + std::to_string(timeInfo.tm_sec);

		std::string fileName = m_logFilePath + strCurTime;
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

		// �Ȼ�ȡ��ǰʱ�����ת��Ϊ�ַ�����д����־�ļ�
		time_t timeStamp;

		time(&timeStamp);
		tm timeInfo;
		localtime_s(&timeInfo, &timeStamp);
		std::string strCurTime = std::to_string(timeInfo.tm_year + 1900) + "-" + std::to_string(timeInfo.tm_mon + 1) +
			"-" + std::to_string(timeInfo.tm_mday) + " " + std::to_string(timeInfo.tm_hour) + ":" +
			std::to_string(timeInfo.tm_min) + ":" + std::to_string(timeInfo.tm_sec);


		// �ϲ��ļ������к�
		std::string combineStr = strCurTime + " " + MapLevel2Str[level] + " " + fileName + " " + std::to_string(lineNum);
		combineStr += " ";
		// ��ӡ��־���ݣ��ɱ����
		
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

