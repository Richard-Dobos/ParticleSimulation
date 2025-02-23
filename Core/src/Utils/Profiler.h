#pragma once

#include "Core.h"

#include <chrono>
#include <utility>
#include <functional>
#include <unordered_map>

namespace Core::Utils
{
	class Profiler
	{
	public:
		Profiler() = default;
		~Profiler() = default;

		template <typename Func, typename... Args>
		auto profile(std::string funcName, Func&& function, Args&&... args)
		{
			using ReturnType = decltype(function(std::forward<Args>(args)...));

			auto startTime = std::chrono::high_resolution_clock::now();

			if constexpr (std::is_void_v<ReturnType>)
				std::invoke(std::forward<Func>(function), std::forward<Args>(args)...);

			else
			{
				ReturnType returnValue = std::invoke(std::forward<Func>(function), std::forward<Args>(args)...);
				
				auto endTime = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double, std::milli> elapsedTime = endTime - startTime;
				
				m_ProfiledFunctions[FUNCTION_NAME(function)] = elapsedTime.count();

				return returnValue;
			}

			auto endTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double, std::milli> elapsedTime = endTime - startTime;

			m_ProfiledFunctions[funcName] = elapsedTime.count();
		}

		double getProfiledFunction(std::string functionName)
		{
			if (m_ProfiledFunctions.contains(functionName))
			{
				return m_ProfiledFunctions[functionName];
			}

			LOG_ERROR("Function {} was not profiled", functionName);
			return 0;
		}
		
	private:
		std::unordered_map<std::string, double> m_ProfiledFunctions;
	};
}