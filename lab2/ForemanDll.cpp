// ForemanDll.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "ForemanDll.h"


namespace ForemanDll
{
	Foreman::Foreman(size_t threads) : foreman_thread(&Foreman::foreman_thread_fn, this), foreman_task_queue(), foreman_go_away(true)
	{
		for (size_t i = 0; i<threads; i++)
		{
			worker_ptr pWorker(new WorkerDll::Worker);
			workers.push_back(pWorker);
		}
	}
	Foreman::~Foreman() {}

	worker_ptr Foreman::GetFreeWorker()
	{
		worker_ptr pWorker = NULL;
		for (auto &id : workers)
		{
			if (id->isEnable())
				return id;
		}
		return pWorker;
	};

	void Foreman::appendToForemanQ(fn_type fn)
	{
		std::unique_lock<std::mutex> locker(foreman_mutex);
		foreman_task_queue.push(fn);
		foreman_cv.notify_one();
	}

	void Foreman::foreman_off()
	{
		foreman_go_away = false;
	}

	void Foreman::foreman_thread_fn()
	{
		while (foreman_go_away)
		{
			std::unique_lock<std::mutex> locker(foreman_mutex);
			foreman_cv.wait(locker, [&](){ return !foreman_task_queue.empty(); });
			while (!foreman_task_queue.empty())
			{
				worker_ptr pWorker(GetFreeWorker());
				if (pWorker == NULL)
				{
					worker_ptr pNewWorker(new WorkerDll::Worker);
					workers.push_back(pNewWorker);
					pWorker->appendFn(foreman_task_queue.front());
				}
				else
				{
					pWorker->appendFn(foreman_task_queue.front());
				}
			}
		}
	}
}



