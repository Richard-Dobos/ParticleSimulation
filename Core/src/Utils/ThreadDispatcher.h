#pragma once

#include <mutex>
#include <queue>
#include <thread>
#include <atomic>
#include <iostream>
#include <functional>

#include "Core.h"

namespace Core::Utils
{
	class CORE_API ThreadDispatcher
	{
		using ThreadTask = std::function<void()>;

	public:
		ThreadDispatcher(uint16_t numberOfThreads);
		ThreadDispatcher();
		~ThreadDispatcher();

		void dispatchTask(ThreadTask threadTask);
		void dispatchNewThreadWorkers(uint16_t numberOfThreads);

		static uint16_t getCPUThreadCount() { return std::thread::hardware_concurrency(); };
		uint16_t getDispatchedCPUThreadsCount() const;

	private:
		void workerThread();

	private:
		std::mutex m_Mutex;
		std::condition_variable m_CV;

		std::queue<ThreadTask> m_Tasks;
		std::vector<std::thread> m_WorkerThreads;

		std::atomic<bool> m_StopThreads;

		uint16_t m_CPUThreadsDispatched = 0;
	};
}