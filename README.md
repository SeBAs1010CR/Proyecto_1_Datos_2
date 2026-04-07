Proyecto de Generador y Ordenamiento de Archivos Binarios

Este proyecto consiste en dos programas principales:

--Generador de archivos binarios (generator): crea archivos .bin de enteros aleatorios de distintos tamaños.
--Ordenador de archivos binarios (Sorter + PagedArray): ordena los enteros usando distintos algoritmos de ordenamiento y maneja la memoria mediante paginación simulada.

------Requisitos------
Compilador de C++ compatible con C++11 o superior.
Sistema operativo: Windows, Linux o macOS.
Memoria suficiente para simular paginación (dependiendo del tamaño del archivo).

--------1. Uso del Generador--------

El generador crea un archivo binario con enteros aleatorios y puede exportar una versión en texto para verificar contenido.

Compilar
g++ generator.cpp -o generator

Ejecutar
./generator -size <SIZE> -output <OUTPUT FILE PATH>

Argumentos
-size <SIZE>: tamaño del archivo a generar. Valores permitidos:
SMALL → 256 MB
MEDIUM → 512 MB
LARGE → 1 GB
-output <OUTPUT FILE PATH>: ruta donde se guardará el archivo .bin.
Si no se coloca .bin, se agregará automáticamente.


Ejemplo
./generator -size SMALL -output datos_pequeños.bin

Se exporta automaticamente un archivo con output.txt

2. Uso del Ordenador (Sorter + PagedArray)

El ordenador permite ordenar archivos binarios muy grandes usando paginación para simular memoria limitada.

Compilar
g++ main.cpp PagedArray.cpp Sorter.cpp -o sorter

Ejecutar
./sorter <archivo_binario> <algoritmo> <page_size> <page_count>

Argumentos

<archivo_binario>: ruta del archivo generado por el generador.
<algoritmo>: algoritmo de ordenamiento:
SELECTION → Selection Sort
INSERTION → Insertion Sort
QUICK → Quick Sort
<page_size>: número de enteros por página (ejemplo: 1024).
<page_count>: número de páginas en memoria simultáneamente (frames).


Ejemplo
./sorter datos_pequeños.bin QUICK 1024 256
Ordena el archivo datos_pequeños.bin con Quick Sort.
Cada página tiene 1024 enteros, y la memoria puede cargar 256 páginas a la vez.

Notas importantes
Paginación:
page_size * page_count determina cuánta memoria se simula.
Si el archivo es muy grande y la memoria pequeña, habrá más page faults.


Estadísticas:

Después de ordenar, el programa imprime:

Tiempo de ejecución: M:S:MS
Page Hits: <número de accesos a páginas ya cargadas>
Page Faults: <número de cargas de página desde disco>

Sugerencias de paginación según tamaño de archivo:

Tamaño archivo	Page Size	Page Count	Memoria simulada
    256 MB	       1024	        256	        ~1 MB
    512 MB	       2048	        512	        ~4 MB
    1 GB	       4096	        512	        ~8 MB


3. Exportar a texto (opcional)

Si desea ver los números ordenados en un archivo de texto:

El programa al finalizar genera un archivo de salida.txt y salida.bin para verificar el ordenamiento.

4. Consejos de uso
Para archivos muy grandes (>1 GB), es recomendable usar QUICK y ajustar page_size para que las páginas sean lo suficientemente grandes y minimizar page faults.
Para pruebas rápidas, usar SMALL y INSERTION o SELECTION.
Para pruebas Ultra rápidas usar SMALLTEST.