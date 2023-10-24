#include <iostream>

#include "../../include/filemanager/filemanager.h"

int main(int argc, char **argv)
{
    FileManager *fm = new FileManager("./dirprueba/");
    vector<string *> *vfiles = fm->listFiles();
    cout << "Lista de ficheros en el directorio de prueba:\n";
    for (unsigned int i = 0; i < vfiles->size(); ++i)
    {
        cout << "Fichero: " << vfiles->at(i)->c_str() << endl;
    }
    cout << "Leyendo el primer fichero del directorio de prueba:\n";

    char *data = nullptr;
    unsigned long int fileLen = 0;
    fm->readFile(&(*(vfiles->at(0)))[0], data, fileLen);
    cout << "Escribiendo el primer fichero del directorio de prueba:\n";
    fm->writeFile(&(*(vfiles->at(0)))[0], data, fileLen);
    cout << "Liberando lista de ficheros:\n";
    fm->freeListedFiles(vfiles);
    cout << "Liberando datos de fichero leído:\n";

    delete[] data;
    return 0;
}
