#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void autotune_init();
void autotune_process(float* buf, int samples_read);
void autotune_free();

#ifdef __cplusplus
}
#endif
