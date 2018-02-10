#include <iostream>
#include <cstdio>
using namespace std;
void swap (int *a ,int*b)
{
	int t = *a;
	*a=*b;
	*b=t;
}
void InsertSort(int a[],int n)
{
    int i,j,t;
    for(i=1;i<n;i++){
		for(j=i;j>0;j--){
			if(a[j]<a[j-1]){
				swap(&a[j],&a[j-1]);
			}
		}
    }
	for(i =0 ;i<n ;i++){
		printf("%d ",a[i]);
	}
	cout<<endl;
}
int main()
{
    int a[20];
    int i,j,n;
    cin>>n;
    for(i=0;i<n;i++){
        scanf("%d",a+i);
    }
    InsertSort(a,n);
    return 0;
}
