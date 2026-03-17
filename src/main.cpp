#include <iostream>
#include "PagedArray.h"

using namespace std;

int main()
{
    // archivo pequeño para probar
    PagedArray arr("test.bin", 10000, 1000, 2);

    cout << arr[10] << endl;     // carga página 0
    cout << arr[20] << endl;     // hit (misma página)

    cout << arr[1500] << endl;   // carga página 1

    // ⚠️ aquí ya estarías al límite (2 páginas)

    return 0;
}