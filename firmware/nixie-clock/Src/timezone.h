#ifndef __TIMEZONE_H
#define __TIMEZONE_H

#include <time.h>

enum week_t  { First=0, Second, Third, Fourth, Last }; 
enum dow_t   { Sun=0, Mon, Tue, Wed, Thu, Fri, Sat };
enum month_t { Jan=0, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };

typedef struct timeZoneRule
{
    char abbrev[6]; /* Five chars max */
    int  week;      /* First, Second, Third, Fourth, or Last week of the month */
    int  dow;       /* day of week, 0=Sun, 1=Mon, ... 6=Sat */
    int  month;     /* 0=Jan, 1=Feb, ... 11=Dec */
    int  hour;      /* 0-23 */
    int  offset;    /* offset from UTC in minutes */
} timeZoneRule_t;

void   timezone_setTimezone(const timeZoneRule_t* dstRule, const timeZoneRule_t* stdRule);
time_t timezone_toLocal(time_t utc);

#endif /* __TIMEZONE_H */
