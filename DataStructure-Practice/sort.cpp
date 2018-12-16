/* PRESET CODE BEGIN - NEVER TOUCH CODE BELOW */

#include "assert.h"
#include <iostream>
using namespace std;

/* PRESET CODE END - NEVER TOUCH CODE ABOVE */

void swap(int &a,int &b){
    int t=a;
    a=b;
    b=t;
}

void Sort(int *a,int n){
	int i=0,j=0,k=n-1;
	int t;
    while(j<=k){
        if(a[j]==2){
                j++;
        }
        else if(a[j]==1){
            swap(a[j],a[i]);
            if(i<j){
                cout<<i<<"<--->"<<j<<endl;
            }
            i++;
            j++;
        }
        else{
            swap(a[k],a[j]);
            if(j<k){
                cout<<j<<"<--->"<<k<<endl;
            }
            k--;
        }
    }
    for(int i=0;i<n;i++){
        cout<<a[i];
    }
}


/* PRESET CODE BEGIN - NEVER TOUCH CODE BELOW */

int main() {
	int n;
	cin >> n;
	int * a= new int[n];
	for (int i=0;i<n;i++)
		cin>>a[i];
	Sort(a,n);
}

/* PRESET CODE END - NEVER TOUCH CODE ABOVE */
