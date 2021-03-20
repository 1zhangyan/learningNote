# 排序

## 堆排序Heapsort
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
## QickSort 快排
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
