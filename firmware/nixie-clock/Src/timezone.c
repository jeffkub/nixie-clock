#include "timezone.h"

//static struct timeZoneRule dst;
//static struct timeZoneRule std;

//static time_t dstUTC;
//static time_t stdUTC;
//static time_t dstLoc;
//static time_t stdLoc;

static int year(time_t time);
static time_t toTime(struct timeZoneRule rule, int year);

static int year(time_t time)
{
    struct tm ts = {0};

    localtime_r(&time, &ts);

    return ts.tm_year + 1900;
}

static time_t toTime(struct timeZoneRule rule, int year)
{
    return 0;
}

time_t toLocal(time_t utc)
{
    return 0;
}

time_t toUTC(time_t local)
{
    return 0;
}
