//
// Created by wuyongyu on 2020/5/20.
//

#include "cache_compile.h"
#include "misc/util.h"
#include "file/file.h"
#include <fstream>
#include "misc/hash.h"
#include <string>

using string = std::string;
namespace leetcode {
namespace jit_compiler {

bool startswith(const string& a, const string& b, uint start, bool equal = false, uint end = 0) {
    if (!end) end = a.size();
    if (b.size()+start > end) return false;
    if (equal && b.size()+start != end) return false;
    for (uint i=0; i<b.size(); i++)
        if (a[i+start] != b[i]) return false;
    return true;
}

bool endswith(const string& a, const string& b) {
    if (a.size() < b.size()) return false;
    return startswith(a, b, a.size()-b.size());
}

bool cache_compile(const string& cmd,
                   const string& cache_path,
                   const string& teaflow_path) {
    vector<string> input_names;
    map<string, vector<string >> extra;
    string output_name;
    find_names(cmd, input_names, output_name, extra);
    string output_cache_key;
    bool ran = false;
    output_cache_key = read_all(output_name + ".key");
    string  cd_cmd = cache_path.size() > 0 ?
                     "cd " + cache_path + " && " + cmd
                                           : cmd;
    // 让core 可以一直编译
    if (endswith(output_name, "leetcode_core.cpython-37m-darwin.so")) {
        output_cache_key = "";
    }

    LOGi << output_name << " " << output_cache_key.size();
    if (output_cache_key.size() == 0) {
        // std::cout << "Cache key of" << output_name << "not found.";
        // std::cout << "Run cmd:" << cmd;
        LOGi << cmd;
        system_with_check(cd_cmd.c_str());
        ran = true;
    }
    string cache_key = cmd;
    cache_key += "\n";
    unordered_set<string> processed;
    auto src_path = join(teaflow_path, "");
    // auto src_path = teaflow_path;


    const auto& extra_include = extra["I"];
    for (size_t i=0; i<input_names.size(); i++) {
        if (input_names[i] == "dynamic_lookup") {
            LOGi << input_names[i];
            continue;
        }
        if (processed.count(input_names[i]) != 0)
            continue;
        processed.insert(input_names[i]);
        auto src = read_all(input_names[i]);
        if (src.size() == 0 ) {
            std::cerr << "Source read failed:" << input_names[i] << std::endl;
        }
        ASSERT(src.size()) << "Source read failed:" << input_names[i];
        auto hash = S(hash64(src));
        vector<string> new_names;
        process(src, new_names);
        // 文件里面的include的文件
        for (auto& name : new_names) {
            string full_name;
            if (name.substr(0, 4) == "jit/" || name.substr(0, 4) == "gen/") {
                full_name = join(cache_path, name);
            } else if (name.size() && name[0]=='/') {
                full_name = name;
            } else {
                full_name = join(src_path, name);
            }
            if (!file_exist(full_name)) {
                bool found = 0;
                for (const auto& inc : extra_include) {
                    full_name = join(inc, name);
                    if (file_exist(full_name)) {
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    LOGe << "Include file" << name << "not found in" << extra_include
                              << "\nCommands:" << cmd;
                }
                ASSERT(found) << "Include file" << name << "not found in" << extra_include
                              >> "\nCommands:" << cmd;
                // LOGvvvv << "Include file found:" << full_name;
                // LOGi << "Include file found:" << full_name;
            }
//          input_names.push_back(full_name);
        }
        cache_key += "# ";
        cache_key += input_names[i];
        cache_key += ": ";
        cache_key += hash;
        cache_key += "\n";
    }
    if (output_cache_key.size() != 0 && output_cache_key != cache_key) {
        LOGvv  << "Cache key of" << output_name << " changed.";
        LOGvv << "Run cmd:" << cmd;
        system_with_check(cd_cmd.c_str());
        ran = true;
    }
    if (output_cache_key != cache_key) {
        LOGvv << "Prev cache key" << output_cache_key;
        LOGvv << "Write cache key" << output_name+".key:\n" << cache_key;
        write(output_name+".key", cache_key);
    }
    if (!ran) {
        LOGvv << "Command cached:" << cmd;
        // LOGvv << "Command cached:" << cmd;
    }

    return ran;
}
}  // end namesapce jit_compiler
}  // end namespace leetcode

#ifdef LEETCODE_TEST
using namespace::leetcode;
using namespace::leetcode::jit_compiler;
void test_cache_compile() {
  string cmd = "g++ main.cpp -o main.o";
  string cache_path = "/Users/wuyongyu/CLionProjects/teaflow";
  string teaflow_path = "/Users/wuyongyu/CLionProjects/teaflow";
//  CHECK(cache_compile(cmd, cache_path, teaflow_path))
//       << " cache compile cmd:"
//       << cmd;

}
#endif