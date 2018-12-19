#pragma once

#include <stdint.h>
#include <array>
#include <cstring>

class SeedRand {
   private:
    std::array<uint32_t, 4> mState;

   public:
    SeedRand(uint32_t seed);
    SeedRand(std::array<uint32_t, 4> state);

    uint32_t getU32();
};
