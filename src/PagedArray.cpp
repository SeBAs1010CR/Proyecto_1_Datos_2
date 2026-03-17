#include <iostream>
#include "PagedArray.h"
#include <fstream>
#include <cstdlib>

// esta func pertenece a <- pagedarray::
PagedArray::PagedArray(const std::string& filepath, long long totalElements, int pageSize, int pageCount)
{
    this->filepath = filepath;
    this->totalElements = totalElements;
    this->pageSize = pageSize;
    this->pageCount = pageCount;
    
    pages = new int*[pageCount]; // espacio para punteros
    loadedPages = new int[pageCount];
    dirtyPages = new bool[pageCount];

    for (int i = 0; i < pageCount; i++) // reccorre todos los frames 
    {
        pages[i] = new int[pageSize];
        loadedPages[i] = -1;
        dirtyPages[i] = false;
    }

    pageHits = 0;
    pageFaults = 0;
    nextVictim = 0;
}
PagedArray::~PagedArray()
{
    for (int i = 0; i < pageCount; i++)
    {
        delete[] pages[i];
    }

    delete[] pages;
    delete[] loadedPages;
    delete[] dirtyPages;
}
int PagedArray::findPageInMemory(int pageNumber){
    for(int i = 0; i < pageCount; i++)
    {
        if(loadedPages[i] == pageNumber)
        {
            return i;
        }
    }
    return -1;
}
int PagedArray::getFreeFrame()
{
    for(int i =0; i < pageCount; i++)
    {
        if (loadedPages[i] == -1)
        {
            return i;
        }
    }
    return -1;
}
void PagedArray::loadPage(int pageNumber)
{
    int frame = getFreeFrame();

    if (frame == -1)
    {
        cout << "No hay espacio libre (falta implementar reemplazo)\n";
        return;
    }

    long long offset = (long long)pageNumber * pageSize * sizeof(int);

    ifstream file(filepath, ios::binary);

    if (!file)
    {
        cout << "Error al abrir el archivo\n";
        return;
    }

    file.seekg(offset);

    file.read(reinterpret_cast<char*>(pages[frame]), pageSize * sizeof(int));

    file.close();

    loadedPages[frame] = pageNumber;
    dirtyPages[frame] = false;

    pageFaults++;
    cout << "Cargando pagina: " << pageNumber << endl;
}
int& PagedArray::operator[](long long index)
{
    long long pageNumber = index / pageSize;
    int offset = index % pageSize;

    int frame = findPageInMemory(pageNumber);

    if (frame != -1)
    {
        pageHits++;
    }
    else
    {
        loadPage(pageNumber);
        frame = findPageInMemory(pageNumber);
    }
    cout << "Page: " << pageNumber << " Frame: " << frame << endl;
    return pages[frame][offset];
}