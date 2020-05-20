
//
// Created by wuyongyu on 2020/5/20.
//

#pragma once
#include "common.h"

namespace leetcode {

uint constexpr const_hash(const char *input) {
    return *input ?
           static_cast<uint>(*input) + 55 * const_hash(input + 1) :
           0;
}

/* simple hash function */
// @pyjt(hash)
inline uint hash(const char* input) {
    uint v=0, mul=1;
    while (*input) {
        v += mul * (uint)*input;
        mul *= 55;
        input++;
    }
    return v;
}


inline uint64 hash64(const string& input) {
    uint64 v=0, mul=1;
    for (char c : input) {
        v += mul * (uint64)c;
        mul *= 257;
    }
    return v;
}

} // end namespace leetcode
