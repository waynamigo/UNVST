#include<iostream>

using namespace std;

int main()
{
	int a,b;
	scanf("%d%d",&a,&b);
	int sum = a*b;
	int direct = 0;
	char matrix[a+1][b+1];
	int base = 65;

	int count =0;
	printf("%d\n",sum);
	int layer=0;
	int i =0,j=0;
	while(sum>=0){
		
		for(;i<a-layer;i++){
			sum--;
			if(sum<0) break;
			matrix[i][j] = count%26+base;
			
			count++;
			cout<<matrix[i][j]<<"("<<i<<','<<j<<") ";
			
		
		}
cout<<endl;
		i--;j++;
		for(;j<b-layer;j++){
			sum--;
			if(sum<0) break;
			matrix[i][j] = count%26+base;
			
			count++;
			cout<<matrix[i][j]<<"("<<i<<','<<j<<") ";
			
			
		}
cout<<endl;
		i--;j--;
		for(;i>=layer;i--){
			sum--;
			if(sum<0) break;
			matrix[i][j] = count%26+base;
			
			count++;
			cout<<matrix[i][j]<<"("<<i<<','<<j<<") ";
			
		
		}
cout<<endl;
		layer++;
		i++;j--;
		for(;j>=layer;j--){
			sum--;
			if(sum<0) break;
			matrix[i][j] = count%26+base;
			//cout<<matrix[i][j]<<' ';
			count++;
			cout<<matrix[i][j]<<"("<<i<<','<<j<<") ";
			
		}
		i++;j++;
cout<<endl;
		
	}

	for(int m=0;m<b;m++){
		for(int n=0;n<a;n++){
			cout<<matrix[n][m]<<' ';
		}
		cout<<endl;
	}

	return 0;
}
