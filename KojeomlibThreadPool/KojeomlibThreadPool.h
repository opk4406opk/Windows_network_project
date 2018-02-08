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
		std::thread* th;
		WorkerInfo() {
			// to do
		}
		~WorkerInfo() {
			if(th != nullptr) delete th;
		}

		WorkerInfo(const WorkerInfo&& other) {
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
		std::queue<unsigned int> readyWorkerCount;
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
			WorkerInfo* info = new WorkerInfo();
			info->th = new std::thread([this, workerJob, idx]() {
				while (true) {
					// lock
					std::unique_lock<std::mutex> lck(mutexObj);
					// wait�� �ϰ� ���� lock�� ����. �ٸ� �����尡 �� �ڵ�� ����(wait�ϰԵȴ�).
					// wait�߿� �ٸ������� ���� notify�� �ϰ� �Ǹ� lock�� �ɰ� wait�� �������´�.
					conditionVar.wait(lck);
					if (jobCount > 0) {
						allWorkers[idx]->state = WORKER_STATE::RUNNING;
						// job ����.
						workerJob();
						// job �Ϸ�.
						CompleteJob(idx);
						std::cout << "current job count : " << jobCount << std::endl;
					}else {
						std::cout << "worker id : [" << idx << "] exit." << std::endl;
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
