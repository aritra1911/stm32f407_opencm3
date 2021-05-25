/* Modified `usart_printf.c' from
https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/lisa-m-2/usart_printf/usart_printf.c
 * To work with STM32F407VG
 */
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <usart_printf.h>

void usart_setup(void) {
    /* Setup GPIO pins for USART transmit. */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

    /* Setup USART TX pin as alternate function. */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

    /* Setup USART parameters */
    usart_set_baudrate(USART_CONSOLE, USART_BAUDRATE);
    usart_set_databits(USART_CONSOLE, 8);
    usart_set_stopbits(USART_CONSOLE, USART_STOPBITS_1);
    usart_set_mode(USART_CONSOLE, USART_MODE_TX);
    usart_set_parity(USART_CONSOLE, USART_PARITY_NONE);
    usart_set_flow_control(USART_CONSOLE, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART_CONSOLE);
}

/**
 * Use USART_CONSOLE as a console.
 * This is a syscall for newlib
 * @param file
 * @param ptr
 * @param len
 * @return
 */
int _write(int file, char *ptr, int len) {
    int i;

    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        for (i = 0; i < len; i++) {
            usart_send_blocking(USART_CONSOLE, ptr[i]);
        }
        return i;
    }
    errno = EIO;
    return -1;
}
