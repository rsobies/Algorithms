#pragma once

/// <summary>
/// Simple blocking queue, first in first out
/// </summary>
/// <typeparam name="Element_t">type of items in queue</typeparam>
/// <remarks>when list is empty it waits during poping</remarks>
template <typename Element_t>
class BlockingQueue
{
public:
	/// <summary>
	/// insert new element to list
	/// </summary>
	/// <param name="newElement"></param>
	/// <remarks>perfect forwarding is used</remarks>
	void push(Element_t&& newElement);

	/// <summary>
	/// gets and removes first element,
	/// it will not block if list is empty
	/// </summary>
	/// <returns>
	/// false if list is empty and no elements is return
	/// true if element is return, secand field contains element
	/// </returns>
	tuple<bool, Element_t> tryPop();

	/// <summary>
	/// gets and removes first element,
	/// if list is empty it waits until new elements is added
	/// </summary>
	/// <returns>first element of the queue</returns>
	Element_t pop();

	/// <summary>
	/// Size of queue
	/// </summary>
	/// <returns></returns>
	size_t size();

private:

	/// <summary>
	/// for synchronizing push and pop
	/// </summary>
	mutex mtx;

	/// <summary>
	/// for synchronizing push and pop
	/// </summary>
	condition_variable condition;

	/// <summary>
	/// implementation of queue
	/// </summary>
	queue<Element_t> myQueue;
};

template<typename Element_t>
inline void BlockingQueue<Element_t>::push(Element_t&& newElement)
{
	lock_guard lock(mtx);
	myQueue.push(forward<Element_t>(newElement));
	condition.notify_one();
}

template<typename Element_t>
inline tuple<bool, Element_t> BlockingQueue<Element_t>::tryPop()
{
	lock_guard lock(mtx);
	if (myQueue.empty()) {
		return tuple(false, Element_t());
	}

	auto ret = tuple(true, Element_t(move(myQueue.front())));
	
	myQueue.pop();

	return ret;
}

template<typename Element_t>
inline Element_t BlockingQueue<Element_t>::pop()
{
	unique_lock lock(mtx);
	condition.wait(lock, [this]() {return !myQueue.empty(); });
	Element_t element(move(myQueue.front()));
	myQueue.pop();

	return element;
}

template<typename Element_t>
inline size_t BlockingQueue<Element_t>::size()
{
	return myQueue.size();
}
