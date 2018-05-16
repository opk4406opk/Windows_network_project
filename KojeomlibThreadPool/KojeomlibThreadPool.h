#pragma once

#ifdef KOJEOMLIB_THREAD_POOL_EXPORTS  
#define KOJEOMLIB_THREAD_POOL_API __declspec(dllexport)   
#else  
#define KOJEOMLIB_THREAD_POOL_API __declspec(dllimport)   
#endif 

#define CALLBACK_FUNC
#include <Windows.h>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <iostream>
namespace KojeomlibThreadPool {

	enum WORKER_STATE {
		READY = 0,
		RUNNING = 1,
		ENDED = 2
	};
	struct WorkerInfo {
	public:
		unsigned int id;
		WORKER_STATE state;
		std::thread* th;
		WorkerInfo() {
			// to do
		}
		~WorkerInfo() {
			if(th != nullptr) delete th;
		}

		WorkerInfo(const WorkerInfo&& other) noexcept {
			id = other.id;
			state = other.state;
			th = other.th;
		}
	};

	// class definition.
	class ThreadPool {
	public:
		template<class Fn>
		void Init(unsigned int workerCnt, unsigned int jobCnt, Fn workerJob);
		void ExcuteAll();
		~ThreadPool();
	private:
		std::vector<WorkerInfo*> allWorkers;
		std::queue<int> jobs;
		std::mutex mutexObj;
		std::condition_variable workerCondition;
		std::thread workerManager;
		unsigned int workerCount;
		void CALLBACK_FUNC CompleteJob(unsigned int workerId);
	};

	// inline methods.
	template<class Fn>
	inline void ThreadPool::Init(unsigned int workerCnt, unsigned int jobCnt, Fn workerJob)
	{
		workerCount = workerCnt;
		for (unsigned int idx = 0; idx < jobCnt; idx++) {
			jobs.push(idx);
		}
		for (unsigned int idx = 0; idx < workerCount; idx++) {
			WorkerInfo* info = new WorkerInfo();
			info->th = new std::thread([this, workerJob, idx]() {
				while (true) {
					// lock
					std::unique_lock<std::mutex> lck(mutexObj);
					// wait�� �ϰ� ���� lock�� ����. �ٸ� �����尡 �� �ڵ�� ����(wait�ϰԵȴ�).
					// wait�߿� �ٸ������� ���� notify�� �ϰ� �Ǹ� lock�� �ɰ� wait�� �������´�.
					workerCondition.wait(lck);
					if (jobs.size() > 0) {
						jobs.pop();
						std::cout << "worker Id : " << idx << " is Waiting..." << std::endl;
						//
						allWorkers[idx]->state = WORKER_STATE::RUNNING;
						// job ����.
						workerJob();
						// job �Ϸ�.
						CompleteJob(idx);
						std::cout << "current job count : " << jobs.size() << std::endl;
					}else {
						std::cout << "worker id : [" << idx << "] exit." << std::endl;
						allWorkers[idx]->state = WORKER_STATE::ENDED;
						break;
					}
				}
				// worker thread destroyed...
			});
			info->id = idx;
			info->state = WORKER_STATE::READY;
			allWorkers.push_back(info);
		}
	}

}
