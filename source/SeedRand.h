#pragma once

#include <stdint.h>
#include <array>
#include <cstring>

class SeedRand {
   private:
    uint32_t mSeed;
    std::array<uint32_t, 4> mState;

   public:
    SeedRand(uint32_t seed);
    SeedRand(uint32_t seed, std::array<uint32_t, 4> state);

    uint32_t getU32();
};
