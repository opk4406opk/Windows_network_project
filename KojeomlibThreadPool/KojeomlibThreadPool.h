#pragma once

#ifdef KOJEOMLIB_THREAD_POOL_EXPORTS  
#define KOJEOMLIB_THREAD_POOL_API __declspec(dllexport)   
#else  
#define KOJEOMLIB_THREAD_POOL_API __declspec(dllimport)   
#endif 

#define CALLBACK_FUNC

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <iostream>
namespace KojeomlibThreadPool {

	enum WORKER_STATE {
		READY = 0,
		RUNNING = 1
	};
	struct WorkerInfo {
	public:
		unsigned int id;
		WORKER_STATE state;
		std::thread th;

		WorkerInfo(const WorkerInfo&& other) {
			id = other.id;
			state = other.state;
			
		}
	};

	// class definition.
	class ThreadPool {
	public:
		template<class Fn>
		void Init(unsigned int workerCnt, unsigned int jobCnt, Fn workerJob);
		void ExcuteAll();
	private:
		std::vector<WorkerInfo> allWorkers;
		std::queue<unsigned int> readyWorkers;
		std::mutex mutexObj;
		std::condition_variable conditionVar;
		std::thread workerManager;
		unsigned int workerCount;
		unsigned int jobCount;
		void CALLBACK_FUNC CompleteJob(unsigned int workerId);
	};

	// inline methods.
	template<class Fn>
	inline void ThreadPool::Init(unsigned int workerCnt, unsigned int jobCnt, Fn workerJob)
	{
		workerCount = workerCnt;
		jobCount = jobCnt;
		for (unsigned int idx = 0; idx < workerCount; idx++) {
			WorkerInfo info;
			info.th = std::thread([&conditionVar, &mutexObj, =workerJob, =workerId]() {
				while (true) {
					std::unique_lock<std::mutex> lck(mutexObj);
					conditionVar.wait(lck);
					workerJob();
					//
					lck.lock();
					afterDone(workerId);
					lck.unlock();
				}
			});
			info.id = idx;
			info.state = WORKER_STATE::READY;
			allWorkers.push(info);
		}
	}

}
