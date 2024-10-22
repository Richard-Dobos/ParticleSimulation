#pragma once

#include "glew.h"
#include "glfw3.h"


#ifdef RENDERING_CORE_API_EXPORT_DLL
	#define CORE_API __declspec(dllexport)
#else
	#define CORE_API __declspec(dllimport)
#endif

#define RESET   "\033[0m"

#define BLACK	"\033[30m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGNETA	"\033[35m"
#define CYAN	"\033[36m"

#define BRIGHT_RED		"\033[90m"
#define BRIGHT_GREEN	"\033[91m"
#define BRIGHT_YELLOW	"\033[92m"
#define BRIGHT_BLUE		"\033[93m"
#define BRIGHT_MAGNETA	"\033[94m"
#define BRIGHT_CYAN		"\033[95m"
#define BRIGHT_BLACK	"\033[96m"

#define RED_BACKGROUND		"\033[41m"
#define GREEN_BACKGROUND	"\033[42m"
#define YELLOW_BACKGROUND	"\033[43m"
#define BLUE_BACKGROUND		"\033[44m"
#define MAGNETA_BACKGROUND	"\033[45m"
#define CYAN_BACKGROUND		"\033[46m"
#define WHITE_BACKGROUND	"\033[47m"
