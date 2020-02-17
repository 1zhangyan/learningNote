#  'algorithm'   

## sort
```C++
int A[];
vector<int> V;
sort ( A , A+n );
sort ( V.begin() , V.end() );  
```
排序按照从小到大的顺序，需要调整顺序的，交换参数次序即可  

===========================  
# 'vector'  
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
# 'map'  　　　　　 
===========================  
# 'list'　　　　　　
===========================  
# 'deque'　　　　　　
===========================  
# 'queue'　　　　　  
===========================  
# 'set'　　　　　　 
===========================  
# 'stack'　　 　　　　　
===========================  
# 'string'　  　　　　
===========================  
# 'cmath'  　  　　　 

