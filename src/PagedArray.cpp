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
        //cout << "No hay espacio libre reemplazando...\n";
        frame = nextVictim;
        if(dirtyPages[frame])
        {
            flushPage(frame);
        }
        nextVictim = (nextVictim + 1) % pageCount; // va rotando circularmente
    }


    long long offset = (long long)pageNumber * pageSize * sizeof(int); // calcula el desplazamiento en bytes para leer la página correcta offset

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
    //cout << "Cargando pagina: " << pageNumber << endl;
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
    //cout << "Guardando pagina: " << pageNumber << endl;
    return;
}

void PagedArray::flushAll() {
    for (int i = 0; i < pageCount; i++) {
        if (loadedPages[i] != -1 && dirtyPages[i]) {
            flushPage(i);
        }
    }
}
// Devuelve el frame en memoria correspondiente al índice, cargando la página si es necesario
int PagedArray::accessFrame(long long index, bool isWrite)
{
    long long pageNumber = index / pageSize;
    int offset = index % pageSize;

    int frame = findPageInMemory(pageNumber);

    if (frame != -1) {
        pageHits++; // página ya estaba en memoria → hit
    } else {
        loadPage(pageNumber);  // no estaba → pageFaults sumado dentro de loadPage
        frame = findPageInMemory(pageNumber);
        if (frame == -1) {
            cout << "Error: no se pudo cargar la pagina\n";
            exit(1);
        }
    }

    if (isWrite) dirtyPages[frame] = true; // marcar como modificada si es escritura

    return frame;
}

// Getter
int PagedArray::get(long long index)
{
    int frame = accessFrame(index, false);
    int offset = index % pageSize;
    return pages[frame][offset];
}

// Setter
void PagedArray::set(long long index, int value)
{
    int frame = accessFrame(index, true);
    int offset = index % pageSize;
    pages[frame][offset] = value;
}

// Operator[]
int& PagedArray::operator[](long long index)
{
    int frame = accessFrame(index, true);
    int offset = index % pageSize;
    return pages[frame][offset]; //ref al dato
}

void PagedArray::printStats()
{
    cout << "Page Hits: " << pageHits << endl;
    cout << "Page Faults: " << pageFaults << endl;
}