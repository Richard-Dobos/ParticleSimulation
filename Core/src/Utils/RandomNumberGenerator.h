#pragma once

#include <iostream>

namespace Core::Utils
{
    inline uint64_t x = 123456789, y = 362436069, z = 521288629;

    static uint64_t xorshf96()
    {          
        uint64_t t;
        x ^= x << 16;
        x ^= x >> 5;
        x ^= x << 1;

        t = x;
        x = y;
        y = z;
        z = t ^ x ^ y;

        return z;
    }
}