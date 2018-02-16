#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <stdlib.h>
#include <boost\format.hpp>
#ifdef KOJEOMLIB_LOGGER_EXPORTS  
#define KOJEOMLIB_LOGGER_API __declspec(dllexport)   
#else  
#define KOJEOMLIB_LOGGER_API __declspec(dllimport)   
#endif 

//http://www.cplusplus.com/doc/tutorial/files/

#define LOG_FILE_EXTENT ".txt"

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
		// 로그파일이 저장될 디렉토리 path.
		void SetDirectory(std::string dirPath);
	private:
		std::ifstream ifstreamObj;
		std::ofstream ofstreamObj;
		std::string CreateLogMessage(LOG_TYPE type, std::string msg);
		// 로컬머신을 기준으로 타임스탬프를 가져옵니다.
		std::string GetTimeStamp();
		std::vector<std::string> logFileBuffer;
		// 로그파일이 저장될 디렉토리 path.
		std::string logFileDirPath;
		time_t systemTimer;
		tm timeInfo;
	};
}

