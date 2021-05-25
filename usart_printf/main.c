#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <usart_printf.h>

static void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    /* Enable GPIOD clock for LED & USARTs. */
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Enable clocks for USART2 */
    rcc_periph_clock_enable(RCC_USART2);
}

static void gpio_setup(void)
{
    /* Setup GPIO pin GP12 on GPIO port D for LED */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);

    /* Turn on LED at GPIO12 */
    gpio_set(GPIOD, GPIO12);
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

        /* wait a little before printing again. *
        for (uint32_t i = 0; i < 0x00FFFFFF; i++) {
            __asm__("nop");
        }*/
    }

    return 0;
}
