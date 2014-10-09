#include "stdafx.h"
#include "ThreadPoolDll.h"

namespace ThreadPoolDll
{

	ThreadPool::ThreadPool(int threads)
	{
		std::ofstream journal("Journal.txt", std::ofstream::out);
		journal << "*******WThreadPoolLight App Journal*******\n\n\n" << std::endl;
		for (int i = 0; i < threads; i++)
		{
			worker_ptr pWorker(new WorkerDll::Worker);
			workers.push_back(pWorker);
		}
	}
	ThreadPool::~ThreadPool(){}

	worker_ptr ThreadPool::GetFreeWorker()
	{
		worker_ptr pWorker = NULL;
		for (auto &id : workers)
		{
			if (id->isEnable())
				return id;
		}
		return pWorker;
	};
}


