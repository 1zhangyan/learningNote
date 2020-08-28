## heapsort

```C++
#include<bits/stdc++.h>
using namespace std;

void swap(int arr[] , int a , int b)
{
	int temp = arr[a];
	arr[a] = arr[b];
	arr[b] = temp;
	return ;
 } 

void heapify(int arr[] , int i , int n)
{
	if(i >= n)
	return ;
	int c1 = i*2 +1 ;
	int c2 = i*2 + 2 ; 
	int max = i;
	if (c1 < n && arr[c1] >arr[max])
		max = c1;
	if(c2 < n &&arr[c2] > arr[max])
		max = c2;
	if(max != i)
	{
		swap(arr, max , i);
		heapify(arr , max , n);
	}	
}


void build_heap(int arr[] ,int n)
{
	int last_node = n -1;
	int parent  = (last_node -1) /2;
	int i;
	for(int i = parent; i>=0 ; i--)
	{
		heapify(arr , i , n);
	}
	
}

void heap_sort(int arr[] , int n)
{
	build_heap(arr , n);
	for(int i = n-1 ; i>=0 ; i--)
	{
		swap(arr, i ,0);
		heapify(arr , 0 , i );
	}
}



int main()
{
	int arr[] = {5, 4 , 3 , 8 ,66 , 7, 6 , 7};
	int n =  sizeof(arr)/sizeof(arr[0]);
	heap_sort(arr , n);
	for(int i = 0 ; i < n;i++)
	cout<<arr[i]<<" ";
	return  0;
 } 
```