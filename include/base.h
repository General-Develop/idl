#ifndef BASE_H
#define BASE_H

#include <boost/cstdint.hpp>

#include "common/declexport.h"

#ifndef IN
#	define IN
#endif

#ifndef OUT
#	define OUT
#endif

#ifndef LOCALTIME
#	if defined(_MSC_VER)
#		define LOCALTIME(tm, time_t)		localtime_s(tm, time_t)
#	else
#		define LOCALTIME(tm, time_t)		localtime_r(time_t, tm)
#	endif
#endif

#endif