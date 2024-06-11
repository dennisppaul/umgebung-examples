#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#include "ArduinoFunctions.h"

/* sketch */

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

#ifndef WEAK
#define WEAK __attribute__((weak))
#endif

extern void setup(void);
extern void loop(void);

void yield(void);

#ifdef __cplusplus
} // extern "C"
#endif // __cplusplus
