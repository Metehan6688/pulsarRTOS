
#include "kernel/os_task.h"
#include "port/esp32/os_port.h"
#include "os_types.h"
#include "os_config.h"// Port katmanına emir vermek için
#include <stdlib.h> // malloc/free için

// --- Global Değişkenler (Extern ile diğerlerine açılacak) ---
os_tcb_t* volatile pxReadyTasks[OS_MAX_TASKS];
os_tcb_t* volatile pxCurrentTCB = NULL;
uint8_t uxTaskCount = 0;

pulsar_status_t pulsar_task_create(os_task_func_t task_func, const char* name, uint32_t stack_size, uint8_t priority) {
    // 1. Görev Sınırı Kontrolü
    if (uxTaskCount >= OS_MAX_TASKS) {
        return PULSAR_ERR_FULL;
    }

    // 2. TCB (Kimlik Kartı) İçin Yer Ayır
    os_tcb_t* newTask = (os_tcb_t*)malloc(sizeof(os_tcb_t));
    if (newTask == NULL) return PULSAR_ERR_MALLOC_TCB;

    // 3. Stack (Çalışma Alanı) İçin Yer Ayır
    uint32_t* stackBase = (uint32_t*)malloc(stack_size);
    if (stackBase == NULL) {
        free(newTask);
        return PULSAR_ERR_MALLOC_STACK;
    }

    // 4. --- DONANIM BAĞLANTISI BURADA ---
    // Kernel, Port katmanına der ki: "Bu stack alanını ESP32'nin anlayacağı dilde hazırla"
    // Stack aşağı doğru büyüdüğü için bitiş adresini (Base + Size) veriyoruz.
    newTask->sp = os_port_stack_init(stackBase + (stack_size / 4), task_func);

    // 5. Task Bilgilerini Doldur
    newTask->priority = priority;
    newTask->name = name;
    newTask->state = READY;
    newTask->stackStart = stackBase; // İleride free() yapabilmek için başlangıcı sakla
    newTask->timeout = 0;

    // 6. Listeye Ekle
    pxReadyTasks[uxTaskCount++] = newTask;

    // Eğer bu ilk task ise, CurrentTCB olarak ata (daha scheduler çalışmadı)
    if (pxCurrentTCB == NULL) {
        pxCurrentTCB = newTask;
    } else {
        // Eğer yeni gelen task'ın önceliği şu ankinden yüksekse Context Switch iste (Preemptive)
        if (newTask->priority > pxCurrentTCB->priority) {
            // os_task_yield(); // İleride burayı açacağız
        }
    }

    return PULSAR_OK;
}


// os_task.c içine ekle
// os_task.c içine eklenecek
void os_task_switch_context(void) {
    static int next_task_index = 0;

    // Basit Round-Robin: Eğer birden fazla task varsa sıradakini seç
    if (uxTaskCount > 1) {
        next_task_index++;
        if (next_task_index >= uxTaskCount) {
            next_task_index = 0;
        }
        pxCurrentTCB = pxReadyTasks[next_task_index];
    }
}
