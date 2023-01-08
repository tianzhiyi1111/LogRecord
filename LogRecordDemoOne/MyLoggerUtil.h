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

		// 从文件路径中获取文件名（不带后缀）
		std::string GetFileNameFromPath(const std::string& strPath);

		// 从文件路径中获取文件名（带后缀）
		std::string GetFileNameWithExFromPath(const std::string& strPath);

		// 从文件路径中获取路径（不带文件名）
		std::string GetFilePathFromPath(const std::string& strPath);

		// 从文件路径中获取文件拓展名
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

	// 定义日志级别
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
		// 获取单例对象
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

		// 禁止单例类拷贝
		MyLoggerUtil(const MyLoggerUtil&) = delete;

		MyLoggerUtil& operator=(const  MyLoggerUtil&) = delete;

		void LogRecord(LogLevel level, const std::string& fileName, const int& lineNum,
			const char* contentFormat, ...);

		void SetLogLevel(LogLevel level);

		void SetFileMaxLength(const int& maxBytes);

		void LogFileRotate();  // 日志文件翻滚

	private:
		MyLoggerUtil();

		static MyLoggerUtil* pLoggerInstance;

		std::ofstream m_fOut;  // 写入文件流

		std::string m_logFilePath;

		LogLevel m_level;  // 仅记录级别以上的  debug < info < warning < error < fatal

		int m_maxFileBytes; // 文件最大存储的字节数

		int m_FileLength; // 文件当前的字节数
	};
}

