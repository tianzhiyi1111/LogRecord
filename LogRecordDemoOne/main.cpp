#include <iostream>
#include "MyLoggerUtil.h"
using namespace Logger;
int main()
{
	// ��־�����������ã������ļ���С�ȣ�,�����Ż��������ļ���ʽ
	Logger::MyLoggerUtil::GetInstance()->OpenLogFile("./test.log");
	debug("test");
	debug("vary content %s, count is %d", "world", 5);
	
	return 0;
}