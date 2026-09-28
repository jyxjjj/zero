#include "LevelExperience.h"

#include <assert.h>
#include <inttypes.h>
#include <stdio.h>

#ifdef NDEBUG
#error Tests require assertions.
#endif

int main(int argc, char** argv) {
    assert(argc == 2);
    uint64_t value = 123;
    assert(level_experience_required(0, &value) == LEVEL_EXPERIENCE_INVALID_LEVEL);
    assert(value == 123);

    // 逐级有限差分提供独立参照，覆盖整个可表示区间。
    uint64_t expected          = 101;
    uint64_t difference        = 307;
    uint64_t second_difference = 212;
    for (uint64_t level = 1; level <= 2642212; ++level) {
        assert(level_experience_required(level, &value) == LEVEL_EXPERIENCE_SUCCESS);
        assert(value == expected);
        if (level < 2642212) {
            expected          += difference;
            difference        += second_difference;
            second_difference += 6;
        }
    }

    const uint64_t preserved        = value;
    const uint64_t invalid_levels[] = {2642213, 10000000, UINT64_MAX - 100, UINT64_MAX};
    for (size_t i = 0; i < sizeof(invalid_levels) / sizeof(invalid_levels[0]); ++i) {
        assert(level_experience_required(invalid_levels[i], &value) == LEVEL_EXPERIENCE_OVERFLOW);
        assert(value == preserved);
    }

    FILE* demo = fopen(argv[1], "w");
    assert(demo != NULL);
    fputs(
        "# 1–2000 级经验演示\n\n"
        "[算法说明](经验算法.md)\n\n"
        "共 2000 行。当级经验表示当前等级升至下一级的需求；累计经验表示到达当前等级的总需求。\n\n"
        "每日有效游玩 3 小时；100 级前经验效率按 180 天到达 100 级校准，之后效率按等级平方增长。"
        "时间估计不计死亡、经验道具、全局提升和其他特殊事件。\n\n"
        "| 当前等级 | 当级经验 | 累计经验 | 当级预计有效小时 | 当级预计天数 | 累计预计天数 |\n"
        "| ---: | ---: | ---: | ---: | ---: | ---: |\n",
        demo
    );

    const uint64_t total_at_100       = 57337500;
    uint64_t total                    = 0;
    uint64_t previous                 = 0;
    uint64_t cumulative_day_numerator = 0;
    for (uint64_t level = 1; level <= 2000; ++level) {
        assert(level_experience_required(level, &value) == LEVEL_EXPERIENCE_SUCCESS);
        assert(value > previous);
        previous                   = value;
        const uint64_t n           = level - 1;
        const uint64_t triangular  = n * (n + 1) / 2;
        const uint64_t exact_total = 100 * n * (n + 1) * (2 * n + 1) / 6 + triangular * triangular;
        assert(total == exact_total);
        if (level == 100) {
            assert(total == total_at_100);
            assert(cumulative_day_numerator == 180 * total_at_100);
        }
        if (level == 2000) {
            assert(total == UINT64_C(4262467700000));
        }

        const uint64_t day_numerator = level <= 100 ? value * 180 : UINT64_C(1800000) * (level + 100);
        const uint64_t exact_days    = level <= 100 ? total * 180 : (total_at_100 + UINT64_C(10000) * (level - 100) * (level + 299) / 2) * 180;
        assert(cumulative_day_numerator == exact_days);

        fprintf(demo, "| %" PRIu64 " | %" PRIu64 " | %" PRIu64 " |", level, value, total);
        const uint64_t numerators[] = {day_numerator * 3, day_numerator, cumulative_day_numerator};
        for (size_t column = 0; column < sizeof(numerators) / sizeof(numerators[0]); ++column) {
            const uint64_t scaled = (numerators[column] * UINT64_C(1000000) + total_at_100 / 2) / total_at_100;
            fprintf(demo, " %" PRIu64 ".%06" PRIu64 " |", scaled / UINT64_C(1000000), scaled % UINT64_C(1000000));
        }
        fputc('\n', demo);
        total                    += value;
        cumulative_day_numerator += day_numerator;
    }

    const int write_failed = ferror(demo);
    const int close_result = fclose(demo);
    assert(write_failed == 0);
    assert(close_result == 0);
    puts("PASS: full representable range, overflow boundaries, cumulative totals and time model; generated 2000 rows.");
    return 0;
}
