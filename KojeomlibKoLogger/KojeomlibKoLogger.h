#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <boost\format.hpp>
#ifdef KOJEOMLIB_LOGGER_EXPORTS  
#define KOJEOMLIB_LOGGER_API __declspec(dllexport)   
#else  
#define KOJEOMLIB_LOGGER_API __declspec(dllimport)   
#endif 

//http://www.cplusplus.com/doc/tutorial/files/

namespace KojeomlibLogger {
	enum LOG_TYPE {
		TYPE_WARNNING = 0,
		TYPE_ERROR = 1
	};
	class KoLogger {
	public:
		KoLogger();
		~KoLogger();
		void SetMsgToLogFileBuffer(std::string logMsg, LOG_TYPE type);
		void FlushLogBufferToFile();
		// 로그파일이 저장될 파일 path ( 파일이름 포함.)
		void SetLogFilePath(std::string pathWithFileName);
	private:
		std::ifstream ifstreamObj;
		std::ofstream ofstreamObj;
		std::string CreateLogMessage(LOG_TYPE type, std::string msg);
		std::vector<std::string> logFileBuffer;
		std::string logFilePath;
		time_t systemTimer;
		tm timeInfo;
	};
}

