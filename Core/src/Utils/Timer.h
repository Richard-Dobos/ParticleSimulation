#pragma once

#include <iostream>
#include <chrono>

namespace Core::Utils
{
	class Timer
	{
	public:
        Timer()
 		{
            m_StartTime = std::chrono::steady_clock::now();
 		}

 		~Timer()
 		{
            m_EndTime = std::chrono::high_resolution_clock::now();
 			
			uint64_t start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
			uint64_t end = std::chrono::time_point_cast<std::chrono::microseconds>(m_EndTime).time_since_epoch().count();

            float duration = (end - start) * 0.001f;

 			std::cout << "\rTime: " << duration << "ms" << " || FPS: " << 1000 / duration ;
 		}

	private:
		std::chrono::time_point<std::chrono::steady_clock> m_StartTime, m_EndTime;
	};
}
