#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/cm3/nvic.h>

static void clock_setup(void)
{
    /* Enable GPIOD clock for LED & USARTs. */
    rcc_periph_clock_enable(RCC_GPIOD);
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Enable clocks for USART2. */
    rcc_periph_clock_enable(RCC_USART2);
}

static void usart_setup(void)
{
    /* Enable the USART2 interrupt. */
    nvic_enable_irq(NVIC_USART2_IRQ);

    /* Setup GPIO pins for USART2 transmit. */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2);

    /* Setup GPIO pins for USART2 receive. */
    gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO3);
    gpio_set_output_options(GPIOA, GPIO_OTYPE_OD, GPIO_OSPEED_25MHZ, GPIO3);

    /* Setup USART2 TX and RX pin as alternate function. */
    gpio_set_af(GPIOA, GPIO_AF7, GPIO2);
    gpio_set_af(GPIOA, GPIO_AF7, GPIO3);

    /* Setup USART2 parameters. */
    usart_set_baudrate(USART2, 115200);
    usart_set_databits(USART2, 8);
    usart_set_stopbits(USART2, USART_STOPBITS_1);
    usart_set_mode(USART2, USART_MODE_TX_RX);
    usart_set_parity(USART2, USART_PARITY_NONE);
    usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

    /* Enable USART2 Receive interrupt. */
    usart_enable_rx_interrupt(USART2);

    /* Finally enable the USART. */
    usart_enable(USART2);
}

static void gpio_setup(void)
{
    /* Setup GPIO pin GPIO12 on GPIO port D for LED. */
    gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12);
}

int main(void)
{
    clock_setup();
    gpio_setup();
    usart_setup();
    usart_enable_tx_interrupt(USART2);

    while (1) {
        __asm__("NOP");
    }

    return 0;
}

void usart2_isr(void)
{
    static uint8_t data = 'A';
    static uint8_t backspace[] = "\x1b[1D";
    static uint8_t transmitting_backspace = 0;
    static uint8_t index = 0;
    static uint8_t bottom_right[] = "\x1b[9999C\x1b[9999B\x1b[6n";
    static uint8_t transmitting_bottom_right = 1;
    static uint8_t receiving_cursor_position = 0;
    static uint8_t cursor_position_report[16];

    /* Check if we were called because of RXNE. */
    if (((USART_CR1(USART2) & USART_CR1_RXNEIE) != 0) &&
        ((USART_SR(USART2) & USART_SR_RXNE) != 0)) {

        /* Indicate that we got data. */
        gpio_toggle(GPIOD, GPIO12);

        if ( receiving_cursor_position ) {
            data = usart_recv(USART2);
            cursor_position_report[index++] = data;
            if ( data == 'R' ) {
                receiving_cursor_position = 0;
                index = 0;
                usart_enable_tx_interrupt(USART2);
            }
            return;
        }

        /* Retrieve the data from the peripheral. */
        data = usart_recv(USART2);

        if ( data == '\b' || data == '\177' ) {
            transmitting_backspace = 1;
        }

        /* Enable transmit interrupt so it sends back the data. */
        usart_enable_tx_interrupt(USART2);
    }

    /* Check if we were called because of TXE. */
    if (((USART_CR1(USART2) & USART_CR1_TXEIE) != 0) &&
        ((USART_SR(USART2) & USART_SR_TXE) != 0)) {

        if ( transmitting_backspace ) {
            data = backspace[index++];
            if ( data ) {
                usart_send(USART2, data);
            } else {
                transmitting_backspace = 0;
                index = 0;
                usart_disable_tx_interrupt(USART2);
            }
            return;
        }

        if ( transmitting_bottom_right ) {
            data = bottom_right[index++];
            if ( data ) {
                usart_send(USART2, data);
            } else {
                transmitting_bottom_right = 0;
                index = 0;
                usart_disable_tx_interrupt(USART2);
                receiving_cursor_position = 1;
            }
            return;
        }

        /* Put data into the transmit register. */
        usart_send(USART2, data);

        if (data == '\r') {
            data = '\n';
            return;
        }

        /* Disable the TXE interrupt as we don't need it anymore. */
        usart_disable_tx_interrupt(USART2);
    }
}
