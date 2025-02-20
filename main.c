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

extern LONG test_add_mem_to_reg(REG(d0, ULONG count), REG(d1, LONG step));
extern LONG test_add_mem_to_reg_unrolled_2x(REG(d0, ULONG count), REG(d1, LONG step));
extern LONG test_add_mem_to_reg_unrolled_4x(REG(d0, ULONG count), REG(d1, LONG step));
extern LONG test_add_mem_to_reg_unrolled_8x(REG(d0, ULONG count), REG(d1, LONG step));
extern LONG test_add_mem_to_reg_unrolled_2x_pf(REG(d0, ULONG count), REG(d1, LONG step));
extern LONG test_add_mem_to_reg_unrolled_4x_pf(REG(d0, ULONG count), REG(d1, LONG step));
extern LONG test_add_mem_to_reg_unrolled_8x_pf(REG(d0, ULONG count), REG(d1, LONG step));

typedef LONG (*TestCode)(REG(d0, ULONG count), REG(d1, LONG step));

typedef struct {
    char const* description;
    TestCode    test_function;
} TestCase;

TestCase test_cases[] = {
    {
        "Add mem to reg, tight loop",
        test_add_mem_to_reg
    },
    {
        "Add mem to reg, 2x unrolled",
        test_add_mem_to_reg_unrolled_2x
    },
    {
        "Add mem to reg, 4x unrolled",
        test_add_mem_to_reg_unrolled_4x
    },
    {
        "Add mem to reg, 8x unrolled",
        test_add_mem_to_reg_unrolled_8x
    },
    {
        "Add mem to reg, 2x unrolled, prefetched",
        test_add_mem_to_reg_unrolled_2x_pf
    },
    {
        "Add mem to reg, 4x unrolled, prefetched",
        test_add_mem_to_reg_unrolled_4x_pf
    },
    {
        "Add mem to reg, 8x unrolled, prefetched",
        test_add_mem_to_reg_unrolled_8x_pf
    },
};

ULONG const BENCH_ITERATIONS = 50000000;
LONG  const STEP_SIZE = 3;

int main(void) {
    if (get_timer()) {
        LONG result;
        ULONG ticks, ms;

        printf("Using %u iterations, step size is %d\n", BENCH_ITERATIONS, STEP_SIZE);

        for (unsigned i = 0; i < sizeof(test_cases)/sizeof(TestCase); ++i) {
            printf("\nTest Case %u: %s\n", i, test_cases[i].description);
            ReadEClock(&clk_begin.ecv);
            result = test_cases[i].test_function(
                BENCH_ITERATIONS,
                STEP_SIZE
            );
            ReadEClock(&clk_end.ecv);
            ticks = (ULONG)(clk_end.ticks - clk_begin.ticks);
            ms    = (1000 * ticks)/clock_freq_hz;
            printf(
                "Result: %d, expected %d\n"
                "Time: %u EClock ticks (%u ms)\n",
                result, (BENCH_ITERATIONS * STEP_SIZE),
                ticks, ms
            );
        }
        free_timer();
    }
    return 0;
}
