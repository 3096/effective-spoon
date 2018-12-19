#include "SeedRand.h"

SeedRand::SeedRand(uint32_t seed = 0) {
    for (int i = 0; i < 4; i++) {
        mState[i] = (uint32_t)(0x6C078965 * (seed ^ (seed >> 30)) + i + 1);
        seed = mState[i];
    }
}

SeedRand::SeedRand(std::array<uint32_t, 4> state) {
    mState = state;
}

uint32_t SeedRand::getU32() {
    uint32_t a = mState[0] ^ (mState[0] << 11);
    uint32_t b = mState[3];
    uint32_t c = a ^ (a >> 8) ^ b ^ (b >> 19);
    for (int i = 0; i < 3; i++)
        mState[i] = mState[i + 1];
    mState[3] = c;
    return c;
}
