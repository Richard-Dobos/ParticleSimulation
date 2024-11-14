#include "ThreadDispatcher.h"

#include "Core.h"

namespace Core::Utils
{
	ThreadDispatcher::ThreadDispatcher(uint16_t numberOfThreads)
		:m_StopThreads(false), m_CPUThreadsDispatched(numberOfThreads)
	{
		if (numberOfThreads > getCPUThreadCount())
		{
			LOG_WARN("Number of threads [{}] exceeded real number of thread [{}] on cpu!", numberOfThreads, getCPUThreadCount());
		}

		LOG_INFO("Dispatched {} worker threads.", numberOfThreads);

		for (uint8_t i = 0; i < numberOfThreads; i++)
		{
			m_WorkerThreads.emplace_back([this] { workerThread(); });
		}
	}

	ThreadDispatcher::ThreadDispatcher()
	{
		uint16_t CPUThreads = getCPUThreadCount();
		uint16_t workerThreadsToDispatch = 1;

		if (CPUThreads < 4 && CPUThreads > 2)
		{
			workerThreadsToDispatch = 2;
		}

		else if(CPUThreads < 8)
		{
			workerThreadsToDispatch = 4;
		}

		else if (CPUThreads >= 12)
		{
			workerThreadsToDispatch = 8;
		}

		for (uint16_t i = 0; i < workerThreadsToDispatch; i++)
		{
			m_WorkerThreads.emplace_back([this] { workerThread(); });
		}
	}

	ThreadDispatcher::~ThreadDispatcher()
	{
		{
			std::scoped_lock<std::mutex> lock(m_Mutex);
			m_StopThreads = true;
		}

		m_CV.notify_all();

		{
			for (std::thread& worker : m_WorkerThreads)
			{
				if (worker.joinable())
				{
					worker.join();
				}
			}
		}
	}

	void ThreadDispatcher::dispatchNewThreadWorkers(uint16_t numberOfThreads)
	{
		m_CPUThreadsDispatched++;

		if (m_CPUThreadsDispatched > getCPUThreadCount())
		{
			LOG_WARN("Number of threads [{}] exceeded real number of thread [{}] on cpu!", m_CPUThreadsDispatched, getCPUThreadCount());
		}

		for (uint16_t i = 0; i < numberOfThreads; i++)
		{
			m_WorkerThreads.emplace_back([this] { workerThread(); });
		}
	}

	void ThreadDispatcher::dispatchTask(ThreadTask threadTask)
	{
		{
			std::scoped_lock<std::mutex> lock(m_Mutex);
			m_Tasks.push(std::move(threadTask));
		}

		m_CV.notify_one();
	}

	void ThreadDispatcher::workerThread()
	{
		while (true)
		{
			ThreadTask threadTask;

			{
				std::unique_lock<std::mutex> lock(m_Mutex);
				m_CV.wait(lock, [this] { return m_StopThreads || !m_Tasks.empty(); });

				if (m_StopThreads && m_Tasks.empty()) { return; }

				threadTask = std::move(m_Tasks.front());
				m_Tasks.pop();
			}

			threadTask();
		}
	}

	uint16_t ThreadDispatcher::getDispatchedCPUThreadsCount() const
	{
		return m_CPUThreadsDispatched;
	}
}
