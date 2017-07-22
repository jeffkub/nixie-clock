#ifndef __RTC_H
#define __RTC_H

#include <stdbool.h>
#include <time.h>

void rtc_init(void);

void rtc_wait(void);

time_t rtc_getTime(void);
int    rtc_setTime(time_t time);

int32_t rtc_getTsOffset(void);

void rtc_adjust(uint32_t offset, bool advance);

#endif /* __RTC_H */
