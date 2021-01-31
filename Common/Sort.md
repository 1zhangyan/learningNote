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
	for(int i = n/2 - 1 ; i >= 0 ; i--)
		Adjust(arr,i,n-1);
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
	int k = arr[left];
	while(i < j)
	{
		while(arr[j]>=key)
			j--;
		while(arr[i]<=key)
			i++;
		swap(arr[i],arr[j]);
	}
	swap(arr[i],arr[left]);
	QuickSort(arr,left,i-1);
	QuickSort(arr,i+1,right);
}
```

## 归并排序
