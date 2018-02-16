
#include "stdafx.h"
#include "KojeomlibKoLogger.h"
void main() {
	KojeomlibLogger::KoLogger logger;
	logger.SetDirectory("D:\\");
	logger.SetMsgToLogFileBuffer("test_00", KojeomlibLogger::LOG_TYPE::TYPE_ERROR);
	logger.SetMsgToLogFileBuffer("test_44", KojeomlibLogger::LOG_TYPE::TYPE_ERROR);
	logger.FlushLogBufferToFile();
}