#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include "PagedArray.h"
#include "sorter.h"
#include <chrono>

using namespace std;

//Copiar archivo binario (input → output)
void copiarArchivo(const string& input, const string& output)
{
    ifstream in(input, ios::binary);
    ofstream out(output, ios::binary);

    if (!in || !out)
    {
        cout << "Error copiando archivo\n";
        return;
    }

    out << in.rdbuf();

    in.close();
    out.close();
}

//Obtener cantidad de elementos en archivo
long long obtenerTotalElementos(const string& filepath)
{
    ifstream file(filepath, ios::binary | ios::ate);

    if (!file)
    {
        cout << "Error abriendo archivo\n";
        return 0;
    }

    long long size = file.tellg(); // tamaño en bytes
    file.close();

    return size / sizeof(int);
}
void exportarATexto(const string& binFile, const string& txtFile)
{
    ifstream in(binFile, ios::binary);
    ofstream out(txtFile);

    if (!in || !out)
    {
        cout << "Error abriendo archivos\n";
        return;
    }

    int value;
    bool primero = true; // para no poner coma antes del primer número

    while (in.read(reinterpret_cast<char*>(&value), sizeof(int)))
    {
        if (value != 0) // ignoramos ceros
        {
            if (!primero)
                out << ", "; // agregamos coma y espacio antes de todos excepto el primero
            out << value;
            primero = false;
        }
    }

    in.close();
    out.close();

    cout << "Archivo exportado a texto sin ceros y con comas: " << txtFile << endl;
}

int main(int argc, char* argv[])
{
    if (argc < 11)
    {
        cout << "Uso:\n";
        cout << "./sorter -input <input> -output <output> -alg <algoritmo> -pageSize <n> -pageCount <n>\n";
        return 1;
    }

    string inputPath;
    string outputPath;
    string algoritmo;
    int pageSize = 0, pageCount = 0;

    // Parseo de argumentos
    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-input") == 0)
            inputPath = argv[++i];
        else if (strcmp(argv[i], "-output") == 0)
            outputPath = argv[++i];
        else if (strcmp(argv[i], "-alg") == 0)
            algoritmo = argv[++i];
        else if (strcmp(argv[i], "-pageSize") == 0)
            pageSize = stoi(argv[++i]);
        else if (strcmp(argv[i], "-pageCount") == 0)
            pageCount = stoi(argv[++i]);
    }

    // Validaciones 
    if (inputPath.empty() || outputPath.empty() || algoritmo.empty() || pageSize <= 0 || pageCount <= 0)
    {
        cout << "Argumentos invalidos\n";
        return 1;
    }

    // Copiar archivo
    copiarArchivo(inputPath, outputPath);

    // Calcular cantidad de elementos
    long long totalElements = obtenerTotalElementos(outputPath);

    cout << "Elementos: " << totalElements << endl;
    cout << "Algoritmo: " << algoritmo << endl;

    // Crear PagedArray
    {
        PagedArray arr(outputPath, totalElements, pageSize, pageCount);
       
        // Selección de algoritmo
        if (algoritmo == "merge")
    
        {
            auto start = std::chrono::high_resolution_clock::now();
            Sorter::mergeSort(arr, 0, totalElements - 1);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            long long total_ms = duration.count();

            long long minutes = total_ms / 60000;
            long long seconds = (total_ms % 60000) / 1000;
            long long milliseconds = total_ms % 1000;
            cout << "Tiempo de ejecucion: "
            << minutes << "m "
            << seconds << "s "
            << milliseconds << "ms\n";
                
        }
        else if (algoritmo == "quick")
        {
            auto start = std::chrono::high_resolution_clock::now();
            Sorter::quickSort(arr, 0, totalElements - 1);
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
            long long total_ms = duration.count();

            long long minutes = total_ms / 60000;
            long long seconds = (total_ms % 60000) / 1000;
            long long milliseconds = total_ms % 1000;
            cout << "Tiempo de ejecucion: "
            << minutes << "m "
            << seconds << "s "
            << milliseconds << "ms\n";
         
            
        }
        else
        {
            cout << "Algoritmo no valido\n";
            return 1;
        }

        // Estadísticas
        cout << "\n--- Estadisticas ---\n";
        
        arr.printStats();
        arr.flushAll();
        exportarATexto(outputPath, "salida.txt");
    }
    

    cout << "\nOrdenamiento completado\n";

    return 0;
}