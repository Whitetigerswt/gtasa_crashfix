#include "CLog.h"
#include <ctime>
#include <Windows.h>

CLog::CLog(char* filename) {
	m_stream.open(filename, std::ofstream::out | std::ofstream::app);
}

CLog::CLog(char* filename, bool append) {
	m_stream.open(filename, append == true ? std::ofstream::out | std::ofstream::app : std::ofstream::out);
}

void CLog::Write(std::string logline){
	time_t now = time(0);
	struct tm* tm = localtime(&now);
	m_stream << "[" << (tm->tm_year + 1900) << '/' << (tm->tm_mon + 1) << '/' << tm->tm_mday
		<< ' ' << tm->tm_hour << ':' << tm->tm_min << ':' << tm->tm_sec << "]: ";
	m_stream << logline << std::endl;
}

//void CLog::Write(std::string logline, int notime){
//	m_stream << logline;
//}

void CLog::Write(const char* format, ...)
{
	va_list vaArgs;
	char szBuffer[2048];
	va_start(vaArgs, format);
	vsnprintf(szBuffer, sizeof(szBuffer), format, vaArgs);
	va_end(vaArgs);

	m_stream << szBuffer << std::endl;
}

CLog::~CLog(){
	m_stream.close();
}