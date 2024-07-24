#include "ThreadPool.h"

ThreadPool::ThreadPool()
{
	for (int i = 0; i < std::thread::hardware_concurrency(); i++) {
		m_Threads.emplace_back(std::thread(
			[this] {
				for (;;) {

					std::function<void()> task;
					{
						std::unique_lock<std::mutex> lock(m_TasksMutex);
						m_TaskCV.wait(lock, [this] { return m_Stop || !m_Tasks.empty(); });

						if (m_Stop)
							return;

						task = std::move(m_Tasks.front());
						m_Tasks.pop();
					}
 
					task();
					if (--m_TasksPending == 0)
						m_WaitCV.notify_one();

				}
			}
		));
	}
}

void ThreadPool::Do(std::function<void()>&& task)
{
	m_TasksPending++;

	std::unique_lock<std::mutex> lock(m_TasksMutex);
	m_Tasks.emplace(task);
	m_TaskCV.notify_one();
}

void ThreadPool::Wait()
{
	std::unique_lock<std::mutex> lock(m_WaitMutex);
	m_WaitCV.wait(lock);
}

ThreadPool::~ThreadPool()
{
	{
		std::unique_lock<std::mutex> lock(m_TasksMutex);
		m_Stop = true;
	}

	m_TaskCV.notify_all();
	m_WaitCV.notify_one();

	for (auto& thread : m_Threads)
		thread.join();
}
