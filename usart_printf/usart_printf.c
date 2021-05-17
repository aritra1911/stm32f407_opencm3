/* Modified `usart_printf.c' from
https://github.com/libopencm3/libopencm3-examples/blob/master/examples/stm32/f1/lisa-m-2/usart_printf/usart_printf.c
 * To work with STM32F407VG
 */

#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

#define USART_CONSOLE USART2

int _write(int, char*, int);

static void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    /* Enable GPIOD clock for LED & USARTs. */
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Enable clocks for USART2 */
    rcc_periph_clock_enable(RCC_USART2);
}

static void usart_setup(void) {
    /* Setup GPIO pins for USART2 transmit. */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

    /* Setup USART2 TX pin as alternate function. */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

    /* Setup USART2 parameters */
    usart_set_baudrate(USART_CONSOLE, 115200);
    usart_set_databits(USART_CONSOLE, 8);
    usart_set_stopbits(USART_CONSOLE, USART_STOPBITS_1);
    usart_set_mode(USART_CONSOLE, USART_MODE_TX);
    usart_set_parity(USART_CONSOLE, USART_PARITY_NONE);
    usart_set_flow_control(USART_CONSOLE, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART_CONSOLE);
}

static void gpio_setup(void)
{
    /* Setup GPIO pin GP12 on GPIO port D for LED */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

    /* Turn on LED at GPIO12 */
    gpio_set(GPIOD, GPIO12);
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

int main(void) {
    int counter = 0;
    float fcounter = 0.0;
    double dcounter = 0.0;

    clock_setup();
    gpio_setup();
    usart_setup();

    /*
     * Write Hello World, an integer, float and double all over
     * again while incrementing the counters.
     */
    while ( 1 ) {
        printf("Hello, World! %i %.2f %.2lf\n", counter, fcounter, dcounter);

        /* Toggle LED attached to GPIO12 indicating successful write. */
        gpio_toggle(GPIOD, GPIO12);

        counter++;
        fcounter += 0.01;
        dcounter += 0.01;

        /* wait a little before printing again. */
        for (uint32_t i = 0; i < 0x000FFFFF; i++) {
            __asm__("nop");
        }
    }

    return 0;
}
