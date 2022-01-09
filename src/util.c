#include "util.h"

void vprint(const char *fmt, va_list argp);

extern void util_init(void) {

    MX_USB_DEVICE_Init();

}

void vprint(const char *fmt, va_list argp)
{
    char string[200];
    if(0 < vsprintf(string,fmt,argp)) // build string
    {

        CDC_Transmit_FS((uint8_t*) string, strlen(string)); // send message via USB
        // HAL_UART_Transmit(&huart1, (uint8_t*)string, strlen(string), 0xffffff); // send message via UART
    }
}

extern void util_printf(const char *fmt, ...) // custom printf() function
{
    va_list argp;
    va_start(argp, fmt);
    vprint(fmt, argp);
    va_end(argp);
}