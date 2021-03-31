# Some Common Code for Leetcode
## 排序
### 堆排序Heapsort
```C++
void Adjust(int arr[] , int start , int end)
{
	int dad = start;
	int son = 2*dad+1;
	while(son <= end)
	{
		if(son+1 <= end && arr[son+1]>arr[son])
			son ++;
		if(arr[son]<=arr[dad])
			return;
		else(
			swap(arr[son],arr[dad]);
			dad = son;
			son = 2*dad + 1;
		)
	}
}

void heap_sort(int arr[] ,int n)
{
	//这是个建堆的过程，先建一个大顶堆
	for(int i = n/2 - 1 ; i >= 0 ; i--)
		Adjust(arr,i,n-1);
        //每次交换后不需要再次建堆，只需要对当前节点往下继续调整就可以
	for(int i = n -1 ; i > 0 ; i --)
	{
		swap(arr[0], arr[i]);
		Adjust(arr,0,i-1);
	}
	
}

```
### QickSort 快排
```C++
QuickSort(int arr[] , int left ,int right)
{
	int i = left;
	int j = right;
	if(i >= j )
		return ;
	int k = arr[left];//取下标的行为必然要放在异常判断的后面
	while(i < j)
	{
		while(i<j&&arr[j]>=key)
			j--;
		while(i<j&&arr[i]<=key)
			i++;
		if(i<j)
		swap(arr[i],arr[j]);
	}
	swap(arr[i],arr[left]);
	QuickSort(arr,left,i-1);
	QuickSort(arr,i+1,right);
}
```
## 归并排序


```C++
#include <iostream>
#include <vector>

using namespace std;

void merge(vector<int>& vec, int left, int mid, int right) {
	vector<int> helper(right - left + 1, 0);
	int p1 = left;
	int p2 = mid + 1;
	int index = 0;
	while (p1 <= mid && p2 <= right) {
		helper[index++] = (vec[p1] < vec[p2] ? vec[p1++] : vec[p2++]);
	}
	while (p1 <= mid) {
		helper[index++] = vec[p1++];
	}
	while (p2 <= right) {
		helper[index++] = vec[p2++];
	}

	for (int i = 0; i < helper.size(); i++) {
		vec[left + i] = helper[i];
	}
}

void mergeSort(vector<int>& vec, int left, int right) {
	if (left >= right) {
		return;
	}
	int mid = left + ((right - left) >> 1);

	mergeSort(vec, left, mid);
	mergeSort(vec, mid + 1, right);
	merge(vec, left, mid, right);
}

void mergeSort(vector<int>& vec) {
	if (vec.size() < 2) {
		return;
	}
	mergeSort(vec, 0, vec.size() - 1);
}

int main() {
	
	vector<int> arr = { 1,4,32,23,4,6,7,1,3,9,7,0,-1 };
	mergeSort(arr);
	for (int i = 0; i < arr.size(); i++) {
		cout << arr[i] << endl;
	}

	return 0;
}
```
## 链表


## 树
### 二叉树遍历 
```C++
void Travel(TreeNode * root)
{
	if(root == nullptr)
		return;
	stack<TreeNode*> s;
	TreeNode*p = root;
	while(s.empty()|| p != nullptr)
	{
		if(p)
		{
			//visit(p);//If you want to travel in a pre-order then put the row here
			s.push(p);
			p = p->left;
		}
		//visit(p);//If you want to travel in a in-order then put the row here
		p = s.top();
		s.pop();
		p = p->right;
	}
}
```
