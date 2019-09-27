#pragma once
#include "BlockingQueue.h"

class ThreadPool
{
public:
	ThreadPool();
	virtual ~ThreadPool();
	future<void> submit(function<void()>&& task);
private:
	atomic_bool stopThreads= ATOMIC_VAR_INIT(false);
	vector<thread> threads;
	BlockingQueue<packaged_task<void()>> tasks;
};

