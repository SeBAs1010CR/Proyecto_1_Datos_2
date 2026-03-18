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

    pages = new int*[pageCount]; // arreglo de punteros a int (cada puntero es una página)
    loadedPages = new int[pageCount]; // cada posición indica qué página está cargada en ese frame (-1 si está vacío)
    dirtyPages = new bool[pageCount]; // cada posición indica si la página en ese frame ha sido modificada (true) o no (false)

    for (int i = 0; i < pageCount; i++) // inicializa cada frame de página
    {
        pages[i] = new int[pageSize]; // por cada frame, asigna un bloque de memoria para una página completa
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
        if (loadedPages[i] != -1 && dirtyPages[i])
        {
            flushPage(i);
        }
    }

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
        cout << "No hay espacio libre reemplazando...\n";
        frame = nextVictim;
        if(dirtyPages[frame])
        {
            flushPage(frame);
        }
        nextVictim = (nextVictim + 1) % pageCount;
    }


    long long offset = (long long)pageNumber * pageSize * sizeof(int); // calcula el desplazamiento en bytes para leer la página correcta

    ifstream file(filepath, ios::binary);

    if (!file)
    {
        cout << "Error al abrir el archivo\n";
        return;
    }

    file.seekg(offset); // mueve el puntero de lectura al inicio de la página que queremos cargar

    file.read(reinterpret_cast<char*>(pages[frame]), pageSize * sizeof(int)); // lee la página completa (pageSize enteros) desde el archivo al frame correspondiente en memoria

    file.close();

    loadedPages[frame] = pageNumber;
    
    dirtyPages[frame] = false; 
    pageFaults++;
    cout << "Cargando pagina: " << pageNumber << endl;
}
void PagedArray::flushPage(int frame)
{
    long long pageNumber = loadedPages[frame];
    long long offset = (long long)pageNumber * pageSize * sizeof(int); // calcula el desplazamiento en bytes para escribir la página correcta

    fstream file(filepath, ios::binary | ios::in | ios::out); // abre el archivo en modo binario y de lectura/escritura

    if (!file)
    {
        cout << "Error al abrir el archivo para escribir\n";
        return;
    }

    file.seekp(offset); // mueve el puntero de escritura al inicio de la página que queremos guardar

    file.write(reinterpret_cast<char*>(pages[frame]), pageSize * sizeof(int)); // escribe la página completa (pageSize enteros) desde el frame correspondiente en memoria al archivo

    file.close();

    dirtyPages[frame] = false; // marca la página como limpia después de guardarla
    cout << "Guardando pagina: " << pageNumber << endl;
    return;
}
int& PagedArray::operator[](long long index)
{
    if (index < 0 || index >= totalElements)
    {
        cout << "Indice fuera de rango\n";
        exit(1);
    }
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
        if (frame == -1)
        {
            cout << "Error: no se pudo cargar la pagina\n";
            exit(1);
        }
    }
    cout << "Page: " << pageNumber << " Frame: " << frame << endl;
    dirtyPages[frame] = true; // marca la página como sucia cada vez que se accede a ella para modificarla
    return pages[frame][offset];
}

void PagedArray::printStats()
{
    cout << "Page Hits: " << pageHits << endl;
    cout << "Page Faults: " << pageFaults << endl;
}