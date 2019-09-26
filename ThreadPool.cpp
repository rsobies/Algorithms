#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool() :threads(thread::hardware_concurrency())
{
	const auto thread_count = thread::hardware_concurrency();

	for (unsigned int i = 0; i < thread_count; i++) {
		threads.push_back(thread([this]() 
		{
			while (!stopThreads) {
				auto [ok, task] = tasks.tryPop();

				if (ok) {
					task();
				}
				else {
					this_thread::yield();
				}
			}
		}));
	}
}

ThreadPool::~ThreadPool()
{
	stopThreads = true;

	for (auto& thread : threads) {
		thread.join();
	}
}
