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

#ifndef __TIMEZONE_H
#define __TIMEZONE_H

/* Includes *******************************************************************/
#include <time.h>


/* Definitions ****************************************************************/
enum week_t  { First=0, Second, Third, Fourth, Last };
enum dow_t   { Sun=0, Mon, Tue, Wed, Thu, Fri, Sat };
enum month_t { Jan=0, Feb, Mar, Apr, May, Jun, Jul, Aug, Sep, Oct, Nov, Dec };

typedef struct timeZoneRule
{
    char abbrev[6]; /* Five chars max */
    int  week;      /* First, Second, Third, Fourth, or Last week of the month */
    int  dow;       /* Day of week 0=Sun, 1=Mon, ... 6=Sat */
    int  month;     /* Month of year 0=Jan, 1=Feb, ... 11=Dec */
    int  hour;      /* Local time 0-23 */
    int  offset;    /* Offset from UTC in minutes */
} timeZoneRule_t;


/* Public function prototypes ************************************************/
void   timezone_set(const timeZoneRule_t* dst_rule, const timeZoneRule_t* std_rule);
time_t timezone_toLocal(time_t utc);


#endif /* __TIMEZONE_H */
