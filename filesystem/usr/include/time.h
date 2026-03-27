#ifndef _TIME_H
#define _TIME_H

typedef long long time_t;

struct tm {
    int         tm_sec;    /* Seconds          [0, 60] */
    int         tm_min;    /* Minutes          [0, 59] */
    int         tm_hour;   /* Hour             [0, 23] */
    int         tm_mday;   /* Day of the month [1, 31] */
    int         tm_mon;    /* Month            [0, 11]  (January = 0) */
    int         tm_year;   /* Year minus 1900 */
    int         tm_wday;   /* Day of the week  [0, 6]   (Sunday = 0) */
    int         tm_yday;   /* Day of the year  [0, 365] (Jan/01 = 0) */
    int         tm_isdst;  /* Daylight savings flag */

    /* Since POSIX.1-2024:  */
    long        tm_gmtoff; /* Seconds East of UTC */
    const char *tm_zone;   /* Timezone abbreviation */
};

#ifndef _SIZET
#define _SIZET
typedef long unsigned int size_t;
#endif

size_t strftime(char s[], size_t max, const char *format, const struct tm *tm);
time_t time(time_t *tloc);

struct tm *localtime(const time_t *timep);
struct tm *gmtime(const time_t *timep);

#endif