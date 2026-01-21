/* src/port/esp32/os_port.h */
#ifndef OS_PORT_H
#define OS_PORT_H

#include "os_types.h" // os_task_func_t için

#ifdef __cplusplus
extern "C" {
#endif

// Stack hazırlama fonksiyonu prototipi
uint32_t* os_port_stack_init(uint32_t *pxTopOfStack, os_task_func_t pxCode);

// İlk taskı başlatma fonksiyonu (Assembly çağıracak)
void os_port_start_first_task(void);

// Context Switch Tetikleyici
void pulsar_port_yield_from_isr(void);

#ifdef __cplusplus
}
#endif

#endif
