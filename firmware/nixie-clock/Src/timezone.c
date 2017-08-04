/*******************************************************************************
MIT License

Copyright (c) 2017 Jeff Kubascik

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*******************************************************************************/

/* Includes *******************************************************************/
#include "timezone.h"

#include <stdbool.h>


/* Private definitions ********************************************************/
#define SECS_PER_MIN  (60)
#define SECS_PER_HOUR (3600)
#define SECS_PER_DAY  (SECS_PER_HOUR * 24)
#define DAYS_PER_WEEK (7)


/* Private variables **********************************************************/
static timeZoneRule_t dst;
static timeZoneRule_t std;


/* Private function prototypes ************************************************/
static int    toYear(time_t time);
static time_t toTime_t(const timeZoneRule_t* rule, int year);
static bool   utcIsDST(time_t utc);


/* Private function definitions ***********************************************/
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
    time_t dst_loc;
    time_t std_loc;
    time_t dst_utc;
    time_t std_utc;
    bool   isDST;

    year   = toYear(utc);
    dst_loc = toTime_t(&dst, year);
    std_loc = toTime_t(&std, year);
    dst_utc = dst_loc - (std.offset * SECS_PER_MIN);
    std_utc = std_loc - (dst.offset * SECS_PER_MIN);

    if(std_utc == dst_utc)
    {
        isDST = false;
    }
    else if(std_utc > dst_utc)
    {
        isDST = (utc >= dst_utc && utc < std_utc);
    }
    else
    {
        isDST = !(utc >= std_utc && utc < dst_utc);
    }

    return isDST;
}


/* Public function definitions ************************************************/
void timezone_set(const timeZoneRule_t* dst_rule, const timeZoneRule_t* std_rule)
{
    dst = *dst_rule;
    std = *std_rule;

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
