#ifndef OS_TASK_H
#define OS_TASK_H

#include "../os_types.h"
#include "../os_config.h"

#ifdef __cplusplus
extern "C" {
#endif

// --- Kernel İç Değişkenleri (Extern) ---
// Diğer C dosyaları (Örn: os_kernel.c) bu listelere erişebilmeli
extern os_tcb_t* volatile pxReadyTasks[OS_MAX_TASKS];
extern uint8_t uxTaskCount;
extern os_tcb_t* volatile pxCurrentTCB;

// --- API Fonksiyonları ---
// Kullanıcının veya Kernel'in çağıracağı fonksiyonlar
pulsar_status_t pulsar_task_create(os_task_func_t task_func, const char* name, uint32_t stack_size, uint8_t priority);

// Task yönetim yardımcıları
void os_task_switch_context(void); // Scheduler tarafından çağrılır

#ifdef __cplusplus
}
#endif

#endif
