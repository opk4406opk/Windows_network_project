#include "stdafx.h"
#include "KojeomlibThreadPool.h"
// ref : http://www.cplusplus.com/reference/condition_variable/condition_variable/
// ref : http://www.cplusplus.com/reference/mutex/unique_lock/
// ref : 
void TestWork();
void main() {
	KojeomlibThreadPool::ThreadPool thPool;
	thPool.Init(3, 10, TestWork);
	thPool.ExcuteAll();

	int inputValue;
	while (true) {
		std::cin >> inputValue;
		if (inputValue == 999) break;

		Sleep(1000);
	}
}

void TestWork() {
	std::cout << "go!" << std::endl;
}