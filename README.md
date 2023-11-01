# Práctica 1: Objetos remotos
En esta primera práctica se suministran dos programas con la siguiente funcionalidad:
[![](https://cdn.loom.com/sessions/thumbnails/d05ad20900f1463ab9c17f115e0742b4-with-play.gif)](https://www.loom.com/share/d05ad20900f1463ab9c17f115e0742b4)

## Programa 1: File Manager
Este programa sirve para listar los archivos de un directorio local, y realizar funciones básicas de
lectura/escritura sobre ellos. Está compuesto por los siguientes archivos:
- Filemanager.cpp/h: Esta clase implementa las funciones de listado de un directorio local
al ejecutable, pasado por parámetros al constructor. Tiene las siguientes funciones:
- ListFiles : Devuelve un vector con los nombres de los ficheros encontrados en el
directorio local. OJO, no es una lista dinámica, si se escriben nuevos ficheros no
se añadirán automáticamente. Hay que liberar esa lista usando el método
“freeListedFiles”.
  - ReadFile: Si se le pasa un nombre de uno de los ficheros encontrados en el
    directorio, leerá su tamaños y contenido. Lo almacenará en las variables pasadas
    por parámetros.
  - WriteFile: Análoga al anterior, escribirá el contenido fichero en el directorio local.
  Si el fichero no existía en el directorio, se añadirá a la lista de ficheros.
- Main_fm.cpp: Programa principal de prueba. Inicia una clase de tipo “fileManager”, lista
los archivos que hubiera en el directorio “dirprueba” (OJO, tenéis que crearlo y rellenar
vosotros con vuestros archivos de prueba), e intenta escribir uno de los ficheros que se
encontrara en un directorio local al ejecutable.

## Programa 2: Multiplicación de matrices
Este programa implementa la multiplicación de matrices de NxM flas/columnas. Está compuesto por los
siguientes archivos:
- MultMatrix.h/cpp: Esta clase implementta los métodos básicos de multiplicación de
matrices. En concreto tiene los siguientes métodos:
  - Estructura matrix_t: Esta estructura es de uso interno, aunque podéis acceder a
  sus datos cuando sea necesario. Contiene los siguientes registros:
    - Rows y cols: Número de filas y columnas de las que está compuesta la
    matriz:
    - Data: Puntero a un array de números, con los datos de la matriz. En caso
    de matrices creadas correctamente, contendrá “rows * cols” números en
    formato integer de 32 bit.
  - ReadMatrix: Función de lectura de datos desde fichero. Se le debe pasar un
  nombre de fichero que contenga los datos de una matriz compatible con este
  programa. Leerá los datos y en caso de éxito devolverá una estructura matrix_t
  con los datos de la matriz.
  - WriteMatrix: Función análoga a readMatrix, escribirá una matriz almacenada en
  una estructura matrix_t en un fichero local al ejecutable.
  - MultMatrix: función que multiplica matrices. Recibe por parámetros dos
  estructuras matrix_t, y devuelve como resultado una tercera estructura matrix_t.
  - CreateIdentity/createRandom: Estas dos funciones sirven para crear matrices por
  defecto. La primera se rellenará con números aleatorios, la segunda sirve para
  crear una matriz identidad.
- Main_multMatrix.cpp: Programa principal para pruebas de esta clase. Crea una matriz
random, una matriz identidad, las multiplica, el resultado es escrito a un archivo, se vuelve
a leer, se vuelve a multiplicar, y se escribe por segunda vez el resultado

# Compilación y ejecución de los programas
```bash
# Creación de los ejecutables
mkdir "out"
cd "out"
cmake ..
make

# Ejecución del broker
./broker

# Ejecución del servidor
./mm_server

# Ejecución del cliente
./mm_client
```

# Autores
- Adrian Toral
- Oscar Viudez
