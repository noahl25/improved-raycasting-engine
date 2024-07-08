#pragma once

#include <thread>
#include <condition_variable>
#include <functional>
#include <mutex>

template <int Size>
class SizedThreadPool {

public:

	SizedThreadPool() {

		for (int i = 0; i < Size; i++)
			m_Tasks[i] = nullptr;

		for (int i = 0; i < Size; i++) {
			m_Threads[i] = std::thread([this, i] {
				while (true) {

					std::unique_lock<std::mutex> lock(m_Mutex);
					m_CV.wait(lock, [this, i] {return m_Stop || m_Tasks[i]; });

					if (m_Stop)
						return;

					m_Tasks[i]();
					m_Tasks[i] = nullptr;
					

				}
			});
		}
	}

	void Do(int thread, std::function<void()>&& func) {

		std::unique_lock<std::mutex> lock(m_Mutex);
		m_Tasks[thread] = std::move(func);

		m_CV.notify_all();

	}

	bool Complete() {
		bool complete = true;
		for (int i = 0; i < Size; i++) {
			if (m_Tasks[i] != nullptr)
				complete = false;
		}
		return complete;
	}

	~SizedThreadPool() {

		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Stop = true;
		}

		m_CV.notify_all();

		for (int i = 0; i < Size; i++)
			m_Threads[i].join();
	}

private:

	std::thread m_Threads[Size];
	std::function<void()> m_Tasks[Size];
	std::condition_variable m_CV;
	std::mutex m_Mutex;

	bool m_Stop = false;

};

 