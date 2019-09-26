#pragma once
class ThreadPool
{
public:
	ThreadPool();
private:
	queue<future<void>> tasks;
};

