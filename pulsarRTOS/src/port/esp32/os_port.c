/* src/port/esp32/os_port.c */
#include "os_port.h"
#include "esp_attr.h" // IRAM_ATTR vb. gerekirse diye

/* * ESP32 (Xtensa LX6) Stack Frame Yapısı
 * İşlemci bir fonksiyonu böldüğünde veya biz yeni bir task başlattığımızda
 * Stack'te şu sırayla veri bekler:
 * [PC] [PS] [A0] [A1] ... [A15]
 */






// Assembly fonksiyonunun adını C'ye tanıtıyoruz
extern void _pulsar_context_switch(void);
extern void _pulsar_start_first_task(void);

// os_port.c içindeki os_port_stack_init fonksiyonunu şu şekilde güncelle:

/* os_port.c */
#include "os_port.h"

uint32_t* os_port_stack_init(uint32_t *pxTopOfStack, os_task_func_t pxCode) {
    // 1. Stack'i 16-byte hizala (Zorunludur)
    uint32_t *pxStack = (uint32_t *)(((uint32_t)pxTopOfStack) & (~((uint32_t)0xf)));

    // 2. Xtensa için 16 word (64 byte) yer aç
    pxStack -= 16; 

    // Tüm alanı temizle
    for(int i = 0; i < 16; i++) pxStack[i] = 0;

    // PS: 0x00060020 (Windowed Mode, Interrupts Enabled, Level 0)
    pxStack[1] = 0x00000020;         
    
    // PC: Task'ın giriş adresi
    pxStack[0] = (uint32_t)pxCode;   

    // A0 (Return Address): 0 yapıyoruz, task bitmemeli.
    pxStack[2] = 0; 
    
    // A1 (Stack Pointer): Task'ın kendi stack'ini bilmesi için
    pxStack[3] = (uint32_t)(pxStack + 16); 

    return pxStack; 
}















/*
uint32_t* os_port_stack_init(uint32_t *pxTopOfStack, os_task_func_t pxCode) {
    uint32_t *pxStack = pxTopOfStack;

    // 1. Hizalama (Alignment)
    // Xtensa stack pointer'ı 16-byte hizalı olmalıdır.
    // 0xf (15) bitlerini temizleyerek aşağı yuvarlıyoruz.
    pxStack = (uint32_t *)(((uint32_t)pxStack) & (~((uint32_t)0xf)));

    // --- Sahte Registerları Diyoruz ---
    // Stack aşağı doğru büyür (--pxStack)

    // PC (Program Counter): Task ilk çalıştığında nereye atlayacak?
    *(--pxStack) = (uint32_t)pxCode;

    // PS (Processor State): İşlemcinin durumu ne olsun?
    // 0x00060020 -> User mode, Interrupt Level 0 (Kesmeler Açık), WindowStart=1
    *(--pxStack) = 0x00060020;

    // A0 - A15 Registerları (Genel Amaçlı)
    // Hepsini 0 ile temizliyoruz ki task temiz bir sayfayla başlasın.
    for (int i = 0; i < 16; i++) {
        // Xtensa ABI: A1 register'ı Stack Pointer'dır ama o zaten donanım tarafından yönetilir.
        // Biz burada stack içine kaydedilen kopyalarını sıfırlıyoruz.
        *(--pxStack) = 0;
    }

    // NOT: Eğer task'a parametre (void *pvParam) göndereceksek
    // Xtensa'da ilk parametre A2 register'ında saklanır.
    // Stack'teki A2 pozisyonuna parametreyi yazabiliriz (İleride ekleriz).

    return pxStack; // Yeni güncellenmiş Stack Pointer'ı döndür
}
*/

















// Scheduler'ı başlatmak için çağrılacak (Birazdan Assembly'e bağlayacağız)
void os_port_start_first_task(void) {
    // Tüm kesmeleri kapat (Güvenlik için)
    // ESP32'de portDISABLE_INTERRUPTS() benzeri bir işlem gerekebilir ama
    // şimdilik Assembly'e güveniyoruz.

    _pulsar_start_first_task(); // Ve bitti. Buradan geri dönüş yok.
}




// Kernel veya Task "Sırayı saldım" (Yield) dediğinde bu çalışır
void pulsar_port_yield_from_isr(void) {
    _pulsar_context_switch();
}

