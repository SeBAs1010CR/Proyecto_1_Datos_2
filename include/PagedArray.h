#ifndef PAGEDARRAY_H
#define PAGEDARRAY_H // si no he usado este .h usalo si ya se uso entonces no la repitas

#include <iostream>

using namespace std;

class PagedArray {

private:
    string filepath; // ruta de arch bin
    long long totalElements; //total de enteros en el arch

    int pageSize; // enteros que caben en 1 pag
    int pageCount; // numero de pag en memoria al mismo tiempo

    int** pages; // memoria con datos reales apunta a varios arreglos
    int* loadedPages; // solo números (qué página está cargada dentro de cada frame) 
    bool* dirtyPages; // si una pag se modifico

    long long pageHits; // ya en memoria
    long long pageFaults; //en disco
    int nextVictim; //para FIFO

    int findPageInMemory(int pageNumber); // busca si ya esta cargada
    int getFreeFrame(); // busca un frame vacio 
    void loadPage(int pageNumber); //carga pag desde el arch a la memor
    void flushPage(int frame); // guarda pag de la memoria al arch
    
public:
    int& operator[](long long index); // operador para usar arr[]
    void printStats(); // imprime stats de hits y faults
    int get(long long index); // obtiene el valor en un índice específico
    void set(long long index, int value); // establece el valor en un índice específico
    void flushAll(); // guarda todas las páginas modificadas
    int accessFrame(long long index, bool isWrite); // accede al frame en memoria correspondiente al índice


    //builder
    PagedArray(const string& filepath,
               long long totalElements,
               int pageSize,
               int pageCount);

    ~PagedArray(); // destroyer

};

#endif