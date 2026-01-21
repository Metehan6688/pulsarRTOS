// src/os_types.h
#ifndef OS_TYPES_H
#define OS_TYPES_H

#include <stddef.h>
#include <stdint.h>

typedef void (*os_task_func_t)(void*); // Görev fonksiyon tipi

typedef enum {
    READY,      // Çalışmaya hazır
    RUNNING,    // Şu an işlemcide olan
    BLOCKED,    // os_delay ile uyuyan
    SUSPENDED   // Tamamen durdurulmuş
} task_state_t;

typedef enum {
    PULSAR_OK               = 0,  // Her şey yolunda
    PULSAR_ERR_FULL         = 1,  // Maksimum görev sayısına ulaşıldı
    PULSAR_ERR_MALLOC_TCB   = 2,  // TCB için RAM yetersiz
    PULSAR_ERR_MALLOC_STACK = 3,  // Stack için RAM yetersiz
    PULSAR_ERR_INVALID_PRIO = 4,  // Geçersiz öncelik değeri
    PULSAR_ERR_ALREADY_RUN  = 5,  // OS zaten çalışıyor
    PULSAR_ERR_NOT_INIT     = 6   // OS henüz ilklendirilmedi
} pulsar_status_t;

typedef struct {
    uint32_t     *sp;           // Mevcut Stack Pointer (Yığın İşaretçisi)
    uint32_t     *stackStart;   // Bellek yönetimi için
    uint8_t      priority;      // Görev önceliği (0: En düşük)
    task_state_t state;         // Görev durumu
    uint32_t     timeout;       // os_delay için milisaniye sayacı
    const char   *name;         // Debug için isim
} os_tcb_t;

#endif
