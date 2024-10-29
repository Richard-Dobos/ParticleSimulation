#pragma once

#include <iostream>
#include <chrono>

#include "Core.h"

namespace Core::Utils
{
    class CORE_API Timer
    {
    public:
        Timer() = default;
        ~Timer()
        {
            m_EndTime = std::chrono::high_resolution_clock::now();

			auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTime).time_since_epoch().count();
			auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_EndTime).time_since_epoch().count();

            double duration = (end - start) * 0.001f;

			double averageFrameTime = duration / m_FrameCount;
            double averageFPS = m_FrameCount / (duration * 0.001);

            std::cout << CYAN_BACKGROUND << BLACK << "\nAverage FrameTime: " << averageFrameTime << "ms" << " || Average FPS: " << averageFPS << RESET;
        }

        void startTimerPeriod()
        {
			if (!m_FPSAvgFlag)
			{
                m_StartTime = std::chrono::high_resolution_clock::now();
				m_FPSAvgFlag = true;
			}

			m_StartPeriod = std::chrono::high_resolution_clock::now();
        }

        void endTimerPeriod()
        {
            m_EndPeriod = std::chrono::high_resolution_clock::now();

            auto start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartPeriod).time_since_epoch().count();
            auto end = std::chrono::time_point_cast<std::chrono::microseconds>(m_EndPeriod).time_since_epoch().count();

            double duration = (end - start) * 0.001f;

			m_FrameCount += 1;

            std::cout << CYAN_BACKGROUND << BLACK << "\rFrameTime: " << duration << "ms" << " || FPS: " << 1000 / duration << RESET;
        }

    private:
        double m_FrameCount = 0;
        
        bool m_FPSAvgFlag = false;

        std::chrono::time_point<std::chrono::steady_clock> m_StartTime, m_EndTime;
        std::chrono::time_point<std::chrono::steady_clock> m_StartPeriod, m_EndPeriod;

    };
}
