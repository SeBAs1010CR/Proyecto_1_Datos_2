#include "sorter.h"
#include <iostream>

using namespace std;

void Sorter::mergeSort(PagedArray& arr, long long left, long long right)
{
    if (left >= right)
        return;

    long long mid = (left + right) / 2;

    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);

    merge(arr, left, mid, right);
}
void Sorter::merge(PagedArray& arr, long long left, long long mid, long long right)
{
    long long size = right - left + 1;
    int* temp = new int[size];  // buffer en RAM

    long long i = left;
    long long j = mid + 1;
    long long k = 0;

    // Mezclar ambas mitades
    while (i <= mid && j <= right)
    {
        if (arr[i] <= arr[j])   //  acceso a PagedArray
        {
            temp[k++] = arr[i++];
        }
        else
        {
            temp[k++] = arr[j++];
        }
    }

    // Copiar lo restante
    while (i <= mid)
    {
        temp[k++] = arr[i++];
    }

    while (j <= right)
    {
        temp[k++] = arr[j++];
    }

    // Copiar de vuelta al PagedArray
    for (long long x = 0; x < size; x++)
    {
        arr[left + x] = temp[x];   // escritura en PagedArray
    }

    delete[] temp;
}

long long partition(PagedArray& arr, long long low, long long high)
{
    int pivot = arr.get(high);  // último elemento como pivot
    long long i = low - 1;

    for (long long j = low; j < high; j++)
    {
        if (arr.get(j) <= pivot)  // mantiene estabilidad
        {
            i++;
            // swap manual usando el operador []
            int temp = arr.get(i);
            arr.set(i, arr.get(j));
            arr.set(j, temp);
        }
    }

    // colocar pivot en su posición correcta
    int temp = arr.get(i + 1);
    arr.set(i + 1, arr.get(high));
    arr.set(high, temp);

    return i + 1; // devuelve índice del pivot
}

// QuickSort recursivo
void Sorter::quickSort(PagedArray& arr, long long low, long long high)
{
    if (low < high)
    {
        long long pi = partition(arr, low, high);

        // ordenar subarreglos
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}


