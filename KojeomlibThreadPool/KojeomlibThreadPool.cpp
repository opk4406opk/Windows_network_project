// KojeomlibThreadPool.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "KojeomlibThreadPool.h"

void KojeomlibThreadPool::ThreadPool::ExcuteAll()
{
	conditionVar.notify_all();
	workerManager = std::thread([this]() {
		while (true) {
			if (readyWorkers.size() > 0) {
				if (jobCount > 0) {
					unsigned int readyWorkerId = readyWorkers.front();
					readyWorkers.pop();
				}
				else {
					break;
				}
			}
		}
		std::cout << "All job is Done.. workerManager wait workers.." << std::endl;
		for (unsigned int idx = 0; idx < allWorkers.size(); idx++) {
			allWorkers[idx].th.join();
		}
	});
}

void KojeomlibThreadPool::ThreadPool::CompleteJob(unsigned int workerId)
{
	allWorkers[workerId].state = WORKER_STATE::READY;
	readyWorkers.push(workerId);
	std::cout << "worker id : " << workerId << " is job Complete, and Ready to Work." << std::endl;
	jobCount--;
}

