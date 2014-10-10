#ifdef WORKER_EXPORTS
#define WORKER_API __declspec(dllexport) 
#else
#define WORKER_API 
#endif

#include <condition_variable>
#include <queue>
#include <functional>
#include <thread>
#include <mutex>
#include <memory>

namespace WorkerDll
{
	extern std::mutex        stream_mutex;
	typedef std::function<void()> fn_type;
	class Worker
	{
	public:
		WORKER_API Worker();
		WORKER_API ~Worker();
		WORKER_API void appendFn(fn_type fn);
		WORKER_API bool  isEnable();
		WORKER_API void  off();

	private:
		std::condition_variable		cv;
		std::queue<fn_type>			task_queue;
		std::mutex					mutex;
		std::thread					thread;
		bool							enabled;
		bool							go_away;

		WORKER_API void thread_fn();
	};
}

