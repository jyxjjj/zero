#ifndef ZERO_LEVEL_EXPERIENCE_H
#define ZERO_LEVEL_EXPERIENCE_H

#include <stdint.h>

typedef enum {
    LEVEL_EXPERIENCE_SUCCESS,
    LEVEL_EXPERIENCE_INVALID_LEVEL,
    LEVEL_EXPERIENCE_OVERFLOW
} LevelExperienceResult;

// experience 必须指向有效输出；仅成功时写入当前等级升至下一级所需经验。
LevelExperienceResult level_experience_required(uint64_t level, uint64_t* experience);

#endif
