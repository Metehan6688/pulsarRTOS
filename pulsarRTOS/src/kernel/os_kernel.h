#ifndef OS_KERNEL_H
#define OS_KERNEL_H

#include "os_types.h"

#ifdef __cplusplus
extern "C" {
#endif

// Kernel Kontrol API
void pulsar_kernel_init(void);           // Kernel'i hazırla
void pulsar_kernel_start(void);          // İlk taskı başlat (Geri dönüşü yok)
void pulsar_kernel_tick_handler(void);   // gtimer'ın her 1ms'de çağırdığı kalp atışı

// Yardımcılar
void pulsar_yield(void);                 // Task'ın kendi isteğiyle sırayı bırakması
void pulsar_delay(uint32_t ms);          // Milisaniye cinsinden uyuma
void os_idle_task(void *param);



#ifdef __cplusplus
}
#endif

#endif
