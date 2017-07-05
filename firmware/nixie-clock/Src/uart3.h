#ifndef __UART3_H
#define __UART3_H

#include <sys/types.h>

void uart3_init(void);

ssize_t uart3_read(void * buf, size_t nbyte);

#endif /* __UART3_H */
