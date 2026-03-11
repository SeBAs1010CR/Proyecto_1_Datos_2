#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>

using namespace std;


int main(int argc, char* argv[]) {

    srand(time(NULL));
    string size;
    string outputpath;
    long long integers = 0;
    long long bytes = 0;
    int randomnum;
    ofstream file;
    long long KB = 1024;

    for(int i = 1; i < argc; i++){
        string arg = argv[i];

        if(arg == "-size"){

            size = argv[i + 1];
            cout << "size correcto" << endl;
            i++;
        }


        else if(arg == "-output"){

            outputpath = argv[i + 1];
            cout << "output correcto" << endl;
            i++;

        }
        
    }
    if (size.empty() || outputpath.empty()) {
            cout << "Error argumento invalido. Por favor especifique el tamano y la direccion del archivo de la siguiente forma:  generator –size <SIZE> -output <OUTPUT FILE PATH>" << endl;
            return -1;
        }
    
    if (outputpath.size() < 4 || outputpath.substr(outputpath.size() - 4) != ".bin") {

    outputpath += ".bin";
    }
    
    
    if(size == "SMALL"){
        bytes = 512 * KB * KB;
    }
    if(size == "MEDIUM"){
        bytes = 1 * KB * KB * KB;
    }
    if(size == "LARGE"){
        bytes = 2 * KB * KB * KB;
    }
    if (size != "SMALL" && size != "MEDIUM" && size != "LARGE" ){
        cout << "El tamaño especificado no es válido solo se acepta: SMALL, MEDIUM y LARGE" << endl;
        return -1;
    }
    
    integers = bytes / sizeof(int);
    cout << integers << endl;


    file.open(outputpath, ios::binary);
    if (!file) {
    cout << "Error al abrir el archivo" << endl;
    return 1;
    }

    for(long long i = 0; i < integers; i++ ){
        randomnum = rand();
        file.write(reinterpret_cast<char*>(&randomnum), sizeof(int)); // trata la dirección de este entero como si fuera una dirección a bytes

    }
    file.close();
    cout << "Archivo creado y cerrado exitosamente" << endl;

    return 0;
}
 