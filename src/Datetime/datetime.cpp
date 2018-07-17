#include "datetime.h"
using namespace Vdl;

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/date_time/local_time_adjustor.hpp>

// 日期时间信息
struct DateTimeInfo
{
	int year;
	int month;
	int day;
	int hour;
	int minute;
	int second;
	int millisecond;
};

// 将boost::posix_time::ptime解析成DateTimeInfo，方便使用
void ToDateTimeInfo(const boost::posix_time::ptime& dateTime, DateTimeInfo& dateTimeInfo)
{
	const boost::posix_time::ptime::date_type::ymd_type date = dateTime.date().year_month_day();
	const boost::posix_time::ptime::time_duration_type time = dateTime.time_of_day();

	// 注意date.year，date.month和date.day是一个对象，而不是一个整数，
	// 不能直接用于sprintf()
	dateTimeInfo.year = (short)date.year;
	dateTimeInfo.month = (short)date.month;
	dateTimeInfo.day = (short)date.day;
	dateTimeInfo.hour = time.hours();
	dateTimeInfo.minute = time.minutes();
	dateTimeInfo.second = time.seconds();
	// 只支持到毫秒
	dateTimeInfo.millisecond = (int)(time.fractional_seconds() / 1000);
}

// 用于utc时间与本地时间的转换（东八区）
typedef boost::date_time::local_adjustor<boost::posix_time::ptime, 8, boost::posix_time::no_dst> local_adj;

// 1. 相对起始时间"1970/1/1 00:00:00.000000"
// 2. 不通过boost::posix_time::time_from_string("1970/1/1 00:00:00.000000")，
// 避免依赖于`boost::gregorian::greg_month::get_month_map_ptr()'，导致在linux下其他
// 程序使用该库时出现链接错误：
// undefined reference to boost::gregorian::greg_month::get_month_map_ptr()
static const boost::posix_time::ptime seedDateTime(boost::gregorian::date(1970, 1, 1), 
	boost::posix_time::time_duration(0, 0, 0, 0));


class DateTimeParser
{
public:
	// 从字符串中解析日期，日期格式为：2013-01-02
	static bool ParseDate(const std::string& strDate, int& year, int& month, int& day);
	// 从字符串中解析时间，时间格式为：23:04:05.678
	static bool ParseTime(const std::string& strTime, int& hour, int& minute, int& second, int& millisecond);
	// 从字符串中解析日期时间，格式为：2013-01-02 23:04:05.678
	static bool ParseDateTime(const std::string& strDateTime, boost::posix_time::ptime& dateTime);

private:
	static bool ParseInt(const std::string& strValue, int expectedWide, int& intValue);
};

bool DateTimeParser::ParseDate(const std::string& strDate, int& year, int& month, int& day)
{
	std::vector<std::string> strDateTokens;
	boost::split(strDateTokens, strDate, boost::is_any_of("-"));
	if (strDateTokens.size() != 3)
	{
		fprintf(stderr, "Wrong date format: %s\n", strDate.c_str());
		return false;
	}

	const std::string& strYear = strDateTokens[0];
	const std::string& strMonth = strDateTokens[1];
	const std::string& strDay = strDateTokens[2];

	if (!ParseInt(strYear, 4, year))
	{
		fprintf(stderr, "Wrong year format: %s\n", strDate.c_str());
		return false;
	}
	if (!ParseInt(strMonth, 2, month))
	{
		fprintf(stderr, "Wrong month format: %s\n", strDate.c_str());
		return false;
	}
	if (!ParseInt(strDay, 2, day))
	{
		fprintf(stderr, "Wrong day format: %s\n", strDate.c_str());
		return false;
	}

	return true;
}

bool DateTimeParser::ParseTime(const std::string& strTime, int& hour, int& minute, int& second, int& millisecond)
{
	std::vector<std::string> strTimeTokens;
	boost::split(strTimeTokens, strTime, boost::is_any_of(":."), boost::token_compress_on);

	if (strTimeTokens.size() != 4)
	{
		fprintf(stderr, "Wrong time format: %s\n", strTime.c_str());
		return false;
	}

	const std::string& strHour = strTimeTokens[0];
	const std::string& strMinute = strTimeTokens[1];
	const std::string& strSecond = strTimeTokens[2];
	const std::string strMillisecond = strTimeTokens[3];

	if (!ParseInt(strHour, 2, hour) || hour > 23 || hour < 0)
	{
		fprintf(stderr, "Wrong hour format: %s\n", strTime.c_str());
		return false;
	}
	if (!ParseInt(strMinute, 2, minute) || minute > 59 || minute < 0)
	{
		fprintf(stderr, "Wrong minute format: %s\n", strTime.c_str());
		return false;
	}
	if (!ParseInt(strSecond, 2, second) || second > 59 || second < 0)
	{
		fprintf(stderr, "Wrong second format: %s\n", strTime.c_str());
		return false;
	}
	if (!ParseInt(strMillisecond, 3, millisecond) || millisecond < 0)
	{
		fprintf(stderr, "Wrong microsecond format: %s\n", strTime.c_str());
		return false;
	}

	return true;
}

