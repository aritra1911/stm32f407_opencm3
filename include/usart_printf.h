#ifndef _USART_PRINTF_H
# define _USART_PRINTF_H

# include <errno.h>
# include <stdio.h>
# include <unistd.h>
# include <libopencm3/stm32/usart.h>

int _write(int, char*, int);
void usart_setup(void);

#endif
