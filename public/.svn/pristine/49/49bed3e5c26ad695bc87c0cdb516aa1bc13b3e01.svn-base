#ifndef YSTHREADPOOL_IS_INCLUDED
#define YSTHREADPOOL_IS_INCLUDED
/* { */

#include <vector>
#include <thread>
#include <mutex>
#include <functional>
#include <condition_variable>

class YsThreadPool
{
public:
	class SubThread
	{
	public:
		int threadIdx;

		std::thread t;

		std::mutex taskMutex;
		std::condition_variable taskWait;
		// taskLock owned by the sub thread.
		// The main thread will notify.

		std::mutex readyMutex;
		std::condition_variable readyWait;
		std::unique_lock <std::mutex> *readyLock;
		// readyWait owned by the main thread.
		// The sub thread will notify.

		YsThreadPool *threadPoolPtr;
		bool bailOut,ready,mustStart;

		SubThread();
		void ThreadFunc(void);
	};

private:
	friend class SubThread;
	std::mutex taskMutex;
	int nTask,nTaskDone;
	const std::function <void()> *taskArray;

private:
	// This thread pool may be shared from multiple objects, and may even be
	// used from the inside of a task running from this thread pool.
	// In that case, further branching the threads will only make CPU traffic jam,
	// and also destroys the integrity of the pool.
	// Therefore, this thread pool should not be used while the pooled threads are
	// already running.
	std::mutex preventRecursiveUse;

	long long int nThread;
	SubThread *threadArray;

	static void ThreadEntry(SubThread *subThread);

public:
	YsThreadPool(int nThread=4);
	~YsThreadPool();

	/*! This function runs given tasks using the cached threads.
	    The function will return when all the tasks are completed. */
	void Run(long long int nTask,const std::function <void()> taskArray[]);
};

/* } */
#endif
