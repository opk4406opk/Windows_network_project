// KojeomlibKoLogger.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "KojeomlibKoLogger.h"

KojeomlibLogger::KoLogger::KoLogger(){
}

KojeomlibLogger::KoLogger::~KoLogger(){
}

void KojeomlibLogger::KoLogger::SetMsgToLogFileBuffer(std::string logMsg, LOG_TYPE type){
	logFileBuffer.push_back(CreateLogMessage(type, logMsg));
}

void KojeomlibLogger::KoLogger::FlushLogBufferToFile(){
	try {
		ofstreamObj.open(logFilePath, std::ios::app);
		for each (auto msg in logFileBuffer) {
			ofstreamObj.write(msg.data(), msg.length());
			ofstreamObj.write("\n", 1);
		}
		ofstreamObj.close();
	}
	catch(const std::exception& ex){
		std::cout << "FlushLogBufferToFile Error : " << ex.what() << std::endl;
		ofstreamObj.close();
	}
	logFileBuffer.clear();
}

void KojeomlibLogger::KoLogger::SetLogFilePath(std::string pathWithFileName){
	logFilePath = pathWithFileName;
}

std::string KojeomlibLogger::KoLogger::CreateLogMessage(LOG_TYPE type, std::string msg){
	// get current systemTime.
	time(&systemTimer);
	localtime_s(&timeInfo, &systemTimer);
	char curTime[256];
	asctime_s(curTime, 256, &timeInfo);
	//
	std::string timeStamp;
	switch (type) {
	case LOG_TYPE::TYPE_ERROR:
		timeStamp = boost::str(boost::format("[DEBUG_ERROR](time : %s) MSG : %s") % curTime % msg);
		break;
	case LOG_TYPE::TYPE_WARNNING:
		timeStamp = boost::str(boost::format("[DEBUG_WARNNING](time : %s) MSG : %s") % curTime % msg);
		break;
	}
	return timeStamp;
}
