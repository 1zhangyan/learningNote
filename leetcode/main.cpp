#include "bits/stdc++.h"

using namespace std;

/**
 * 946 验证栈序列
 * 模拟法验证
 */
class ValidateStackSequences {
    public:
    bool validateStackSequences(vector<int>& pushed, vector<int>& popped) {
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
    1。两次遍历 顺序逆序，取最大值
    2。单次遍历，但是更新前序
**/

class Candy {
    public: int candy(vector<int>& ratings) {
        vector<int> pre(ratings.size());
        vector<int> last(ratings.size());
        for(int i = 0; i < ratings.size(); i++) {
            if (i == 0) {
                pre [i] = 1;
            } else {
                if (ratings[i] > ratings[i-1]) {
                    pre[i] = pre[i-1] + 1;
                } else {
                    pre[i] = 1;
                }
            }
        }
        for(int i = ratings.size()-1; i >=0 ; i--) {
            if (i == ratings.size()-1) {
                last[i] = 1;
            } else {
                if (ratings[i] > ratings[i+1]) {
                    last[i] = last[i+1] + 1;
                } else {
                    last[i] = 1;
                }
            }
        }
        int total = 0;
        for(int i = 0; i < ratings.size(); i++) {
            total+= max(pre[i], last[i]);
        }
        return total;
    }
};

/**
 * 166. 分数到小数
 */
class FractionToDecimal {
public:
    string fractionToDecimal(int numerator, int denominator) {
        string result = "";
        if (numerator == 0) {
            return "0";
        }
        if (numerator < 0 || denominator < 0) {
            if (!(numerator < 0 && denominator < 0)) {
                result+="-";
            }
        }
        numerator = abs(numerator);
        denominator = abs(denominator);
        long cur = numerator;
        unordered_map<int, int> curSet;
        if (numerator > denominator) {
            result += to_string(numerator/denominator);
            cur = numerator%denominator;
            if (cur == 0) {
                return result;
            }
        }
        if(result == "") {
            result = "0";
        }
        result += ".";
        while(cur != 0) {
            cur = cur * 10;
            if (curSet.find(cur) != curSet.end()) {
                result.insert(curSet[cur],"(");
                result += ")";
                return result;
            }
            curSet[cur] = result.size();
            int qoutient = cur/denominator;
            int reminder = cur%denominator;

            result += to_string(qoutient);
            cur = reminder;
        }
        return result;
    }

};


int main (){
    FractionToDecimal fractionToDecimal;
    cout << fractionToDecimal.fractionToDecimal(-1, -2147483648);
}