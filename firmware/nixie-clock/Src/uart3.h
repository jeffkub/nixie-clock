#ifndef __UART3_H
#define __UART3_H

#include <sys/types.h>

void uart3_init(void);

ssize_t uart3_read(void * buf, size_t nbyte);

char * uart3_gets(char * str, size_t maxlen);

#endif /* __UART3_H */
