#include <Arduino.h>
#include "pulsarRTOS.h"

// Kullanacağımız LED pini (ESP32 DevKit'lerde genelde 2. pin üzerindedir)
const int LED_PIN = 2;

// --- TASK 1: LED BLINK ---
// Task fonksiyonları asla geri dönmemeli (return etmemeli), 
// bu yüzden her zaman sonsuz bir while(1) döngüsü içindedirler.
void led_blink_task(void* param) {
    pinMode(LED_PIN, OUTPUT);
    
    while(1) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("PulsarRTOS: LED ON");
        pulsar_delay(1000); // 1 saniye bekle (Task uykusuna dalar)
        
        digitalWrite(LED_PIN, LOW);
        Serial.println("PulsarRTOS: LED OFF");
        pulsar_delay(1000); // 1 saniye bekle
    }
}

// --- TASK 2: MONITOR (Opsiyonel) ---
// İkinci bir task ekleyerek RTOS'un tasklar arasında geçiş yaptığını görebiliriz.
void monitor_task(void* param) {
    while(1) {
        Serial.println("---> Sistem Calisiyor...");
        pulsar_delay(2500); // Daha uzun aralıklarla mesaj atar
    }
}

void setup() {
    Serial.begin(115200);
    delay(2000); // Seri portun oturması için kısa bir bekleme
    Serial.println("PulsarRTOS Yukleniyor...");

    // 1. Kernel'i sıfırla ve hazırla
    pulsar_kernel_init(); 

    // 2. Taskları oluştur
    // Parametreler: Fonksiyon, İsim, Stack Boyutu (Byte), Öncelik
    pulsar_task_create(led_blink_task, "BlinkTask", 4096, 1);
    pulsar_task_create(monitor_task, "Monitor", 4096, 1);

    Serial.println("PulsarRTOS: Kernel Baslatiliyor...");
    
    // 3. Zamanlayıcıyı başlat ve ilk taska zıpla
    // Bu fonksiyondan sonraki kodlar ASLA çalışmaz.
    pulsar_kernel_start(); 
}

void loop() {
    // Boş kalmalı. RTOS artık işlemciyi yönettiği için Arduino'nun standart loop'u devre dışı kalır.
}