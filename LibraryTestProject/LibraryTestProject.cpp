// LibraryTestProject.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include "testlibs/KojeomlibThreadPool.h"
#pragma comment(lib, "testlibs/KojeomlibThreadPool.lib")

using namespace KojeomlibThreadPool;
void TestJob();
int main()
{
	ThreadPool testPool;
	testPool.Init(3, 3, TestJob);
	testPool.ExcuteAll();
    return 0;
}

void TestJob() {
	std::cout << "hello" << std::endl;
}