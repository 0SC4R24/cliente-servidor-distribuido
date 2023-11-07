#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../../include/utils/socket.h"
#include "../../include/utils/tipos.h"
#include "../../include/filemanager/filemanager_stub.h"

int main(int argc, char **argv)
{
    FileManager_stub *fm1 = new FileManager_stub("./dirprueba/");
    vector<string *> *vfiles = fm1->listFiles();

    cout << "Lista de ficheros en el directorio de prueba:\n";
    for (unsigned int i = 0; i < vfiles->size(); i++)
    {
        cout << "Fichero: " << vfiles->at(i)->c_str() << endl;
    }

    return 0;
}