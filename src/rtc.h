#ifndef RTC_H
#define RTC_H

struct RealTimeClock {
	unsigned char second;
	unsigned char minute;
	unsigned char hour;
	unsigned char day;
	unsigned char month;
	unsigned int year;
	unsigned int msec;
};

extern struct RealTimeClock RTC;
void rtc_init();

#endif