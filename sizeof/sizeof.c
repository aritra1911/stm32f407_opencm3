#include <libopencm3/stm32/rcc.h>
#include <usart_printf.h>

static void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    /* Enable GPIOD clock for LED & USARTs. */
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Enable clocks for USART2 */
    rcc_periph_clock_enable(RCC_USART2);
}

int main(void) {
    clock_setup();
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
