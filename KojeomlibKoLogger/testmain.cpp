
#include "stdafx.h"
#include "KojeomlibKoLogger.h"
void main() {
	KojeomlibLogger::KoLogger logger;
	logger.SetLogFilePath("D:\\test.txt");
	logger.SetMsgToLogFileBuffer("test_00", KojeomlibLogger::LOG_TYPE::TYPE_ERROR);
	logger.SetMsgToLogFileBuffer("test_44", KojeomlibLogger::LOG_TYPE::TYPE_ERROR);
	logger.FlushLogBufferToFile();
}