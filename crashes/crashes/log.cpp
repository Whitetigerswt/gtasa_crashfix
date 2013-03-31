#include "log.h"
#include <ctime>


Log::Log(char* filename) {
  m_stream.open(filename, fstream::in | fstream::out | fstream::app);
}

void Log::Write(char* logline){
	time_t rawtime;
	struct tm * timeinfo;

	time ( &rawtime );
	timeinfo = localtime ( &rawtime );

	char * time = asctime(timeinfo);
	time[strlen(time)-1] = '\0';

	char buf[256];
	sprintf(buf, "[%s] %s", time, logline);

    m_stream << buf << endl;
}

void Log::Write(char* logline, int notime){
	if(notime == 0) {
		m_stream << logline;
	} else {
		time_t rawtime;
		struct tm * timeinfo;

		time ( &rawtime );
		timeinfo = localtime ( &rawtime );

		char buf[256];
		sprintf(buf, "[%s] %s", asctime(timeinfo), logline);

		m_stream << buf << endl;
	}
}

Log::~Log(){
  m_stream.close();
}