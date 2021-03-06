// KojeomlibKoLogger.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "KojeomlibKoLogger.h"

KojeomlibLogger::KoLogger::KoLogger(){
}

KojeomlibLogger::KoLogger::~KoLogger(){
	ifstreamObj.close();
	ofstreamObj.close();
}

void KojeomlibLogger::KoLogger::MsgGoToConsole(std::string logMsg, LOG_TYPE type){
	std::cout << CreateLogMessage(type, logMsg) << std::endl;
}

void KojeomlibLogger::KoLogger::MsgGoToConsoleSafely(std::string logMsg, LOG_TYPE type){
	std::lock_guard<std::mutex> lockGuard(mutexObj);
	std::cout << CreateLogMessage(type, logMsg) << std::endl;
}

void KojeomlibLogger::KoLogger::SetMsgToLogFileBuffer(std::string logMsg, LOG_TYPE type){
	logFileBuffer.push_back(CreateLogMessage(type, logMsg));
}

void KojeomlibLogger::KoLogger::SetMsgToLogFileBufferSafely(std::string logMsg, LOG_TYPE type){
	// mutex 래핑한 lock_guard로 스레드 동기화.
	// lock_guard의 경우 scope 범위에서만 lock을 걸어준다.
	// http://en.cppreference.com/w/cpp/thread/lock_guard
	std::lock_guard<std::mutex> lockGuard(mutexObj);
	logFileBuffer.push_back(CreateLogMessage(type, logMsg));
}

void KojeomlibLogger::KoLogger::FlushLogBufferToFile(){
	try {
		// init open file path.
		std::string filePath = logFileDirPath;
		filePath.append(GetTimeStamp());
		filePath.append("kologger");
		filePath.append(LOG_FILE_EXTENT);
		// open file.
		ofstreamObj.open(filePath, std::ios::app);
		if (ofstreamObj.good()) {
			for each (auto msg in logFileBuffer) {
				ofstreamObj.write(msg.data(), msg.length());
				ofstreamObj.write("\n", 1);
			}
		} else {
			if (ofstreamObj.eof())std::cout << "open stream is end of file." << std::endl;
			if (ofstreamObj.fail())std::cout << "open stream is failed." << std::endl;
			if (ofstreamObj.bad()) std::cout << "open stream is bad." << std::endl;
		}
		ofstreamObj.close();
	}
	catch(const std::exception& ex){
		std::cout << "FlushLogBufferToFile Error : " << ex.what() << std::endl;
		ofstreamObj.close();
	}
	logFileBuffer.clear();
}

void KojeomlibLogger::KoLogger::SetDirectory(std::string dirPath){
	logFileDirPath = dirPath;
}

std::string KojeomlibLogger::KoLogger::CreateLogMessage(LOG_TYPE type, std::string msg){
	std::string resembleLogMsg;
	switch (type) {
	case LOG_TYPE::TYPE_ERROR:
		resembleLogMsg = boost::str(boost::format("[DEBUG_ERROR](time : %s) MSG : %s") % GetTimeStamp() % msg);
		break;
	case LOG_TYPE::TYPE_WARNNING:
		resembleLogMsg = boost::str(boost::format("[DEBUG_WARNNING](time : %s) MSG : %s") % GetTimeStamp() % msg);
		break;
	case LOG_TYPE::TYPE_INFO:
		resembleLogMsg = boost::str(boost::format("[DEBUG_INFO](time : %s) MSG : %s") % GetTimeStamp() % msg);
		break;
	}
	return resembleLogMsg;
}

std::string KojeomlibLogger::KoLogger::GetTimeStamp(){
	// get current systemTime.
	time_t timer;
	time(&timer);
	tm timeInfo;
	localtime_s(&timeInfo, &timer);
	return boost::str(boost::format("[%s-%s-%s_%s(h)_%s(m)_%s(sec)]")
		% std::to_string(timeInfo.tm_year + 1900)
		% std::to_string(timeInfo.tm_mon + 1)
		% std::to_string(timeInfo.tm_mday)
		% std::to_string(timeInfo.tm_hour)
		% std::to_string(timeInfo.tm_min)
		% std::to_string(timeInfo.tm_sec));
}
