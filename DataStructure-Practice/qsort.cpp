#include <iostream>
using namespace std;

int getPartition(int *nums, int low, int height)
{
    int keyVal = nums[height];
    int i = low - 1;
    for (int j = low; j < height; j++)
    {

        if (nums[j] <= keyVal)
        {
            i = i + 1;
            swap(nums[i], nums[j]);
        }
    }
    swap(nums[i + 1], nums[height]);
    return i+1;
}

void quickSort(int *nums,int low,int height) {

    if (low<height)
    {
        int mid = getPartition(nums, low, height);
        quickSort(nums, low, mid-1);
        quickSort(nums, mid + 1, height);
    }
}

int main()
{
    int a[] = {5,1,3,2,1,9};

    quickSort(a, 0, 5);

    for(int i = 0; i < 6; i++)
    {
        cout << a[i] << "";
    }

    return 0;
}
