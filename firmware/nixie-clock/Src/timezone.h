#ifndef __TIMEZONE_H
#define __TIMEZONE_H

#include <time.h>

enum week_t  {Last, First, Second, Third, Fourth}; 
enum dow_t   {Sun=1, Mon, Tue, Wed, Thu, Fri, Sat};
enum month_t {Jan=1, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec};

struct timeZoneRule
{
    char    abbrev[6]; /* Five chars max */
    uint8_t week;      /* First, Second, Third, Fourth, or Last week of the month */
    uint8_t dow;       /* day of week, 1=Sun, 2=Mon, ... 7=Sat */
    uint8_t month;     /* 1=Jan, 2=Feb, ... 12=Dec */
    uint8_t hour;      /* 0-23 */
    int     offset;    /* offset from UTC in minutes */
};

time_t toLocal(time_t utc);
time_t toUTC(time_t local);

#endif /* __TIMEZONE_H */
