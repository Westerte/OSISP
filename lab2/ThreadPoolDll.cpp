// ThreadPoolDll.cpp: определяет экспортированные функции для приложения DLL.
//

#include "stdafx.h"
#include "ThreadPoolDll.h"

namespace ThreadPoolDll
{
	

	ThreadPool::ThreadPool(int threadNum) :foreman_obj(new ForemanDll::Foreman(threadNum))	{}
	ThreadPool::~ThreadPool(){}

	template<class functionName, class... _ARGS>
	void ThreadPool::appendFuncToPool(functionName _fn, _ARGS... _args)
	{
		foreman_obj->appendToForemanQ(std::bind(_fn, _args...));
	}

}


