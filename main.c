#include <stdio.h>
#include <SDI_compiler.h>

#include <exec/types.h>
#include <proto/exec.h>
#include <devices/timer.h>
#include <proto/timer.h>

typedef unsigned long long ULONG64;

typedef union {
    struct EClockVal ecv;
    ULONG64 ticks;
} ClockValue;

ULONG clock_freq_hz = 0;

ClockValue clk_begin, clk_end;

struct TimeRequest time_request;
struct Device* TimerBase = NULL;

struct Device* get_timer(void) {
    if (OpenDevice(TIMERNAME, UNIT_MICROHZ, &time_request.tr_node, 0) != 0) {
        return NULL;
    }
    TimerBase = time_request.tr_node.io_Device;
    clock_freq_hz = ReadEClock(&clk_begin.ecv);
    printf("Got Timer, frequency is %u Hz\n", clock_freq_hz);
    return TimerBase;
}

void free_timer(void) {
    if (TimerBase) {
        CloseDevice(&time_request.tr_node);
        TimerBase = NULL;
    }
}

extern LONG test_add_mem_to_reg(
    REG(d0, ULONG count),
    REG(d1, LONG step)
);

ULONG const BENCH_ITERATIONS = 10000000;
LONG  const STEP_SIZE = 3;

int main(void) {
    if (get_timer()) {

		ReadEClock(&clk_begin.ecv);
		LONG result = test_add_mem_to_reg(
			BENCH_ITERATIONS,
			STEP_SIZE
		);
		ReadEClock(&clk_end.ecv);
        free_timer();

		ULONG ticks = (ULONG)(clk_end.ticks - clk_begin.ticks);
		ULONG ms    = (1000 * ticks)/clock_freq_hz;

		printf(
			"Iterations: %lu, step: %ld\n"
			"Result: %ld, expected %ld\n"
			"Time: %lu EClock ticks (%lu ms)\n",
			BENCH_ITERATIONS,
			STEP_SIZE,
			result, (BENCH_ITERATIONS * STEP_SIZE),
			ticks, ms
		);
    }
    return 0;
}
