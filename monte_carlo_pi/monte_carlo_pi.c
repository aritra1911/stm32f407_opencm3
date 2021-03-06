#include <stdio.h>
#include <math.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f4/rng.h>

#define ACTUAL_PI 3.14159265358979323846264338327950288

static void rcc_setup(void) {
    rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]);

    /* Enable GPIOD clock for onboard blinkies. */
    rcc_periph_clock_enable(RCC_GPIOD);

    /* Enable GPIOA clock for USART */
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Enable rng clock */
    rcc_periph_clock_enable(RCC_RNG);

    /* Enable clocks for USART2. */
    rcc_periph_clock_enable(RCC_USART2);
}

static void rng_setup(void) {
    /* Enable interrupt by seting the IE bit in the RNG_CR register. */
    RNG_CR |= RNG_CR_IE;

    /* Enable the RNG by setting the RNGEN bit in the RNG_CR register. This
       activates the analog part, the RNG_LFSR and the error detector. */
    RNG_CR |= RNG_CR_RNGEN;
}

static void gpio_setup(void) {
    /* Setup GPIO pins for USART2 transmit. */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

    /* Setup USART2 TX pin as alternate function. */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);

    /* Setup GPIO pin GPIO14 on GPIO port D for LED. */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO14);
}

static void usart_setup(void) {
    /* Setup USART2 parameters. */
    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    /* Finally enable the USART. */
    usart_enable(USART2);
}

static uint32_t random_u32(void) {
    /* Initialize them to the same thing so that the first integer that we
       return is not a grabage random value */
    static uint32_t last_value = 0;
    static uint32_t new_value  = 0;

    /* Reading CEIS bit as 1 means there was a `clock error'
       Reading SEIS bit as 1 means there was a `seed error' */
    uint32_t error_bits = RNG_SR_CEIS | RNG_SR_SEIS;

    do {
        /* Loop until we've got errors or data is not ready */
        while ( (RNG_SR & error_bits)  || !(RNG_SR & RNG_SR_DRDY) );

        /* Read the new value from the data register. */
        new_value = RNG_DR;

    } /* Try again if we haven't got a different value */
    while ( new_value == last_value );

    /* Keep track of the generated value and return it */
    last_value = new_value;
    return new_value;
}

static float random_float(void) {
    /* Returns a float between -1.0 and +1.0 */
    return random_u32() / (double) 0x7fffffff - 1.;
}

static float approximate_pi(uint32_t max_points) {
    uint32_t incircle_points = 0;

    for ( uint32_t i = 0; i < max_points; i++ ) {
        float x = random_float();
        float y = random_float();

        if ( x*x + y*y < 1. )
            incircle_points++;
    }

    return (incircle_points / (double) max_points) * 4.;
}

int main(void) {
    rcc_setup();
    rng_setup();
    gpio_setup();
    usart_setup();

    float pi, error;

    while ( 1 ) {
        pi = approximate_pi(100000);
        error = fabs(ACTUAL_PI - pi);
        printf("Pi is approximately : %1.22f    (error : %1.22f)\n", pi, error);
    }
    return 0;
}
