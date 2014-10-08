#include "stdafx.h"
#include "WorkerDll.h"

namespace WorkerDll
{
	Worker::Worker()
		:enabled(true), task_queue()
		, thread(&Worker::thread_fn, this)
	{}
	Worker::~Worker() {}

	void Worker::appendFn(fn_type fn)
	{
		std::unique_lock<std::mutex> locker(mutex);
		task_queue.push(fn);
		cv.notify_one();
	}
	bool Worker::isEnable()
	{
		return enabled;
	};

	void Worker::thread_fn()
	{
		while (enabled)
		{
			std::unique_lock<std::mutex> locker(mutex);
			cv.wait(locker, [&](){ return !task_queue.empty(); });
			while (!task_queue.empty())
			{
				enabled = false;
				fn_type fn = task_queue.front();
				task_queue.pop();
				locker.unlock();
				fn();
				locker.lock();
				enabled = true;
			}
		}
	}

}