#include <unistd.h>
#include <errno.h>
#include <libopencm3/stm32/usart.h>

int _write(int, char*, int);

/**
 * Use USART2 as a console.
 * This is a syscall for newlib
 * @param file
 * @param ptr
 * @param len
 * @return
 */
__attribute__((weak)) int _write(int file, char *ptr, int len) {
    int i;

    if (file == STDOUT_FILENO || file == STDERR_FILENO) {
        for (i = 0; i < len; i++) {
            usart_send_blocking(USART2, ptr[i]);
        }
        return i;
    }
    errno = EIO;
    return -1;
}
