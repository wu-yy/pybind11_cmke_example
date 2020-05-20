//
// Created by wuyongyu on 2020/5/20.
//

#include "file/file.h"
#include <fstream>
#include <streambuf>
#include "misc/hash.h"
#include "types.h"
#include "log.h"
#include "common.h"

namespace leetcode {
string read_all(const string& fname) {
    std::ifstream ifs(fname);
    if (ifs)
        return string((std::istreambuf_iterator<char>(ifs)),
                      (std::istreambuf_iterator<char>()));
    return "";
}

void write(const string& fname, const string& src) {
    std::ofstream(fname) << src;
}

bool file_exist(const string& fname) {
    std::ifstream f(fname);
    return f.good();
}

string join(string a, string b) {
    const char sep = '/';
    if (!b.empty() && b.front() == sep) return b;
    a.reserve(a.size() + b.size() + 1);
    if (!a.empty() && a.back() != sep) a += sep;
    a += b;
    return a;
}

// 解析 cmd, 获取编译指令的输入文件
// 输出文件
// 编译命令
void find_names(string cmd, vector<string>& input_names, string& output_name, map<string,vector<string>>& extra) {
    size_t i=0;
    while (i<cmd.size() && cmd[i] != ' ') i++;
    CHECK(i<cmd.size());
    // find space not in str
    auto pass = [&](size_t& j) {
        while (j<cmd.size()) {
            if (cmd[j]=='\'') {
                j++;
                while (j<cmd.size() && cmd[j]!='\'') j++;
                ASSERT(j<cmd.size());
                j++;
                continue;
            }
            while (j<cmd.size() && cmd[j]!=' ' && cmd[j]!='\'') j++;
            if (j<cmd.size()) {
                if (cmd[j]==' ') break;
                if (cmd[j]=='\'') continue;
            }
        }
    };
    // remove "'"
    auto substr = [&](size_t i, size_t j) -> string {
        string s;
        for (size_t k=i; k<j; k++)
            if (cmd[k]!='\'') s += cmd[k];
        return s;
    };
    while (i<cmd.size()) {
        if (cmd[i] == ' ') {
            i++;
            continue;
        }
        if (cmd[i] == '-') {
            if (i+2<cmd.size() && cmd[i+1]=='o' && cmd[i+2]==' ') {
                auto j=i+3;
                while (j<cmd.size() && cmd[j] == ' ') j++;
                CHECK(j<cmd.size());
                auto k=j;
                pass(k);
                CHECK(j<k && output_name.size()==0);
                // -o xxx
                // i  j  k
                output_name = substr(j, k);
                i = k;
                continue;
            } else if (i+2<cmd.size() && cmd[i+1]=='I') {
                // -Ixxx -I'xxx' -I xxx
                size_t j=i+2;
                while (j<cmd.size() && cmd[j]==' ') j++;
                size_t k=j;
                pass(k);
                CHECK(j<k);
                auto inc = substr(j, k);
                // find include
                i = k;
                extra["I"].push_back(inc);
                continue;
            } else if (i+2<cmd.size() && cmd[i+1]=='x') {
                // option with space and arg
                size_t j=i+2;
                while (j<cmd.size() && cmd[j]==' ') j++;
                size_t k=j;
                pass(k);
                i = k;
                continue;
            } else {
                pass(i);
                continue;
            }
        }
        auto j=i;
        pass(j);
        input_names.push_back(substr(i, j));
        i = j;
    }
    std::cout  << "output_name: " << output_name
               << " input_names: " << input_names << "\n" << cmd;
//    CHECK(output_name.size() && input_names.size())
//            << "output_name: " << output_name
//            << " input_names: " << input_names << "\n" << cmd;
}

size_t skip_comments(const string& src, size_t i) {
    if (src[i] == '/' && (i+1<src.size() && src[i+1] == '/')) {
        size_t j=i+1;
        while (j<src.size() && src[j] != '\n') j++;
        if (j<src.size()) j++;
        return j;
    } else
    if (src[i] == '/' && (i+1<src.size() && src[i+1] == '*')) {
        size_t j=i+1;
        while (j<src.size() && !(src[j] == '/' && src[j-1] == '*')) j++;
        if (j<src.size()) j++;
        return j;
    }
    return i;
}

// 解析文件行，获取include文件
void process(string src, vector<string>& input_names) {
    for (size_t i=0; i<src.size(); i++) {
        i = skip_comments(src, i);
        if (i>=src.size()) break;
        if (src[i] == '#') {
            // #include "a.h"
            // i       jk    l
            auto j=i+1;
            while (j<src.size() && src[j] != ' ') j++;
            if (j>=src.size()) return;
            auto k=j+1;
            while (k<src.size() && src[k] == ' ') k++;
            if (k>=src.size()) return;
            auto l=k+1;
            while (l<src.size() && (src[l] != ' ' && src[l] != '\n')) l++;
            if (src[k] == '"' && src[l-1] == '"' && j-i==8 && src.substr(i,j-i) == "#include") {
                auto inc = src.substr(k+1, l-k-2);
                if (inc != "test.h") {
                     std::cerr << "Found include" << inc;
                    input_names.push_back(inc);
                }
            }
            i=l;
        }
    }
}
}  // end namespace leetcode