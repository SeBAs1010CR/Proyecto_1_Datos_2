#include "sorter.h"
#include <iostream>

using namespace std;

void Sorter::selectionSort(PagedArray& arr, long long n)
{
    for (long long i = 0; i < n - 1; i++)
    {
        long long minIndex = i;

        for (long long j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[minIndex])
            {
                minIndex = j;
            }
        }

        if (minIndex != i)
        {
            int temp = arr[i];
            arr[i] = arr[minIndex];
            arr[minIndex] = temp;
        }

        cout << "Paso " << i + 1 << " completado" << endl;
    }
}