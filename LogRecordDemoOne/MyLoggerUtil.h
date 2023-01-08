#pragma once
#include <map>
#include <string>
#include <fstream>


namespace FileHelper
{
	class FileUtil
	{
	public:
		FileUtil();

		virtual ~FileUtil();

		// ���ļ�·���л�ȡ�ļ�����������׺��
		std::string GetFileNameFromPath(const std::string& strPath);

		// ���ļ�·���л�ȡ�ļ���������׺��
		std::string GetFileNameWithExFromPath(const std::string& strPath);

		// ���ļ�·���л�ȡ·���������ļ�����
		std::string GetFilePathFromPath(const std::string& strPath);

		// ���ļ�·���л�ȡ�ļ���չ��
		std::string GetExtendNameFromPath(const std::string& strPath);
	};
}


namespace Logger
{
#define debug(format, ...) \
	Logger::MyLoggerUtil::GetInstance()->LogRecord(Logger::LogLevel::DEBUG, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define info(format, ...) \
	Logger::MyLoggerUtil::GetInstance()->LogRecord(Logger::LogLevel::INFO, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define warning(format, ...) \
	Logger::MyLoggerUtil::GetInstance()->LogRecord(Logger::LogLevel::WARNING, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define error(format, ...) \
	Logger::MyLoggerUtil::GetInstance()->LogRecord(Logger::LogLevel::ERROR, __FILE__, __LINE__, format, ##__VA_ARGS__);
#define fatal(format, ...) \
	Logger::MyLoggerUtil::GetInstance()->LogRecord(Logger::LogLevel::FATAL, __FILE__, __LINE__, format, ##__VA_ARGS__);

	// ������־����
	enum class LogLevel
	{
		DEBUG = 0,
		INFO,
		WARNING,
		ERROR,
		FATAL
	};


	class MyLoggerUtil
	{
	public:
		// ��ȡ��������
		static MyLoggerUtil* GetInstance()
		{
			if (nullptr == pLoggerInstance)
			{
				pLoggerInstance = new MyLoggerUtil();
			}
			return pLoggerInstance;
		}


		virtual ~MyLoggerUtil();

		void OpenLogFile(const std::string& logFilePath);

		void CloseLogFile();

		// ��ֹ�����࿽��
		MyLoggerUtil(const MyLoggerUtil&) = delete;

		MyLoggerUtil& operator=(const  MyLoggerUtil&) = delete;

		void LogRecord(LogLevel level, const std::string& fileName, const int& lineNum,
			const char* contentFormat, ...);

		void SetLogLevel(LogLevel level);

		void SetFileMaxLength(const int& maxBytes);

		void LogFileRotate();  // ��־�ļ�����

	private:
		MyLoggerUtil();

		static MyLoggerUtil* pLoggerInstance;

		std::ofstream m_fOut;  // д���ļ���

		std::string m_logFilePath;

		LogLevel m_level;  // ����¼�������ϵ�  debug < info < warning < error < fatal

		int m_maxFileBytes; // �ļ����洢���ֽ���

		int m_FileLength; // �ļ���ǰ���ֽ���
	};
}

