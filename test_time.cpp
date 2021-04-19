#include <iostream>
#include <time.h>
#include <sys/time.h>
//struct tm {
//	int tm_sec;    /* Seconds (0-60) */
//	int tm_min;    /* Minutes (0-59) */
//	int tm_hour;   /* Hours (0-23) */
//	int tm_mday;   /* Day of the month (1-31) */
//	int tm_mon;    /* Month (0-11) */
//	int tm_year;   /* Year - 1900 */
//	int tm_wday;   /* Day of the week (0-6, Sunday = 0) */
//	int tm_yday;   /* Day in the year (0-365, 1 Jan = 0) */
//	int tm_isdst;  /* Daylight saving time */
//};

std::string get_date(){
	struct tm info;
	struct timeval time;
	char buf[29];
	gettimeofday(&time, NULL);
	std::string s = std::to_string(time.tv_sec);
	strptime(s.c_str(), " %s ", &info);
	strftime (buf, sizeof(buf), "%a, %d %b %Y %X %Z", &info);
	std::string str = buf;
	return str;
}
int main()
{
	get_date();
	std::cout << get_date();
	return 0;
}
