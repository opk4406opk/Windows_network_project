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
		// init open file path.
		std::string filePath = logFileDirPath;
		filePath.append(GetTimeStamp());
		filePath.append(LOG_FILE_EXTENT);
		// open file.
		ofstreamObj.open(filePath, std::ios::app);
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
	}
	return resembleLogMsg;
}

std::string KojeomlibLogger::KoLogger::GetTimeStamp(){
	// get current systemTime.
	time(&systemTimer);
	localtime_s(&timeInfo, &systemTimer);
	return boost::str(boost::format("%s(h)_%s(m)_%s(sec)") % std::to_string(timeInfo.tm_hour)
		% std::to_string(timeInfo.tm_min) % std::to_string(timeInfo.tm_sec));
}
