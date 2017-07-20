#ifndef __RTC_H
#define __RTC_H

#include <time.h>

void rtc_init(void);

void rtc_wait(void);

time_t rtc_getTime(void);
int    rtc_setTime(time_t time);

#endif /* __RTC_H */
