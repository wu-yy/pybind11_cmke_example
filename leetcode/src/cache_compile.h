
//
// Created by wuyongyu on 2020/5/20.
//

#pragma once
#include "common.h"
namespace leetcode {
namespace jit_compiler {
bool cache_compile(const string& cmd,
                   const string& cache_path="",
                   const string& teaflow_path="");
} // end namespace jit_compiler
} // end  namespace leetcode
// #define TEAFLOW_TEST
#ifdef LEETCODE_TEST
using namespace::leetcode;
using namespace::leetcode::jit_compiler;
void test_cache_compile();
#endif
