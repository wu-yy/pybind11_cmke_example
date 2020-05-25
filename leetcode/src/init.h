//
// Created by wuyongyu on 2020/5/24.
//

#pragma once

#include <random>
#include "common.h"

namespace leetcode {
    typedef void (*set_seed_callback)(int);

    void init();

/*init.h*/
// @pyjt(set_seed, seed)
void set_seed(int seed);

void add_set_seed_callback(set_seed_callback callback);

extern "C"
std::default_random_engine* get_random_engine();
}