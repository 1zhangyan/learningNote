#include "bits/stdc++.h"

using namespace std;

/**
 * 946 验证栈序列
 * 模拟法验证
 */
static class ValidateStackSequences {
    public:
    static bool validateStackSequences(vector<int>& pushed, vector<int>& popped) {
        stack<int> s;
        int curPop = 0;
        for(int pushElement : pushed) {
            s.push(pushElement);
            while(!s.empty() && curPop <popped.size() && s.top() == popped[curPop]) {
                s.pop();
                curPop ++;
            }
        }
        if (!s.empty()) {
            return false;
        }
        if (curPop < popped.size()) {
            return false;
        }
        return true;
    }
};

/** 135 分发糖果

**/

static class Candy {
    static int candy(vector<int>& ratings) {

    }
};


int main (){

}