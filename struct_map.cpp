#include <deque>
#include <iostream>
#include <cmath>
#include <fstream>
#include <algorithm>
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
#include <vector>
using namespace std;
bool compare(const int &a, const int &b) {
    return a < b;
}
int main()
{
    int n;
    cin >>n;
    int M = 0;
    int  sum =0;
    std::vector<int> All;
    for (int i =0; i < n;i++) {
        int x, y;
        cin >> x >> y;
        for (int j =0; j< x; j++) {
            All.push_back(y);
        }
        M += x;
        sum += x*y;
    }
//    for (auto& p : All) {
//        std::cout << " " << p;
//    }
//    std::cout << std::endl;
    std::sort(All.begin(), All.end(),compare);
    for (int i =0 ;i < M/2; i++) {
        if(i==0) {
            sum = All[i] + All[M-i-1];
        } else if (sum < All[i] + All[M-i-1]) {
            sum = All[i] + All[M-i-1];
        }
    }
    std::cout << sum;

    return 0;
}