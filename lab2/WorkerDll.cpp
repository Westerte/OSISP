#include "stdafx.h"
#include <iostream>
#include <fstream>
#include "WorkerDll.h"


namespace WorkerDll
{
	std::mutex        stream_mutex;

	Worker::Worker()
		: go_away(false), task_queue(), thread(&Worker::thread_fn, this)
	{}
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
		return task_queue.empty();
	}

	void Worker::off()
	{
		go_away = true;
	}

	void Worker::thread_fn()
	{
		std::ofstream journal("Journal.txt", std::ofstream::app);
		stream_mutex.lock();
		journal << "Thread " << std::this_thread::get_id() << " have started" << std::endl;
		stream_mutex.unlock();
		while (!go_away)
		{
			std::unique_lock<std::mutex> locker(mutex);
			cv.wait(locker, [&](){ return !task_queue.empty() || go_away; });
			if (!task_queue.empty())
			{
				stream_mutex.lock();
				journal << "\nThread " << std::this_thread::get_id() << " have got task" << std::endl;
				stream_mutex.unlock();
				fn_type fn = task_queue.front();
				task_queue.pop();
				fn();
				stream_mutex.lock();
				journal << "\nThread " << std::this_thread::get_id() << " have finished task" << std::endl;
				stream_mutex.unlock();
			}
		}
		stream_mutex.lock();
		journal << "Thread " << std::this_thread::get_id() << " have been destroyed" << std::endl;
		stream_mutex.unlock();
	}

}