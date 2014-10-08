#ifdef FOREMAN_EXPORTS
#define FOREMAN_API __declspec(dllexport) 
#else
#define FOREMAN_API __declspec(dllimport) 
#endif

#include "WorkerDll.h"
#include <vector>
#include <memory>

namespace ForemanDll
{
	typedef std::shared_ptr<WorkerDll::Worker> worker_ptr;
	typedef std::function<void()> fn_type;
	
	class Foreman
	{
	public:
		FOREMAN_API Foreman(size_t threads);
		FOREMAN_API ~Foreman();
		FOREMAN_API void appendToForemanQ(fn_type fn);
		FOREMAN_API void foreman_off();
	private:

		FOREMAN_API worker_ptr GetFreeWorker();
		FOREMAN_API void foreman_thread_fn();
		std::vector<worker_ptr> workers;

		std::condition_variable		foreman_cv;
		std::queue<fn_type>			foreman_task_queue;
		std::mutex					foreman_mutex;
		std::thread					foreman_thread;
		bool							foreman_go_away;


	};
}
