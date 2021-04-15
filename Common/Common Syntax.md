# Some Syntax

Reference : http://www.cplusplus.com/reference

##  'algorithm'   

## headfile  
#include<bits/stdc++.h>
### sort  reverse
```C++
int A[];
vector<int> V;
sort ( A , A+n );
sort ( V.begin() , V.end() );  

reverse ( V.begin()+ i + 1 , V.end() )
```
排序按照从小到大的顺序，需要调整顺序的，交换参数次序即可   
Reference: https://www.runoob.com/w3cnote/cpp-vector-container-analysis.html

===========================  
## 'vector'  
```C++
#include <vector>  
int a=0;  

vector<int> V;             // initialization  
V.push_back(a);            //push an element at the end of the vector  
V.pop_back();              //delete the last element of the vector
V.empty();                 //see if the vector is empty
V.size();                  //return the size of the vector

int b = V.front();
int c = V.back();

//get access to vector element through iterator
vector<int>::iterator it;   
for(it = V.begin(); it!= V.end(); it++)
{
    cout<<*it<<endl;
}
//get access to vector element through iterator

vector<vector<int>> two_demension_nums;
```

需要注意的是vector.end() 指向vector的末尾后一位，直接访问vector.end越界了，而且，vector.push_back()分配的地址不是vector.end();  
Reference https://www.runoob.com/w3cnote/cpp-vector-container-analysis.html  

===========================     
## 'stack'　　 　　　　
```C++
#include<stack>
stack <int> s;
s.empty();//See if the stack is empty;
s.push();//Add element into the stack;
s.top();//Get the top element from the stack;
s.pop();// Pop the top element from the stack;
```  
===========================    
## 'map'  　 
### Diference among map hash_map and unordered_map:  
 Reference: https://www.cnblogs.com/zzyoucan/p/9127660.html  
 
### unordered_map   
```C++
#include <unordered_map>
unordered_map<char , int> unord_map;
unordered_map<char , int>:: iterator it;

// insert the element
unord_map.insert(map<char,int>::value_type('a', 1));
unord_map['b'] = 2; 
unord_map.insert(pair<int,float>('c' , 3));
//insert the element

//find
bool result = (unord_map.find('d') == unord_map.end());

//visit
it = unord_map.find('a');
cout<<it->first;
cout<<it->second;

//delete
unord_map.erase(it);
```
Reference: https://blog.csdn.net/fx677588/article/details/76401870  

===========================   
## 'list'　
===========================  
## 'deque'　
```C++
deque<type> deq;  // 声明一个元素类型为type的双端队列que
deque<type> deq(size);  // 声明一个类型为type、含有size个默认值初始化元素的的双端队列que
deque<type> deq(size, value);  // 声明一个元素类型为type、含有size个value元素的双端队列que
deque<type> deq(mydeque);  // deq是mydeque的一个副本
deque<type> deq(first, last);  // 使用迭代器first、last范围内的元素初始化deq
deq[ ]：用来访问双向队列中单个的元素。
deq.front()：返回第一个元素的引用。
deq.back()：返回最后一个元素的引用。
deq.push_front(x)：把元素x插入到双向队列的头部。
deq.pop_front()：弹出双向队列的第一个元素。
deq.push_back(x)：把元素x插入到双向队列的尾部。
deq.pop_back()：弹出双向队列的最后一个元素。
```
===========================  
## 'queue'　　　　　  
===========================  
## 'set'　
unordered_set<char>lookset;
lookset.find(s[right]) == lookset.end()
lookset.insert(s[right]);
lookset.erase(s[left]);　　
lookset.size()

　　　 
===========================  
## 'string'　  　　　　
===========================  
## 'cmath'  　  　　　 

