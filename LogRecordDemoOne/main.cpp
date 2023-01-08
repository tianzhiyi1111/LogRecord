#include <iostream>
#include "MyLoggerUtil.h"
using namespace Logger;
int main()
{
	// 日志对象属性设置（包括文件大小等）,后续优化成配置文件形式
	Logger::MyLoggerUtil::GetInstance()->OpenLogFile("./test.log");
	Logger::MyLoggerUtil::GetInstance()->SetLogLevel(LogLevel::INFO);
	Logger::MyLoggerUtil::GetInstance()->SetFileMaxLength(1024);
	debug("test");
	debug("vary content %s, count is %d", "world", 5);
	info("test");
	info("vary content %s, count is %d", "world", 5);
	warning("test");
	warning("vary content %s, count is %d", "world", 5);
	error("test");
	error("vary content %s, count is %d", "world", 5);
	fatal("test");
	fatal("vary content %s, count is %d", "world", 5);
	
	return 0;
}