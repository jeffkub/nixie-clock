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

#ifndef __RTC_H
#define __RTC_H

/* Includes *******************************************************************/
#include <stdbool.h>
#include <time.h>


/* Definitions ****************************************************************/
#define PREDIV_A 		127
#define PREDIV_S 		255

#define SUBSEC_PER_SEC 	(PREDIV_S + 1)


/* Public function prototypes *************************************************/
void rtc_init(void);

void rtc_wait(void);

void rtc_getTime(struct tm * ts, int * subsec);
void rtc_getTimeFromISR(struct tm * ts, int * subsec);
void rtc_setTime(const struct tm * ts);

void rtc_adjust(int offset);


#endif /* __RTC_H */
