#ifndef CPU_INIT_H
#define CPU_INIT_H

#include "lq_include.h"

void cpu_init(void);
void SafetyWatchdog(void);
void readCPUFreq(void);
void enableInterruptLatency(void);

boolean disableInterruptLatency(void);

#endif


