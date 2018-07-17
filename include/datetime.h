#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include "base.h"

namespace Vdl
{
	// 与1970年1月1日0点0分0秒的开始的毫秒数
	typedef boost::int64_t DateTime;

	static const int DATE_TIME_STRING_SIZE = 24;

	static const DateTime ERROR_DATE_TIME = 0;

	class DECL_EXPORT DateTimeHelper
	{
	public:
		// IN: 2017-01-01 00:00:00.000
		// OUT: 与1970年1月1日0点0分0秒的开始的毫秒数
		static DateTime FromString(const std::string& strTime);
		// IN: 与1970年1月1日0点0分0秒的开始的毫秒数  
		// OUT: 2017-01-01 00:00:00.000
		static std::string ToString(const DateTime& time);


		static DateTime CurrentDateTime();

	};
}

#endif