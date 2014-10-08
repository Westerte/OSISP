#ifdef WORKER_EXPORTS
#define WORKER_API __declspec(dllexport) 
#else
#define WORKER_API __declspec(dllimport) 
#endif

#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <memory>
#include <condition_variable>

namespace WorkerDll
{
	typedef std::function<void()> fn_type;

	class Worker
	{
	public:
		WORKER_API Worker();
		WORKER_API ~Worker();
		WORKER_API void appendFn(fn_type fn);
		WORKER_API bool  isEnable();

	private:
		std::condition_variable		cv;
		std::queue<fn_type>			task_queue;
		std::mutex					mutex;
		std::thread					thread;
		bool							enabled;

		WORKER_API void thread_fn();
	};
}