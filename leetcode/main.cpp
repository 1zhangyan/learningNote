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
 * 166 分数到小数
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

/**
 * 54 螺旋顺序
 * 层次遍历，递归进行，需要注意单行单列的情况，不能遍历两次
 */
class SpiralOrder {
    vector<int> re;
    void dfsSpiral(vector<vector<int>>& matrix, int x1, int y1, int x2, int y2) {
        if (x1 > x2 || y1 > y2 || x1 < 0 || x2 >= matrix[0].size() || y1 < 0 || y2 >= matrix.size()) {
            return;
        }
        for (int i = x1; i <= x2; i++) {
            re.push_back(matrix[y1][i]);
        }
        for(int i = y1+1; i <=y2; i++) {
            re.push_back(matrix[i][x2]);
        }
        if (y1 != y2) {
            for(int i = x2 -1; i >= x1; i--) {
                re.push_back(matrix[y2][i]);
            }
        }
        if (x1 != x2) {
            for(int i = y2-1; i > y1; i--) {
                re.push_back(matrix[i][x1]);
            }
        }
        dfsSpiral(matrix,x1+1,y1+1,x2-1,y2-1);
    }
    vector<int> spiralOrder(vector<vector<int>>& matrix) {
        dfsSpiral(matrix, 0,0,matrix[0].size()-1, matrix.size()-1);
        return re;
    }
};


/**
 * 101 对称二叉树
 * 递归判断 p 往左走 q就往右走
 */
class IsSymmetric {
public:
    bool check(TreeNode* p, TreeNode* q) {
        if (p == nullptr && q == nullptr) {
            return true;
        }
        if (p == nullptr && q != nullptr) {
            return false;
        }
        if (q == nullptr && p != nullptr) {
            return false;
        }
        if( q->val != p->val ){
            return false;
        }
        return check(p->left, q->right) && check(p->right , q->left);
    }
    bool isSymmetric(TreeNode* root) {
        return check(root, root);
    }
};


/**
 * 129 求根节点到叶节点之和
 * 深度优先遍历
 * 回退
 */
class SumNumbers {
public:
    long result;
    void dfsSearch(TreeNode* root, string tmp) {
        tmp += ('0' + root->val);
        if (root -> left) {
            dfsSearch(root->left, tmp);
        }
        if (root -> right) {
            dfsSearch(root->right, tmp);
        }
        if (root->right == nullptr && root->left == nullptr) {
            result += stoi(tmp);
        }
        tmp.pop_back();
    }
    int sumNumbers(TreeNode* root) {
        if (root == nullptr) {
            return 0;
        }
        dfsSearch(root, "");
        return result;
    }
};


/**
 * 98 验证二叉搜索树
 * 中序遍历
 */
class IsValidBST {
public:
    bool isValidBST(TreeNode* root) {
        long pre = (long long )INT_MIN-1;
        stack<TreeNode*> s;
        TreeNode * p = root;
        while(!s.empty() || p) {
            while (p) {
                s.push(p);
                p = p->left;
            }
            p = s.top();
            if (p->val <= pre) {
                return false;
            }
            pre = p->val;
            s.pop();
            p = p->right;
        }
        return true;
    }
};

/**
 * 139 单词拆分
 * 简单粗暴 但是会超时
 */
class WordBreak {
public:
    bool re = false;
    unordered_set<string> set;

    void dfsFind(int index, string s) {
        if ( index == s.size()) {
            re = true;
            return;
        }
        string tmp = "";
        for(int i = index; i < s.size(); i++) {
            tmp.push_back(s[i]);
            if (set.find(tmp) != set.end()) {
                dfsFind(i+1, s);
            }
        }
    }
    bool wordBreak(string s, vector<string>& wordDict) {
        for (auto word : wordDict) {
            set.insert(word);
        }
        dfsFind(0, s);
        return re;
    }
};

/**
 * 151 颠倒字符串中的单词
 * 先分割后颠倒
 */
class ReverseWords {
    string reverseWords(string s) {
        string  re = "";
        vector<string> words;
        int i = 0;
        while(i < s.size()) {
            while(i < s.size() && s[i] == ' ') {
                i++;
            }
            string tmp = "";
            bool  flag = false;
            while(i < s.size() && s[i] != ' ') {
                flag = true;
                tmp.push_back(s[i]);
                i++;
            }
            if (flag) {
                words.push_back(tmp);
            }
        }
        if (words.empty()) {
            return re;
        }
        reverse(words.begin(),words.end());
        for (auto word : words) {
            re += word;
            re.push_back(' ');
        }
        re.pop_back();
        return re;
    }
};

/**
 * 402 移掉 K 位数字
 * 单调栈，从左到右，找单调递增的最大的数
 */
