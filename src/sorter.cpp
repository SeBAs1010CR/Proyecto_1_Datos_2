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
            if (arr.get(j) < arr.get(minIndex))
            {
                minIndex = j;
            }
        }

        if (minIndex != i)
        {
            int temp = arr.get(i);
            arr.set(i, arr.get(minIndex));
            arr.set(minIndex, temp);
        }

        //cout << "Paso " << i + 1 << " completado" << endl;
    }
}
void Sorter::insertionSort(PagedArray& arr, long long n) //[71, 19, 19, 3]
{                                                      
    for (long long i = 1; i < n; i++)
    {
        int key = arr.get(i);   // elemento a insertar 
        long long j = i - 1;

        // mover elementos mayores hacia la derecha
        while (j >= 0 && arr.get(j) > key)
        {
            arr.set(j + 1, arr.get(j)); // desplaza el elemento hacia la derecha
            j--; // mueve el índice hacia la izquierda
        }

        // insertar en posición correcta
        arr.set(j + 1, key); // coloca la clave en su posición final    

        //cout << "Paso " << i << " completado" << endl;
    }
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


