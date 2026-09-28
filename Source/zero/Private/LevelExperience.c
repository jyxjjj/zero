#include "LevelExperience.h"

LevelExperienceResult level_experience_required(uint64_t level, uint64_t* experience) {
    if (level == 0) {
        return LEVEL_EXPERIENCE_INVALID_LEVEL;
    }

    if (level > UINT64_MAX - 100 || level > UINT64_MAX / level) {
        return LEVEL_EXPERIENCE_OVERFLOW;
    }

    const uint64_t square = level * level;
    const uint64_t factor = level + 100;
    if (square > UINT64_MAX / factor) {
        return LEVEL_EXPERIENCE_OVERFLOW;
    }

    *experience = square * factor;
    return LEVEL_EXPERIENCE_SUCCESS;
}
