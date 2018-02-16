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
		// �α������� ����� ���丮 path.
		void SetDirectory(std::string dirPath);
	private:
		std::ifstream ifstreamObj;
		std::ofstream ofstreamObj;
		std::string CreateLogMessage(LOG_TYPE type, std::string msg);
		// ���øӽ��� �������� Ÿ�ӽ������� �����ɴϴ�.
		std::string GetTimeStamp();
		std::vector<std::string> logFileBuffer;
		// �α������� ����� ���丮 path.
		std::string logFileDirPath;
		time_t systemTimer;
		tm timeInfo;
	};
}

