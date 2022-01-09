#ifndef UTIL_H_
#define UTIL_H_

// Standard Library Includes
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

// STM32 Includes 
#include "usb_device.h"
#include "usbd_cdc_if.h"

extern void util_init(void);
extern void util_printf(const char *fmt, ...);

#endif /* UTIL_H_ */