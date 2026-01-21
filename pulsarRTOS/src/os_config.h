// src/os_config.h
#ifndef OS_CONFIG_H
#define OS_CONFIG_H

/* Sistem Zamanlaması */
#define OS_TICK_RATE_MS     1       // Senin basicTick kütüphanenle uyumlu
#define OS_CPU_CLOCK_HZ     240000000 // ESP32 standart hızı (240MHz)

/* Görev Limitleri */
#define OS_MAX_TASKS        16      // Aynı anda kaç görev (task) olabilir?
#define OS_IDLE_STACK_SIZE  1024    // Boşta durma görevi için yığın boyutu
#define OS_DEFAULT_STACK    2048    // Standart görev yığını (2KB)

/* Güvenlik ve Kripto Ayarları */
#define OS_ENABLE_STACK_CHECK 1     // Stack taşmasını kontrol et (Güvenlik için şart!)
#define OS_STACK_CANARY_VAL   0xDEADC0DE // Stack sonuna konulacak imza
#define OS_CRYPTO_ENABLED     1     // Kripto servislerini aktif et

/* Haberleşme */
#define OS_TERMINAL_UART_BAUD 115200
#define OS_MAX_MSG_LEN        64    // Terminalden gelecek maks. komut uzunluğu

#endif
