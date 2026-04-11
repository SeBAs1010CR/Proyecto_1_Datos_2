#ifndef SORTER_H
#define SORTER_H

#include "PagedArray.h"

class Sorter
{
private:
    static void merge(PagedArray& arr, long long left, long long mid, long long right);
public:
    static void mergeSort(PagedArray& arr, long long left, long long right);
    static void quickSort(PagedArray& arr, long long low, long long high);
};

#endif