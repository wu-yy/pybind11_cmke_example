//
// Created by wuyongyu on 2019/4/3.
//
#include<iostream>
#include<cstring>
#include<map>
#include <vector>
#include <set>
#include <algorithm>
#include<limits.h>
#include<stack>
#include <queue>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>

// 删除字符串里面的空格
void deleteAllMark(string &s, const string &mark)
{
    size_t nSize = mark.size();
    while(1)
    {
        size_t pos = s.find(mark);    //  尤其是这里
        if(pos == string::npos)
        {
            return;
        }

        s.erase(pos, nSize);
    }
}

// 对于一行字符串，输出里面的数字  ，字符串类似： 12 34 56
int getInt(char *buf)
{
    char *p = strtok(buf, " ");
    int res = 0;
    while(p)
    {
        atoi(p);
        res++;
        p = strtok(NULL, " ");
    }
    return res;
}

// 判断输入的是不是空行
void get_in(){
    char s[10000];
    vector <string> strings;

    while (gets(s)) {
        if (s[0] == '\0')break;//gets函数如果读取了换行符会将其自动转换成字符串结束符'\0'
        cout<<"s:"<<s<<endl;
    }
}

int main() {

//    findA();
    int n;
    cin >> n;
    getchar();
    char s[10000];
    vector <vector<int>> nums;

    char s[100];
    vector <string> strings;

    while (gets(s)) {
        if (s[0] == '\0')break;//gets函数如果读取了换行符会将其自动转换成字符串结束符'\0'

        vector<int> tem_vec;
//        cout<<"s:"<<s<<endl;
        char *p = strtok(s, ",");
        while (p) {
            int ip = atoi(p);

            p = strtok(NULL, ",");
            tem_vec.push_back(ip);
//            cout<<ip<<endl;
        }
        nums.push_back(tem_vec);
    }
    return 0;
}