#ifdef THREADPOOLDLL_EXPORTS
#define THREADPOOLDLL_API __declspec(dllexport) 
#else
#define THREADPOOLDLL_API __declspec(dllimport) 
#endif

#include "ForemanDll.h"

namespace ThreadPoolDll
{
	typedef std::function<void()> fn_type;
	typedef std::shared_ptr<ForemanDll::Foreman> foreman_ptr;
	class ThreadPool
	{
	public:
		THREADPOOLDLL_API ThreadPool(int threadNum);
		THREADPOOLDLL_API ~ThreadPool();
		template<class functionName, class... _ARGS>
		THREADPOOLDLL_API void appendFuncToPool(functionName _fn, _ARGS... _args);
	private:
		foreman_ptr		 foreman_obj;
	};
}