bool DateTimeParser::ParseDateTime(const std::string& strDateTime, boost::posix_time::ptime& dateTime)
{
	std::vector<std::string> strDateTimeTokens;
	boost::split(strDateTimeTokens, strDateTime, boost::is_any_of(" "), boost::token_compress_on);

	if (strDateTimeTokens.size() != 2)
	{
		fprintf(stderr, "Wrong date time format: %s\n", strDateTime.c_str());
		return false;
	}

	const std::string& strDate = strDateTimeTokens[0];
	const std::string& strTime = strDateTimeTokens[1];

	int year, month, day;
	if (!ParseDate(strDate, year, month, day))
	{
		return false;
	}

	int hour, minute, second, microsecond;
	if (!ParseTime(strTime, hour, minute, second, microsecond))
	{
		return false;
	}

	try
	{
		boost::gregorian::date date(year, month, day);
		boost::posix_time::time_duration time(hour, minute, second, microsecond * 1000);
		dateTime = boost::posix_time::ptime(date, time);
	}
	catch (const std::exception& exp)
	{
		fprintf(stderr, "Wrong date time format: %s, %s\n", 
			strDateTime.c_str(), exp.what());
		return false;
	}

	return true;
}

bool DateTimeParser::ParseInt(const std::string& strValue, int expectedWide, int& intValue)
{
	if (strValue.size() != expectedWide)
	{
		return false;
	}

	try 
	{
		intValue = boost::lexical_cast<int>(strValue);
	}
	catch (...)
	{
		return false;
	}

	return true;
}



DateTime Vdl::DateTimeHelper::FromString( const std::string& strTime )
{
	boost::posix_time::ptime destDateTime;
	if (!DateTimeParser::ParseDateTime(strTime, destDateTime))
	{
		return ERROR_DATE_TIME;
	}

	try
	{
		boost::posix_time::ptime utcTime = local_adj::local_to_utc(destDateTime);
		boost::posix_time::time_duration offsetTime = utcTime - seedDateTime;
		return offsetTime.total_milliseconds();
	}
	catch (const std::exception& exp)
	{
		fprintf(stderr, "Wrong date time format: %s, %s\n", 
			strTime.c_str(), exp.what());
		return ERROR_DATE_TIME;
	}

	return ERROR_DATE_TIME;
}

std::string Vdl::DateTimeHelper::ToString( const DateTime& time )
{
	boost::posix_time::ptime localDateTime;
	try 
	{
		localDateTime = local_adj::utc_to_local(
			seedDateTime + boost::posix_time::milliseconds(time));
	}
	catch (const std::exception& exp)
	{
		fprintf(stderr, "error time: %lld. %s.\n", time, exp.what());
		return "";
	}

	DateTimeInfo dateTimeInfo;
	ToDateTimeInfo(localDateTime, dateTimeInfo);

	char buffer[DATE_TIME_STRING_SIZE] = {0};
	sprintf_s(buffer, sizeof(buffer), "%04d-%02d-%02d %02d:%02d:%02d.%03d", 
		dateTimeInfo.year, dateTimeInfo.month, dateTimeInfo.day, 
		dateTimeInfo.hour, dateTimeInfo.minute, dateTimeInfo.second, 
		dateTimeInfo.millisecond);

	return buffer;
}

DateTime Vdl::DateTimeHelper::CurrentDateTime()
{
	boost::posix_time::ptime localTime;
	try
	{
		localTime = boost::posix_time::microsec_clock::local_time();
		boost::posix_time::ptime utcTime = local_adj::local_to_utc(localTime);
		boost::posix_time::time_duration offsetTime = utcTime - seedDateTime;
		return offsetTime.total_milliseconds();
	}
	catch (const std::exception& exp)
	{
		fprintf(stderr, "error time: %lld. %s.\n", localTime, exp.what());
		return ERROR_DATE_TIME;
	}
	
	return ERROR_DATE_TIME;
}
