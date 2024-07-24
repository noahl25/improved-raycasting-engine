#pragma once

#include <thread>
#include <condition_variable>
#include <functional>
#include <mutex>
#include <queue>
 
class ThreadPool {

public:

	ThreadPool();

	void Do(std::function<void()>&& task);
	void Wait();

	~ThreadPool();

private:

	std::vector<std::thread> m_Threads;
	std::queue<std::function<void()>> m_Tasks;

	std::condition_variable m_TaskCV;
	std::mutex m_TasksMutex;

	std::atomic<int> m_TasksPending;
	std::mutex m_WaitMutex;
	std::condition_variable m_WaitCV;

	bool m_Stop = false;

};

 