#include "SeedRand.h"

SeedRand::SeedRand(uint32_t seed = 0) {
    mSeed = seed;
    uint32_t priv_seed = seed;
    for (int i = 0; i < 4; i++) {
        mState[i] =
            (uint32_t)(0x6C078965 * (priv_seed ^ (priv_seed >> 30)) + i + 1);
        priv_seed = mState[i];
    }
}

SeedRand::SeedRand(uint32_t seed, std::array<uint32_t, 4> state) {
    mSeed = seed;
    mState = state;
}

uint32_t SeedRand::getU32() {
    uint32_t a = mState[0] ^ (mState[0] << 11);
    uint32_t b = mState[3];
    uint32_t c = a ^ (a >> 8) ^ b ^ (b >> 19);
    mState[0] = mState[1];
    mState[1] = mState[2];
    mState[2] = mState[3];
    mState[3] = c;
    return c;
}