class RemoveKdigits {
    public:
        string removeKdigits(string num, int k) {
         if (k == num.size()) {
             return "0";
         }
         string re = "";
         stack<int> s;
         set<int> erasePos;
         s.push(0);
         for(int i = 1; i < num.size(); i++) {
             while (!s.empty() && erasePos.size() < k && num[s.top()] > num[i]) {
                 erasePos.insert(s.top());
                 s.pop();
             }
             s.push(i);
         }
         while (erasePos.size() < k) {
             erasePos.insert(s.top());
             s.pop();
         }
         for(int i = 0; i < num.size(); i++) {
             if (erasePos.find(i) == erasePos.end()) {
                 if (re == "" && num[i] == '0'){
                     continue;
                 }
                 re.push_back(num[i]);
             }
         }
         if (re == "") return "0";
         return re;
     }
 };

/**
 * 79 单词搜索
 * 部分用例超时
 */
class dfsWordSearch {
    bool dfs(vector<vector<char>>& board,vector<vector<bool>> flag,string word,int x, int y, int index) {
        if (x >= board.size() || y >= board[0].size() || x < 0|| y< 0){
            return false;
        }
        if (board[x][y] != word[index] || flag[x][y] == true) {
            return false;
        }
        if (index == word.size() -1) {
            return true;
        }
        flag[x][y] = true;
        bool re = dfs(board,flag,word,x+1,y,index+1)||dfs(board,flag,word,x-1,y,index+1)||dfs(board,flag,word,x,y+1,index+1)||dfs(board,flag,word,x,y-1,index+1);
        flag[x][y] = false;
        return re;
    }
    bool exist(vector<vector<char>>& board, string word) {
        vector<vector<bool>> flag ( board.size(),vector<bool>(board[0].size(), false));
        for (int i = 0 ; i < board.size(); i++) {
            for (int j = 0 ; j < board[0].size() ; j ++) {
                if (dfs(board,flag,word,i,j,0)) return true;
            }
        }
        return false;
    }
};

/**
 * 209 长度最小的子数组
 */
int minSubArrayLen(int target, vector<int>& nums) {
   int minLen = nums.size() + 1;
   int left = 0;
   int right = 0;
   int total = 0;
   for (int i = 0 ; i < nums.size(); i++) {
        right = i;
        total += nums[right];
        if (total >= target) {
            while (total >= target && left < right) {
                total-=nums[left];
                left++;
            }
            if (total < target) {
                left --;
                total+=nums[left];
            }
            minLen = min(minLen, right-left+1);
        }
   }
    return minLen > nums.size() ? 0: minLen;
}
/**
 * 121 买卖股票的最佳时机
 */
class MaxProfit {
public:
    int maxProfit(vector<int>& prices) {
        if (prices.size() < 2) {
            return 0;
        }
        int maxProf = prices[1] - prices[0];
        int minPrice = min(prices[0], prices[1]);
        for (int i = 2; i < prices.size(); i++) {
            maxProf = max(maxProf, prices[i] - minPrice);
            if (prices[i] < minPrice) {
                minPrice = prices[i];
            }
        }
        return maxProf > 0? maxProf:0;
    }
};

/**
 * 162 寻找峰值
 * 二分法，只要去单调递增的地方就可
 */
int findPeakElement(vector<int>& nums) {
    int left = 0;
    int right = nums.size() - 1;
    if (nums.size() == 1) {
        return 0;
    }
    while(left <= right) {
        int mid = (left + right) / 2;
        if (mid <= 0) {
            return nums[0] > nums[1] ? 0 : 1;
        }
        if (mid >= nums.size()-1) {
            return nums[nums.size()-1] > nums[nums.size()-2] ? nums.size()-1 : nums.size()-2;
        }
        if (nums[mid] > nums[mid + 1] && nums[mid] > nums[mid - 1]) {
            return mid;
        } else if (nums[mid] < nums[mid + 1]) {
            left = mid+1;
        } else {
            right = mid-1;
        }
    }
    return left;
}

/**
 * offer 04 二维数组的查找
 * 从右上方开始
 */
bool findNumberIn2DArray(vector<vector<int>>& matrix, int target) {
    if (matrix.size() < 1 || matrix[0].size() < 1) {
        return false;
    }
    int row = 0;
    int col = matrix[0].size() -1;
    while(row >=0 && col < matrix[0].size() && col >=0 && row < matrix.size()) {
        if (target == matrix[row][col]) {
            return true;
        }
        if (matrix[row][col] > target) {
            col --;
        } else {
            row ++;
        }
    }
    return false;
}

/**
 *  165 比较版本号
 *  循环比较，如果不存在则把数字置为 0
 *  比较每次取的数字大小 ，统一模型
 */
int compareVersion(string version1, string version2) {
    int i = 0;
    int j = 0;
    while(i < version1.size() || j < version2.size()) { //need attention!
        long x = 0;
        while(i < version1.size() && version1[i] != '.') {
            x = x * 10 + version1[i] - '0';
            i++;
        }
        i++;
        long y = 0;
        while(j < version2.size() && version2[j] != '.') {
            y = y * 10 + version2[j] - '0';
            j++;
        }
        j++;
        if (x != y) {
            return x>y?1:-1;
        }
    }
    return 0;
}

int main () {
    cout<<compareVersion("1.2147483647"
                   ,"1.2147483647");
}