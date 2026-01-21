#ifndef PULSAR_RTOS_H
#define PULSAR_RTOS_H

/* C++ (Arduino) tarafında C fonksiyonlarını kullanabilmek için şart */
#ifdef __cplusplus
extern "C" {
#endif

#include "os_types.h"
#include "os_config.h"
#include "kernel/os_kernel.h"
#include "kernel/os_task.h"
#include "port/esp32/os_port.h"

#ifdef __cplusplus
}
#endif

#endif
