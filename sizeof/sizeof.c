#include <stdio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

static void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    /* Enable GPIOA clock for USART. */
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Enable clock for USART2 */
    rcc_periph_clock_enable(RCC_USART2);
}

static void gpio_setup(void) {
    /* Setup GPIO pin for USART2 transmit. */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

    /* Setup USART TX pin as alternate function. */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
}

static void usart_setup(void) {
    /* Setup USART parameters */
    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART2);
}

int main(void) {
    clock_setup();
    gpio_setup();
    usart_setup();

    printf ( "                sizeof(char) = %u\n", sizeof(char) );
    printf ( "       sizeof(unsigned long) = %u\n", sizeof(unsigned long) );
    printf ( "  sizeof(unsigned long long) = %u\n", sizeof(unsigned long long) );
    printf ( "                 sizeof(int) = %u\n", sizeof(int) );
    printf ( "               sizeof(void*) = %u\n", sizeof(void*) );
    printf ( "               sizeof(float) = %u\n", sizeof(float) );
    printf ( "              sizeof(double) = %u\n", sizeof(double) );

    while ( 1 ) {
        __asm__("nop");
    }

    return 0;
}
