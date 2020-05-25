//
// Created by wuyongyu on 2020/5/21.
//

#pragma once
#include <iostream>
#include <map>
#include <tuple>
#include <sstream>
#include <vector>

template<class T>
T get_from_env(const char *name, const T &_default);

template <>
std::vector<int> get_from_env(const char* name, const std::vector<int>& _default);
// 特别化
template<>
std::string get_from_env(const char *name, const std::string &_default);


#define DECLARE_FLAG(type, name) \
extern "C" type FLAGS_ ## name; \
extern "C" std::string doc_FLAGS_ ## name; \
extern "C" void set_FLAGS_ ##name (const type& t);


#define TOS_(x) #x
#define TOS(x) TOS_(x)

// #define TEA_JIT
#ifdef TEA_JIT

#define DEFINE_FLAG(type, name, default, doc) \
DECLARE_FLAG(type, name)

#define DEFINE_FLAG_WITH_SETTER(type, name, default, doc, setter) \
    DECLARE_FLAG(type, name)
#else

#define DEFINE_FLAG(type, name, default, doc) \
    DECLARE_FLAG(type, name); \
    type FLAGS_ ##name; \
    std::string doc_FLAGS_ ## name = doc; \
    void set_FLAGS_ ## name (const type& value) { \
      FLAGS_ ## name = value; \
    } \
    void init_FLAGS_ ## name (const type& value) { \
      FLAGS_ ## name = value; \
      if (getenv(TOS(FLAGS_ ## name))) { } \
    } \
    int caller_FLAGS_ ## name = (init_FLAGS_ ## name(get_from_env<type>(TOS(FLAGS_ ## name), default)), 0);

#define DEFINE_FLAG_WITH_SETTER(type, name, default, doc) \
    DECLARE_FLAG(type, name); \
    type FLAGS_ ## name; \
    std::string doc_FLAGS_ ## name = doc; \
    void setter_FLAGS_ ## name (type value); \
    void set_FLAGS_ ## name (const type& value) { \
        setter_FLAGS_ ## name (value); \
        FLAGS_ ## name = value; \
    }; \
    void init_FLAGS_ ## name (const type& value) { \
        setter_FLAGS_ ## name (value); \
        FLAGS_ ## name = value; \
        if (getenv(TOS(FLAGS_ ## name))) {} \
    }; \
    int caller_FLAGS_ ## name = (init_FLAGS_ ## name (get_from_env<type>(TOS(FLAGS_ ## name), default)), 0);
#endif



