#include <stddef.h>
#include "kernel/os_kernel.h"
#include "kernel/os_task.h"         // pxReadyTasks ve pxCurrentTCB'ye erişim için
#include "hal/gtimer.h"
#include "port/esp32/os_port.h"

// Kernel durum değişkeni
static uint8_t os_running = 0;

// --- KERNEL TICK HANDLER ---
// Bu fonksiyon her 1ms'de bir kesme (ISR) içinden çağrılır.
void pulsar_kernel_tick_handler(void) {
    if (!os_running) return;

    // 1. Taskların uyku sürelerini (delay) kontrol et
    for (int i = 0; i < uxTaskCount; i++) {
        if (pxReadyTasks[i]->state == BLOCKED) {
            if (pxReadyTasks[i]->timeout > 0) {
                pxReadyTasks[i]->timeout--;
            }
            if (pxReadyTasks[i]->timeout == 0) {
                pxReadyTasks[i]->state = READY; // Süre doldu, uyandır
            }
        }
    }

    // 2. Preemptive Karar Mekanizması
    // Basitçe: "Sıradaki taska geçme zamanı geldi mi?"
    // Burada şimdilik Round-Robin (Sırayla) mantığı kuruyoruz.
    // Gerçek switch, port katmanındaki Assembly kodunu tetikleyerek yapılır.
    pulsar_port_yield_from_isr();
}

void pulsar_delay(uint32_t ms) {
    if (pxCurrentTCB != NULL) {
        pxCurrentTCB->timeout = ms;
        pxCurrentTCB->state = BLOCKED;
        pulsar_yield(); // İşlemciyi hemen başkasına devret
    }
}

void pulsar_kernel_start(void) {
    if (uxTaskCount > 0) {
        os_running = 1;
        gtimer_init(); // Kalbi başlat
        os_port_start_first_task(); // İlk taska zıpla (Assembly)
    }
}

void pulsar_kernel_init(void) {
    // İleride buraya liste temizleme (reset) kodları gelecek
    uxTaskCount = 0;
    pxCurrentTCB = NULL;
    //idle yap
    pulsar_task_create(os_idle_task, "IDLE", 2048, 0);
}





void os_idle_task(void *param){
    while(1){
        pulsar_yield();
    }
}






void pulsar_yield(void) {
    // Eğer OS çalışıyorsa, hemen Port katmanına context switch yapmasını söyle
    // Bu çağrı, Assembly tarafındaki _pulsar_context_switch'i tetikleyecek
    pulsar_port_yield_from_isr(); 
}
