#include "gtimer.h"
#include "driver/gptimer.h"
#include "esp_attr.h"
#include "../kernel/os_kernel.h" // Kernel'e haber vermek için ekledik!

static volatile uint32_t __pulsar_ticks = 0;
static gptimer_handle_t timer_handle = NULL;

// ISR (Kesme Servis Rutini)
static bool IRAM_ATTR on_timer_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    __pulsar_ticks++;

    // --- KRİTİK NOKTA ---
    // Her 1ms'de bir Kernel'in "Tick Handler" fonksiyonunu çağırıyoruz.
    // Bu fonksiyon scheduler'ı tetikleyecek.
    pulsar_kernel_tick_handler();

    return false;
}

void gtimer_init(void) {
    // Senin yazdığın gptimer yapılandırma kodları aynen buraya geliyor...
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = 1000000,
    };
    gptimer_new_timer(&timer_config, &timer_handle);

    gptimer_alarm_config_t alarm_config = {
        .reload_count = 0,
        .alarm_count = 1000, // 1ms
        .flags.auto_reload_on_alarm = true,
    };
    gptimer_set_alarm_action(timer_handle, &alarm_config);

    gptimer_event_callbacks_t cbs = {
        .on_alarm = on_timer_alarm_cb,
    };
    gptimer_register_event_callbacks(timer_handle, &cbs, NULL);

    gptimer_enable(timer_handle);
    gptimer_start(timer_handle);
}

uint32_t gtimer_get_tick(void) {
    return __pulsar_ticks;
}
