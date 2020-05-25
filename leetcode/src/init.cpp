//
// Created by wuyongyu on 2020/5/24.
//

#ifdef HAS_CUDA

#include <cuda_runtime.h>
#include <helper_cuda.h>
#endif
#include <random>
#include "init.h"

namespace leetcode {
DEFINE_FLAG(vector<int>, cuda_archs, {}, "");
unique_ptr <std::default_random_engine> eng;
vector<set_seed_callback > callbacks;

int current_seed;

static void init_cuda_devices() {
#ifdef HAS_CUDA
int count;
cudaGetDeviceCount(&count);
for (int i =0; i< count; ++i) {
    cudaDevicceProp devprop;
    cudaGetDeviceProperties(&devprop, i);
    int number = devprop.major * 10 + devprop.minor;
    int found = 0;
    for(auto& v : FLAGS_cuda_archs) {
        if (v == number) {
            found = 1;
            break;
        }
    }
    if (!found) {
        FLAGS_cuda_archs.push_back(number);
    }
}
LOGi << "cuda_archs:" << FLAGS_cuda_archs;
#endif
}

void set_seed(int seed) {
    current_seed = seed;
    eng.reset(new std::default_random_engine(seed));
    for (auto cb : callbacks) {
        cb(seed);
    }
}

void init() {
    set_seed(time(0));

    init_cuda_devices();
}

void add_set_seed_callback(set_seed_callback callback) {
    callbacks.push_back(callback);
    callback(current_seed);
}

std::default_random_engine* get_random_engine() { return eng.get(); }
} // end namespace leetcode