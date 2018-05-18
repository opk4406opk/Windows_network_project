#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <time.h>
#include <mutex>
#include <boost\format.hpp>

#define KOJEOMLIB_LOGGER_EXPORTS

#ifdef KOJEOMLIB_LOGGER_EXPORTS  
#define KOJEOMLIB_LOGGER_API __declspec(dllexport)   
#else  
#define KOJEOMLIB_LOGGER_API __declspec(dllimport)   
#endif 

//http://www.cplusplus.com/doc/tutorial/files/

#define LOG_FILE_EXTENT ".txt"

namespace KojeomlibLogger {
	enum class LOG_TYPE {
		TYPE_INFO,
		TYPE_WARNNING,
		TYPE_ERROR
	};
	class KoLogger {
	public:
		KoLogger();
		~KoLogger();
		// 메세지 로그를 Console 창으로 출력합니다. ( Not thread safe. )
		KOJEOMLIB_LOGGER_API void MsgGoToConsole(std::string logMsg, LOG_TYPE type);
		//메세지 로그를 Console 창으로 출력합니다. ( Thread safe. )
		KOJEOMLIB_LOGGER_API void MsgGoToConsoleSafely(std::string logMsg, LOG_TYPE type);
		// 메세지 로그를 파일 버퍼에 저장한다. ( Not thread safe.)
		KOJEOMLIB_LOGGER_API void SetMsgToLogFileBuffer(std::string logMsg, LOG_TYPE type);
		// 메세지 로그를 파일 버퍼에 저장한다. ( Thread safe. )
		KOJEOMLIB_LOGGER_API void SetMsgToLogFileBufferSafely(std::string logMsg, LOG_TYPE type);
		// 파일 버퍼를 외부 파일로 저장합니다. ( Not thread safe. )
		KOJEOMLIB_LOGGER_API void FlushLogBufferToFile();
		// 로그파일이 저장될 디렉토리 path.
		KOJEOMLIB_LOGGER_API void SetDirectory(std::string dirPath);
	private:
		std::ifstream ifstreamObj;
		std::ofstream ofstreamObj;
		std::string CreateLogMessage(LOG_TYPE type, std::string msg);
		// 로컬머신을 기준으로 타임스탬프를 가져옵니다.
		std::string GetTimeStamp();
		std::vector<std::string> logFileBuffer;
		// 로그파일이 저장될 디렉토리 path.
		std::string logFileDirPath;
		// 스레드 동기화에 쓰이는 mutex.
		std::mutex mutexObj;
	};
}

