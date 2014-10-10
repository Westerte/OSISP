#include "stdafx.h"
#include "ThreadPoolDll.h"

namespace ThreadPoolDll
{
	ThreadPool::ThreadPool(int threads)
	{
		std::ofstream journal("Journal.txt", std::ofstream::out);
		journal << "     *******WThreadPoolLight App Journal*******\n\n\n" << std::endl;
		for (int i = 0; i < threads; i++)
		{
			worker_ptr pWorker(new WorkerDll::Worker);
			workers.push_back(pWorker);
		}
	}
	ThreadPool::~ThreadPool(){}

	worker_ptr ThreadPool::GetFreeWorker()
	{
		int counter = 0;
		worker_ptr pWorker = NULL;
		while (pWorker == NULL)
		{
			for (auto &id : workers)
			{
				if (id->isEnable())
				{
					pWorker = id;
					break;
				}
			}
			if (counter == 0 && pWorker == NULL)
			{
				std::ofstream journal("Journal.txt", std::ofstream::app);
				WorkerDll::stream_mutex.lock();
				std::cout << "Warning: ThreadPool is full" << std::endl;
				journal << "Warning: ThreadPool is full" << std::endl;
				WorkerDll::stream_mutex.unlock();
				counter++;
			}
		}
		return pWorker;
	};
}


