#include "datetime.h"
using namespace Vdl;

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/local_time_adjustor.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/date_time/local_time_adjustor.hpp>

// ����ʱ����Ϣ
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

// ��boost::posix_time::ptime������DateTimeInfo������ʹ��
void ToDateTimeInfo(const boost::posix_time::ptime& dateTime, DateTimeInfo& dateTimeInfo)
{
	const boost::posix_time::ptime::date_type::ymd_type date = dateTime.date().year_month_day();
	const boost::posix_time::ptime::time_duration_type time = dateTime.time_of_day();

	// ע��date.year��date.month��date.day��һ�����󣬶�����һ��������
	// ����ֱ������sprintf()
	dateTimeInfo.year = (short)date.year;
	dateTimeInfo.month = (short)date.month;
	dateTimeInfo.day = (short)date.day;
	dateTimeInfo.hour = time.hours();
	dateTimeInfo.minute = time.minutes();
	dateTimeInfo.second = time.seconds();
	// ֻ֧�ֵ�����
	dateTimeInfo.millisecond = (int)(time.fractional_seconds() / 1000);
}

// ����utcʱ���뱾��ʱ���ת������������
typedef boost::date_time::local_adjustor<boost::posix_time::ptime, 8, boost::posix_time::no_dst> local_adj;

// 1. �����ʼʱ��"1970/1/1 00:00:00.000000"
// 2. ��ͨ��boost::posix_time::time_from_string("1970/1/1 00:00:00.000000")��
// ����������`boost::gregorian::greg_month::get_month_map_ptr()'��������linux������
// ����ʹ�øÿ�ʱ�������Ӵ���
// undefined reference to boost::gregorian::greg_month::get_month_map_ptr()
static const boost::posix_time::ptime seedDateTime(boost::gregorian::date(1970, 1, 1), 
	boost::posix_time::time_duration(0, 0, 0, 0));


class DateTimeParser
{
public:
	// ���ַ����н������ڣ����ڸ�ʽΪ��2013-01-02
	static bool ParseDate(const std::string& strDate, int& year, int& month, int& day);
	// ���ַ����н���ʱ�䣬ʱ���ʽΪ��23:04:05.678
	static bool ParseTime(const std::string& strTime, int& hour, int& minute, int& second, int& millisecond);
	// ���ַ����н�������ʱ�䣬��ʽΪ��2013-01-02 23:04:05.678
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
