#pragma once
#include "BlockingQueue.h"

class ThreadPool
{
public:
	ThreadPool();
private:
	BlockingQueue<future<void>> tasks;
};

