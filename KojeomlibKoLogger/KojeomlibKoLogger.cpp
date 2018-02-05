// KojeomlibKoLogger.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "KojeomlibKoLogger.h"

void KojeomlibLogger::KoLogger::SetMsgToLogFileBuffer(std::string logMsg, LOG_TYPE type){
	switch (type) {
	case LOG_TYPE::TYPE_NORMAL:
		logMsg.append("[NORMAL]\n");
		logFileBuffer.push_back(logMsg);
		break;
	case LOG_TYPE::TYPE_ERROR:
		logMsg.append("[ERROR]\n");
		logFileBuffer.push_back(logMsg);
		break;
	}
}

void KojeomlibLogger::KoLogger::FlushLogBufferToFile(){
}
