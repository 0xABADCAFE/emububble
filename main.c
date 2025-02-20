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

extern LONG test_add_mem_to_reg_unrolled_4x(
    REG(d0, ULONG count),
    REG(d1, LONG step)
);

extern LONG test_add_mem_to_reg_unrolled_4x_pf(
    REG(d0, ULONG count),
    REG(d1, LONG step)
);


ULONG const BENCH_ITERATIONS = 10000000;
LONG  const STEP_SIZE = 3;

int main(void) {
    if (get_timer()) {
        LONG result;
        ULONG ticks, ms;

        ReadEClock(&clk_begin.ecv);
        result = test_add_mem_to_reg(
            BENCH_ITERATIONS,
            STEP_SIZE
        );
        ReadEClock(&clk_end.ecv);

        ticks = (ULONG)(clk_end.ticks - clk_begin.ticks);
        ms    = (1000 * ticks)/clock_freq_hz;

        printf(
            "Iterations: %lu, step: %ld\n"
            "Result: %ld, expected %ld\n"
            "Time: %lu EClock ticks (%lu ms)\n",
            BENCH_ITERATIONS,
            STEP_SIZE,
            result, (BENCH_ITERATIONS * STEP_SIZE),
            ticks, ms
        );

        ReadEClock(&clk_begin.ecv);
        result = test_add_mem_to_reg_unrolled_4x(
            BENCH_ITERATIONS,
            STEP_SIZE
        );
        ReadEClock(&clk_end.ecv);

        ticks = (ULONG)(clk_end.ticks - clk_begin.ticks);
        ms    = (1000 * ticks)/clock_freq_hz;

        printf(
            "Unrolled (4x):\n"
            "Result: %ld, expected %ld\n"
            "Time: %lu EClock ticks (%lu ms)\n",
            result, (BENCH_ITERATIONS * STEP_SIZE),
            ticks, ms
        );

        ReadEClock(&clk_begin.ecv);
        result = test_add_mem_to_reg_unrolled_4x_pf(
            BENCH_ITERATIONS,
            STEP_SIZE
        );
        ReadEClock(&clk_end.ecv);

        ticks = (ULONG)(clk_end.ticks - clk_begin.ticks);
        ms    = (1000 * ticks)/clock_freq_hz;

        printf(
            "Unrolled (4x):\n"
            "Result: %ld, expected %ld\n"
            "Time: %lu EClock ticks (%lu ms)\n",
            result, (BENCH_ITERATIONS * STEP_SIZE),
            ticks, ms
        );

        free_timer();
    }
    return 0;
}
