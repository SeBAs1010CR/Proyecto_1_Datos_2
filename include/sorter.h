#ifndef SORTER_H
#define SORTER_H

#include "PagedArray.h"

class Sorter
{
public:
    static void selectionSort(PagedArray& arr, long long n);
    static void insertionSort(PagedArray& arr, long long n);
    static void quickSort(PagedArray& arr, long long low, long long high);
};

#endif