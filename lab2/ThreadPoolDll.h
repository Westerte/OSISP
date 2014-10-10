#ifdef THREADPOOLDLL_EXPORTS
#define THREADPOOLDLL_API __declspec(dllexport) 
#else
#define THREADPOOLDLL_API
#endif

#include "WorkerDll.h"
#include <vector>
#include <memory>
#include <iostream>
#include <fstream>


namespace ThreadPoolDll
{
	typedef std::shared_ptr<WorkerDll::Worker> worker_ptr;
	typedef std::function<void()> fn_type;

	class ThreadPool
	{
	public:
		THREADPOOLDLL_API ThreadPool(int threadNum);
		THREADPOOLDLL_API ~ThreadPool();
		template<class _FN, class... _ARGS>
		void AppendToPool(_FN _fn, _ARGS... _args)
		{
			worker_ptr pWorker = GetFreeWorker();
			pWorker->appendFn(std::bind(_fn, _args...));
		}
	private:
		THREADPOOLDLL_API worker_ptr GetFreeWorker();
		std::vector<worker_ptr> workers;
	};
}

