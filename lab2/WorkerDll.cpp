#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "WorkerDll.h"

namespace WorkerDll
{
	Worker::Worker()
		:enabled(true), go_away(false), task_queue(), thread(&Worker::thread_fn, this)
	{

	}
	Worker::~Worker()
	{
		off();
		cv.notify_one();
		thread.join();
	}

	void Worker::appendFn(fn_type fn)
	{
		std::unique_lock<std::mutex> locker(mutex);
		task_queue.push(fn);
		cv.notify_one();
	}

	bool Worker::isEnable()
	{
		return enabled;
	}

	void Worker::off()
	{
		go_away = true;
	}


	void Worker::thread_fn()
	{
		std::ofstream journal("Journal.txt", std::ofstream::app);
		stream_mutex.lock();
		journal << "Thread " << std::this_thread::get_id() << " have started processing" << std::endl;
		stream_mutex.unlock();
		while (!go_away)
		{
			std::unique_lock<std::mutex> locker(mutex);
			cv.wait(locker, [&](){ return !task_queue.empty() || go_away; });
			enabled = false;
			if (!task_queue.empty())
			{
				fn_type fn = task_queue.front();
				task_queue.pop();
				locker.unlock();
				fn();
				locker.lock();
			}
			enabled = true;
		}
	}

}