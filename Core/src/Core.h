#pragma once

#include "CoreConfig.h"

#include <format>
#include <iostream>

#if RENDERING_CORE_API_EXPORT_DLL
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif

#if defined (DEBUG_LOG_TRACE) || defined (DEBUG_LOG_ENABLE)
	#define LOG_TRACE(...) std::cout << WHITE << "[TRACE]: " << std::format(__VA_ARGS__, __VA_ARGS__) << '\n' << RESET
#else
	#define LOG_TRACE(...)
#endif
#if defined (DEBUG_LOG_INFO) || defined (DEBUG_LOG_ENABLE)
	#define LOG_INFO(...) std::cout << GREEN << "[INFO]: " << std::format(__VA_ARGS__, __VA_ARGS__) << '\n' << RESET
#else
	#define LOG_INFO(...)
#endif
#if defined (DEBUG_LOG_WARN) || defined (DEBUG_LOG_ENABLE)
	#define LOG_WARN(...) std::cout << YELLOW << "[WARN]: " << std::format(__VA_ARGS__, __VA_ARGS__) << '\n' << RESET
#else
	#define LOG_WARN(...)
#endif
#if defined (DEBUG_LOG_ERROR) || defined (DEBUG_LOG_ENABLE)
	#define LOG_ERROR(...) std::cerr << RED << "[ERORR]: " << std::format(__VA_ARGS__, __VA_ARGS__) << std::endl << RESET
#else
	#define LOG_ERROR(...)
#endif
#ifdef DEBUG_LOG_PERFORMANCE
	#define LOG_PERFORMANCE(...) std::cout << CYAN << "[PERFORMANCE]: " << std::format(__VA_ARGS__, __VA_ARGS__) << '\r' << RESET
#else
	#define LOG_PERFORMANCE(...)
#endif

#define BIT(x) (1 << x)

#define RESET   "\033[0m"

#define BLACK	"\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGNETA	"\033[35m"
#define CYAN	"\033[36m"
#define WHITE	"\033[37m"

#define BRIGHT_RED		"\033[90m"
#define BRIGHT_GREEN	"\033[91m"
#define BRIGHT_YELLOW	"\033[92m"
#define BRIGHT_BLUE		"\033[93m"
#define BRIGHT_MAGNETA	"\033[94m"
#define BRIGHT_CYAN		"\033[95m"
#define BRIGHT_BLACK	"\033[96m"
#define BRIGHT_WHITE	"\033[97m"

#define RED_BACKGROUND		"\033[41m"
#define GREEN_BACKGROUND	"\033[42m"
#define YELLOW_BACKGROUND	"\033[43m"
#define BLUE_BACKGROUND		"\033[44m"
#define MAGNETA_BACKGROUND	"\033[45m"
#define CYAN_BACKGROUND		"\033[46m"
#define WHITE_BACKGROUND	"\033[47m"

