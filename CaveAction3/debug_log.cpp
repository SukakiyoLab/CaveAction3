#include "debug_log.h"

#include <eigen3/Eigen/Core>
#include <stdio.h>
#include <stdarg.h>

using namespace Eigen;

namespace debug
{

	void debugLog(const char *format, ...)
	{
#ifdef _DEBUG
		/*
		va_list valist;
		va_start(valist, format);
		vprintf(format, valist);
		va_end(valist);
		*/
#endif
	}

	void print(std::string category, Vector3d vec)
	{
		debugLog("[%s]:\n", category.c_str());
		debugLog("	x: %f, y: %f, z: %f\n", vec(0), vec(1), vec(2));
	}

}
