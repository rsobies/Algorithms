#include "pch.h"
#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
	const auto thread_count = thread::hardware_concurrency();

	for (unsigned int i = 0; i < thread_count; i++) {
		threads.push_back(thread([this]() 
		{
			while (!stopThreads) {
				activeThreadsMtx.lock();
				auto [ok, task] = tasks.tryPop();

				if (ok) {
					activThreads++;
					activeThreadsMtx.unlock();
					task();
					activeThreadsMtx.lock();
					activThreads--;
					activeThreadsMtx.unlock();
				}
				else {
					activeThreadsMtx.unlock();
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
		if (thread.joinable()) {
			thread.join();
		}
		
	}
}

future<void> ThreadPool::submit(function<void()>&& task)
{
	auto pack = packaged_task<void()>(move(task));
	auto fut = pack.get_future();
	tasks.push(packaged_task(move(pack)));
	return fut;
}

bool ThreadPool::isFull()
{
	lock_guard lock(activeThreadsMtx);
	return activThreads == maxThreads;
}
