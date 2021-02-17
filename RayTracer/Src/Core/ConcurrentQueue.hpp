#pragma once
#include <queue>
#include <mutex>
#include <vector>

template<class T>
class ConcurrentQueue
{
public:
	ConcurrentQueue() : m_lock()
	{
	}

	ConcurrentQueue(const std::vector<T> items) : m_lock()
	{
		for (const auto& item : items)
		{
			m_queue.push(item);
		}
	}

	~ConcurrentQueue()
	{
	}

	void Push(const T& item)
	{
		std::unique_lock<std::mutex> lock(m_lock);

		m_queue.push(item);

		lock.unlock();
	}

	bool Pop(T& item)
	{
		std::unique_lock<std::mutex> lock(m_lock);

		bool isEmpty = m_queue.empty();
		if (!isEmpty)
		{
			item = m_queue.front();
			m_queue.pop();
		}

		lock.unlock();

		return !isEmpty;
	}

	bool IsEmpty()
	{
		std::unique_lock<std::mutex> lock(m_lock);

		bool isEmpty = m_queue.empty();

		lock.unlock();

		return isEmpty;
	}

private:
	mutable std::mutex m_lock;
	std::queue<T> m_queue;
};
