#include "timezone.h"

#define SECS_PER_HOUR (3600)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24)
#define DAYS_PER_WEEK (7)

//static timeZoneRule_t dst;
//static timeZoneRule_t std;

//static time_t dstUTC;
//static time_t stdUTC;
//static time_t dstLoc;
//static time_t stdLoc;

static time_t toTime_t(const timeZoneRule_t* rule, int year);

static time_t toTime_t(const timeZoneRule_t* rule, int year)
{
    struct tm ts = {0};

    /* Set time structure to the first day of the month of the rule.  For the
     * last week rule, treat as the first week of the next month  */
    ts.tm_hour = rule->hour;
    ts.tm_mday = 1;
    ts.tm_mon  = (rule->week == Last) ? (rule->month + 1) : (rule->month);
    ts.tm_year = year;

    /* Adjust time structure and calculate day of week */
    mktime(&ts);

    /* Adjust for for day of week */
    ts.tm_mday += (rule->dow - ts.tm_wday + DAYS_PER_WEEK) % DAYS_PER_WEEK;

    /* Adjust for week of month.  For last week rule, subtract one week. */
    ts.tm_mday += (rule->week == Last) ? (-7) : (rule->week * DAYS_PER_WEEK);

    return mktime(&ts);
}

time_t toLocal(time_t utc)
{
    return 0;
}

time_t toUTC(time_t local)
{
    return 0;
}
