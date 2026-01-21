#ifndef GTIMER_H
#define GTIMER_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void     gtimer_init(void);    // basicTick_init yerine
uint32_t gtimer_get_tick(void); // basicTick_get yerine

#ifdef __cplusplus
}
#endif

#endif
