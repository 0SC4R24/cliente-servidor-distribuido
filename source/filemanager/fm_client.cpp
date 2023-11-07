#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../../include/utils/socket.h"
#include "../../include/utils/tipos.h"
#include "../../include/filemanager/filemanager_stub.h"

int main(int argc, char **argv)
{
    FileManager_stub *fm = new FileManager_stub("./dirprueba/");
    vector<string *> *vfiles = fm->listFiles();

    cout << "Lista de ficheros en el directorio de prueba:\n";
    for (unsigned int i = 0; i < vfiles->size(); i++)
    {
        cout << "Fichero: " << vfiles->at(i)->c_str() << endl;
    }

    cout << "Leyendo el primer fichero del directorio de prueba:\n";
    char *data = nullptr;
    unsigned long int fileLen = 0;
    fm->readFile(&(*(vfiles->at(0)))[0], data, fileLen);
    cout << data;

    cout << "Liberando lista de ficheros:\n";
    fm->freeListedFiles(vfiles);
    cout << "Liberando datos de fichero leído:\n";

    delete[] data;
    delete fm;

    return 0;
}
