#include <iostream>
#include <fstream>
#include "PagedArray.h"
#include "sorter.h"

using namespace std;

void crearArchivoPrueba(const string& filepath)
{
    int datos[] = {42, 7, 19, 3, 25, 10, 1, 8, 50, 12};
    ofstream file(filepath, ios::binary);

    if (!file)
    {
        cout << "Error creando archivo\n";
        return;
    }

    file.write(reinterpret_cast<char*>(datos), sizeof(datos));
    file.close();
}

void imprimirArchivo(const string& filepath, int totalElements)
{
    ifstream file(filepath, ios::binary);

    if (!file)
    {
        cout << "Error abriendo archivo para leer\n";
        return;
    }

    int valor;
    for (int i = 0; i < totalElements; i++)
    {
        file.read(reinterpret_cast<char*>(&valor), sizeof(int));
        cout << valor << " ";
    }

    cout << endl;
    file.close();
}

int main()
{
    string filepath = "datos.bin";
    int totalElements = 10;
    int pageSize = 4;
    int pageCount = 2;

    crearArchivoPrueba(filepath);

    cout << "Archivo antes de ordenar:\n";
    imprimirArchivo(filepath, totalElements);

    {
        PagedArray arr(filepath, totalElements, pageSize, pageCount);
        cout << "\nOrdenando...\n";
        Sorter::selectionSort(arr, totalElements);
    } // aquí se destruye arr y se guardan las páginas sucias

    cout << "\nArchivo despues de ordenar:\n";
    imprimirArchivo(filepath, totalElements);

    return 0;
}