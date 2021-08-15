#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/crc.h>

static void clock_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    /* Enable GPIOD clock for onboard blinkies. */
    rcc_periph_clock_enable(RCC_GPIOD);

    /* Enable GPIOA clock for USART. */
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Enable clock for USART2 */
    rcc_periph_clock_enable(RCC_USART2);

    /* Enable the CRC peripheral clock via the RCC peripheral. */
     rcc_periph_clock_enable(RCC_CRC);
}

static void gpio_setup(void) {
    /* Setup GPIO pin for USART2 transmit. */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

    /* Setup USART TX pin as alternate function. */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

    /* Setup GPIO pin GPIO14 on GPIO port D for LED. */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE,
                    GPIO12 | GPIO13 | GPIO14 | GPIO15);
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

static uint32_t calc_crc(const char *str) {
    /* We shall try to calculate the CRC-32 of `str` */

    /* Note: Here I'm assuming one character takes an entire 32-bit word. If
     * this is not desirable, for example one wants to shove 4 characters in a
     * 32-bit word, further processing of `str` is necessary. */

    int len = strlen(str);

    uint32_t *data = calloc(len, sizeof(uint32_t));

    for (int i = 0; i < len; i++) {
        data[i] = str[i];
    }

    crc_reset();
    uint32_t ret = crc_calculate_block(data, len);

    free(data);
    return ret;
}

int main(void) {
    clock_setup();
    gpio_setup();
    usart_setup();

    char hello[] = "Hello, World!";
    uint32_t res;

    gpio_set(GPIOD, GPIO12);

    while ( 1 ) {
        res = calc_crc(hello);
        printf("String : \"%s\"\n", hello);
        printf("Length : %d\n", strlen(hello));
        printf("CRC-32 : 0x%08"PRIx32"\n\n", res);

        for (uint32_t i = 0; i < 0x00ffffff; i++)
            __asm__("nop");

        gpio_toggle(GPIOD, GPIO12);
    }

    return 0;
}
