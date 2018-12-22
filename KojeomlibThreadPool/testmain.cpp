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
	std::cin >> inputValue;
}

void TestWork() {
	std::cout << "go!" << std::endl;
}