#include "timezone.h"

#include <stdbool.h>

#define SECS_PER_MIN  (60)
#define SECS_PER_HOUR (3600)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24)
#define DAYS_PER_WEEK (7)

static timeZoneRule_t dst;
static timeZoneRule_t std;

static int    toYear(time_t time);
static time_t toTime_t(const timeZoneRule_t* rule, int year);
static bool   utcIsDST(time_t utc);

static int toYear(time_t time)
{
    struct tm ts = {0};

    localtime_r(&time, &ts);

    return ts.tm_year;
}

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
    ts.tm_mday += (rule->week == Last) ? (-DAYS_PER_WEEK) : (rule->week * DAYS_PER_WEEK);

    return mktime(&ts);
}

static bool utcIsDST(time_t utc)
{
    int    year;
    time_t dstUTC;
    time_t stdUTC;
    bool   isDST;

    year   = toYear(utc);
    dstUTC = toTime_t(&dst, year);
    stdUTC = toTime_t(&std, year);

    if(stdUTC == dstUTC)
    {
        isDST = false;
    }
    else if(stdUTC > dstUTC)
    {
        isDST = (utc >= dstUTC && utc < stdUTC);
    }
    else
    {
        isDST = !(utc >= stdUTC && utc < dstUTC);
    }

    return isDST;
}

void timezone_setTimezone(const timeZoneRule_t* dstRule, const timeZoneRule_t* stdRule)
{
    dst = *dstRule;
    std = *stdRule;

    return;
}

time_t timezone_toLocal(time_t utc)
{
    time_t local;

    if(utcIsDST(utc))
    {
        local = utc + (dst.offset * SECS_PER_MIN);
    }
    else
    {
        local = utc + (std.offset * SECS_PER_MIN);
    }

    return local;
}
