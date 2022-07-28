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

/**
 * 138 带随机指针的链表复制
 * 先复制 在拆分
 */
class CopyRandomList {
public:
    class Node {
    public:
        int val;
        Node* next;
        Node* random;

        Node(int _val) {
            val = _val;
            next = NULL;
            random = NULL;
        }
    };

    Node* copyRandomList(Node* head) {
        Node * cur = head;
        while(cur) {
            Node* tmp = new Node(cur->val);
            tmp->next = cur->next;
            cur->next = tmp;
            cur = tmp->next;
        }
        cur = head;
        while(cur) {
            if (cur->random) {
                cur->next->random = cur->random->next;
            } else {
                cur->next->random = nullptr;
            }
            cur = cur->next->next;
        }
        Node * dumpHead = new Node(0);
        Node * p = dumpHead;
        cur = head;
        while(cur) {
            p->next=cur->next;
            cur->next = cur->next->next;
            cur = cur->next;
            p = p->next;
        }
        return dumpHead->next;
    }
};

/**
 *  114 二叉树展开为链表
 *  找后继节点。左节点置为空，右节点置原来的左节点。原来的右节点找它的前驱，将它赋给前驱的右节点。
 */
class Flatten {
public:
    void flatten(TreeNode* root) {
        flattenOneNode(root);
    }
    void flattenOneNode(TreeNode *root) {
        if (root == nullptr) {
            return;
        }
        if (root->left == nullptr) {
            flattenOneNode(root->right);//need to deal
            return;
        }
        TreeNode * cur = root->left;
        while(cur->right) {
            cur = cur->right;
        }
        cur->right = root->right;
        root->right = root->left;
        root->left = nullptr; // need to set null!
        flattenOneNode(root->right);
    }
};

/**
 * 93 复原 IP 地址
 **/
class RestoreIpAddresses {
public:
    // 0.1.2.201
    vector<string> results;
    string regionS;

    void dfsFind(int index, string curIp, int segment) {
        if (segment == 4) {
            if (index >= regionS.size()){
                results.push_back(curIp);
            }
            return;
        }
        if (index >= regionS.size()) {
            return;
        }

        if (regionS[index] == '0') {
            curIp.push_back('0');
            if (segment != 3) {
                curIp.push_back('.');
            }
            dfsFind(index+1, curIp, segment+1);
            return;
        }
        int curNum = 0;
        for (int i = index; i < regionS.size(); i++) {
            curNum = curNum*10 + regionS[i] - '0';
            if (curNum > 0 && curNum < 256) {
                string tmpIp = curIp;
                tmpIp += to_string(curNum);
                if (segment != 3) {
                    tmpIp.push_back('.');
                }
                dfsFind(i+1,tmpIp,segment+1);
            } else {
                break;
            }
        }
    }

    vector<string> restoreIpAddresses(string s) {
        regionS = s;
        string curIp = "";
        dfsFind(0, curIp, 0);
        return results;
    }
};




int main () {
    RestoreIpAddresses res;
    for (auto s : res.restoreIpAddresses("101023")) {
        cout<<s<<endl;
    }
}