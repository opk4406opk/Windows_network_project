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
		// �޼��� �α׸� Console â���� ����մϴ�. ( Not thread safe. )
		KOJEOMLIB_LOGGER_API void MsgGoToConsole(std::string logMsg, LOG_TYPE type);
		//�޼��� �α׸� Console â���� ����մϴ�. ( Thread safe. )
		KOJEOMLIB_LOGGER_API void MsgGoToConsoleSafely(std::string logMsg, LOG_TYPE type);
		// �޼��� �α׸� ���� ���ۿ� �����Ѵ�. ( Not thread safe.)
		KOJEOMLIB_LOGGER_API void SetMsgToLogFileBuffer(std::string logMsg, LOG_TYPE type);
		// �޼��� �α׸� ���� ���ۿ� �����Ѵ�. ( Thread safe. )
		KOJEOMLIB_LOGGER_API void SetMsgToLogFileBufferSafely(std::string logMsg, LOG_TYPE type);
		// ���� ���۸� �ܺ� ���Ϸ� �����մϴ�. ( Not thread safe. )
		KOJEOMLIB_LOGGER_API void FlushLogBufferToFile();
		// �α������� ����� ���丮 path.
		KOJEOMLIB_LOGGER_API void SetDirectory(std::string dirPath);
	private:
		std::ifstream ifstreamObj;
		std::ofstream ofstreamObj;
		std::string CreateLogMessage(LOG_TYPE type, std::string msg);
		// ���øӽ��� �������� Ÿ�ӽ������� �����ɴϴ�.
		std::string GetTimeStamp();
		std::vector<std::string> logFileBuffer;
		// �α������� ����� ���丮 path.
		std::string logFileDirPath;
		// ������ ����ȭ�� ���̴� mutex.
		std::mutex mutexObj;
	};
}

