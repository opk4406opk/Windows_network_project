#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
namespace KojeomlibLogger {
	enum LOG_TYPE {
		TYPE_NORMAL = 0,
		TYPE_ERROR = 1
	};
	class KoLogger {
	public:
		void SetMsgToLogFileBuffer(std::string logMsg, LOG_TYPE type);
		void FlushLogBufferToFile();
	private:
		std::ifstream ifstreamObj;
		std::ofstream ofstreamObj;
		std::vector<std::string> logFileBuffer;
	};
}

