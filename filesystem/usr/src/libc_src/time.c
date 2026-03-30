#include <time.h>
#include <sys/io.h>
#include <stddef.h>

size_t strftime(char s[], size_t max, const char *format, const struct tm *tm){
    
}

time_t time(time_t *tloc){
    time_t t = 0;
    int clock_fd = open("/-/dev/RTC", O_READ);
    if(clock_fd == -1) goto fail;

    struct RealTimeClock {
        unsigned char second;
        unsigned char minute;
        unsigned char hour;
        unsigned char day;
        unsigned char month;
        unsigned int year;
        unsigned int msec;
    } rtc;

    read(clock_fd, &rtc, sizeof(rtc));
    close(clock_fd);

    t = rtc.second + 60*rtc.minute + 60*60*rtc.hour + 60*60*24*rtc.day + 60*60*24*30*rtc.month + 60*60*24*30*12*rtc.year;

    fail:;
    if(tloc != NULL) *tloc = t;
    return t;
}

/* Days in each month for normal and leap years */
static const int days_in_month[2][12] = {
    { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 },
    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 }
};

static inline int is_leap_year(int year)
{
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

static int day_of_week(int year, int month, int day)
{
    static const int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    if (month < 3)
        year--;
    return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}

static void time_t_to_tm(time_t t, struct tm *result, long utc_offset)
{
    /* Adjust for timezone */
    t += utc_offset;

    result->tm_sec = (int)(t % 60);
    t /= 60;
    result->tm_min = (int)(t % 60);
    t /= 60;
    result->tm_hour = (int)(t % 24);
    t /= 24;

    /* t is now the number of days since epoch (Jan 1 1970) */

    /* Compute year */
    int year = 1970;
    while (1) {
        int days_in_year = is_leap_year(year) ? 366 : 365;
        if (t < days_in_year)
            break;
        t -= days_in_year;
        year++;
    }

    /* Compute month */
    int leap = is_leap_year(year);
    int month = 0;
    while (month < 12 && t >= days_in_month[leap][month]) {
        t -= days_in_month[leap][month];
        month++;
    }

    int day = (int)t + 1;

    result->tm_mday  = day;
    result->tm_mon   = month;
    result->tm_year  = year - 1900;
    result->tm_wday  = day_of_week(year, month + 1, day);
    result->tm_yday  = 0;
    result->tm_isdst = 0;

    /* Compute day of year */
    for (int m = 0; m < month; m++)
        result->tm_yday += days_in_month[leap][m];
    result->tm_yday += day - 1;
}

/* Static storage — mirrors how the standard library works */
static struct tm gmtime_tm;
static struct tm localtime_tm;

struct tm *gmtime(const time_t *timep)
{
    time_t_to_tm(*timep, &gmtime_tm, 0);
    gmtime_tm.tm_gmtoff = 0;
    gmtime_tm.tm_zone   = "UTC";
    return &gmtime_tm;
}

struct tm *localtime(const time_t *timep)
{
    /*
     * Replace __get_utc_offset() with however your OS exposes
     * the system timezone offset in seconds east of UTC.
     */
    long offset = 0;
    time_t_to_tm(*timep, &localtime_tm, offset);
    localtime_tm.tm_gmtoff = offset;
    localtime_tm.tm_zone   = NULL; /* set if your OS has a timezone name */
    return &localtime_tm;
}
