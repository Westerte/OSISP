// ForemanDll.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "ForemanDll.h"


namespace ForemanDll
{
	Foreman::Foreman(size_t threads)
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
}



