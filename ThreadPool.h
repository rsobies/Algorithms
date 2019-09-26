#pragma once
#include "BlockingQueue.h"

class ThreadPool
{
public:
	ThreadPool();
	virtual ~ThreadPool();
private:
	atomic_bool stopThreads = false;
	vector<thread> threads;
	BlockingQueue<function<void()>> tasks;
};

