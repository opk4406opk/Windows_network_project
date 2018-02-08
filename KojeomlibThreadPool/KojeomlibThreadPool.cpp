// KojeomlibThreadPool.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KojeomlibThreadPool.h"

void KojeomlibThreadPool::ThreadPool::ExcuteAll()
{
	conditionVar.notify_all();
	workerManager = std::thread([this]() {
		while (true) {
			if (readyWorkerCount.size() > 0) {
				if (jobCount > 0) {
					std::unique_lock<std::mutex> lck(mutexObj);
					readyWorkerCount.pop();
					conditionVar.notify_one();
					std::cout << "Some waiting Worker is started!!" << std::endl;
				}
				else {
					break;
				}
			}
			// 1sec(=1000ms)간격으로 준비된 작업 스레드가 있는지 확인.
			Sleep(1000);
		}
		std::unique_lock<std::mutex> lck(mutexObj);
		std::cout << "All job is Done.. workerManager wait workers.. (ready : " << readyWorkerCount.size() << " )" << std::endl;
		for (unsigned int idx = 0; idx < allWorkers.size(); idx++) {
			allWorkers[idx]->th->join();
		}
		std::cout << "All Worker is Exit Success.. and this Manager ended." << std::endl;
	});
}

KojeomlibThreadPool::ThreadPool::~ThreadPool(){
	workerManager.join();
	for (int idx = 0; idx < readyWorkerCount.size(); idx++) {
		readyWorkerCount.pop();
	}
}

void KojeomlibThreadPool::ThreadPool::CompleteJob(unsigned int workerId)
{
	allWorkers[workerId]->state = WORKER_STATE::READY;
	readyWorkerCount.push(workerId);
	std::cout << "worker id : " << workerId << " is job Complete, and Ready to Work." << std::endl;
	jobCount--;
}

