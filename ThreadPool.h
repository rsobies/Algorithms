#pragma once
#include "BlockingQueue.h"

/// <summary>
/// Thread pool where max threads = nubmer of processors
/// </summary>
class ThreadPool
{
public:
	/// <summary>
	/// Creates and starts threads.
	/// </summary>
	ThreadPool();

	/// <summary>
	/// Stops all threads
	/// </summary>
	virtual ~ThreadPool();

	/// <summary>
	/// Submits a task
	/// </summary>
	/// <param name="task">A task to submit</param>
	/// <returns>The futre object of submited task</returns>
	future<void> submit(function<void()>&& task);

	/// <summary>
	/// Is there any idle threads
	/// </summary>
	/// <returns>Thread pool is full, no idle threads</returns>
	bool isFull();
private:
	/// <summary>
	/// Should all threads be stopped?
	/// </summary>
	atomic_bool stopThreads= false;

	/// <summary>
	/// List of all threads
	/// </summary>
	vector<thread> threads;

	/// <summary>
	/// List of submited tasks, waiting to be run
	/// </summary>
	BlockingQueue<packaged_task<void()>> tasks;

	/// <summary>
	/// 
	/// </summary>
	unsigned int maxThreads= thread::hardware_concurrency();

	/// <summary>
	/// How many thread are performing tasks
	/// </summary>
	short activThreads=0;

	/// <summary>
	/// Guard for activThreads field
	/// </summary>
	mutex activeThreadsMtx;
};

