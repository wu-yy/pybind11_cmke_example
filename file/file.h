//
// Created by wuyongyu on 2020/5/20.
//

//
// Created by wuyongyu on 2020-04-09.
//

#pragma once
#include "common.h"
#include "types.h"
#include <string>
#include <vector>
#include <map>

namespace leetcode {
string read_all(const string& fname);
void write(const string& fname, const string& src);
bool file_exist(const string& fname);
string join(string a, string b);
size_t skip_comments(const string& src, size_t i);
void find_names(string cmd, vector<string>& input_names, string& output_name, map<string,vector<string>>& extra);
void expect_error2(std::function<void()> func);
void process(string src, vector<string>& input_names);
} // end  namespace leetcode