#include <xtensa/corebits.h>


.global _pulsar_start_first_task
.global _pulsar_context_switch
.extern pxCurrentTCB
.extern os_task_switch_context

/* port_asm.S */

.section .text
.align 4


_pulsar_start_first_task:
    /* Register pencerelerini temizle ve resetle */
    movi    a3, 0
    wsr     a3, WINDOWSTART
    movi    a3, 1
    wsr     a3, WINDOWSTART
    wsr     a3, WINDOWBASE
    rsync

    /* İlk taskın Stack Pointer'ını yükle */
    movi    a2, pxCurrentTCB
    l32i    a2, a2, 0       /* a2 = *pxCurrentTCB */
    l32i    a1, a2, 0       /* a1 = TCB->sp */

    /* Context Restore */
    l32i    a0, a1, 4       /* PS değerini al */
    wsr     a0, PS
    l32i    a0, a1, 0       /* PC (giriş adresi) değerini al */
    
    /* Stack alanını (64 byte) serbest bırak */
    addi    a1, a1, 64      
    rsync
    
    /* Task'a zıpla! */
    jx      a0
.align 4
_pulsar_context_switch:
    /* 1. Mevcut taskın durumunu kaydet (SAVE) */
    addi    a1, a1, -64     /* Stack'te 16 registerlık yer aç */
    s32i    a0, a1, 0       /* Geri dönüş adresini (PC) kaydet */
    rsr     a0, PS
    s32i    a0, a1, 4       /* İşlemci durumunu (PS) kaydet */
    
    /* Diğer kritik registerları sakla */
    s32i    a2, a1, 8
    s32i    a3, a1, 12
    s32i    a12, a1, 48
    /* ... (Basitlik için temel registerlar, Call0 ABI varsayımıyla) */

    /* 2. Mevcut Stack Pointer'ı TCB'ye kaydet */
    movi    a2, pxCurrentTCB
    l32i    a2, a2, 0
    s32i    a1, a2, 0

    /* 3. C tarafındaki scheduler'ı çağır (Sıradaki taskı seçer) */
    call0   os_task_switch_context

    /* 4. Yeni taskın Stack Pointer'ını yükle (RESTORE) */
    movi    a2, pxCurrentTCB
    l32i    a2, a2, 0
    l32i    a1, a2, 0

_restore_context:
    l32i    a0, a1, 4
    wsr     a0, PS          /* İşlemci durumunu geri yükle */
    l32i    a0, a1, 0       /* Hedef adresi (PC) geri yükle */
    
    addi    a1, a1, 64      /* Stack'i eski haline getir */
    rsync
    ret                     /* Yeni task'a zıpla! */
