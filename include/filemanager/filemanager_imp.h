#pragma once

//
// Created by Oscar on 31/10/2023.
//

#ifndef FILEMANAGER_IMP_H
#define FILEMANAGER_IMP_H

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <map>
#include <vector>
#include <dirent.h>
#include "filemanager.h"
#include "../utils/socket.h"
#include "../utils/serializacion.h"
#include "../utils/tipos.h"

using namespace std;

class FileManager_imp
{
private:
    int clientID;
    FileManager *fm = nullptr;
public:
    bool conexionCerrada = false;

    FileManager_imp()
    {

    }

    void recOp(int clientID)
    {
        std::vector<unsigned char> packetIn;
        std::vector<unsigned char> packetOut;

        recvMSG(clientID, packetIn);

        e_operacion_filemanager op = unpack<e_operacion_filemanager>(packetIn);

        switch (op)
        {
            case FL_CONSTRUCTOR:
            {
                cout << "CREANDO CONSTRUCTOR" << endl;
                std::string dato;

                int size = unpack<int>(packetIn);

                char *d = new char[size];
                unpackv<char>(packetIn, d, size);
                dato = string(d);
                delete[] d;

                fm = new FileManager(dato);

                int ok = 1;
                pack(packetOut, ok);
            } break;
            case FL_DESTRUCTOR:
            {
                cout << "DESTRUYENDO FILEMANAGER" << endl;
                delete fm;
				conexionCerrada = true;
                pack<int>(packetOut, 1);
            } break;
            case FL_LISTFILES:
            {
                cout << "LISTANDO ARCHIVOS" << endl;
                vector<string *> *flist;
                flist = fm->listFiles();
                pack(packetOut, 1);
                serializar_lista_ficheros(packetOut, flist);
            } break;
            case FL_FREELISTEDFILES:
            {
                cout << "LIBERANDO LISTA DE ARCHIVOS" << endl;
                vector<string *> *flist = new vector<string *>();
                deserializar_lista_ficheros(packetIn, flist);
                fm->freeListedFiles(flist);
                pack(packetOut, 1);
            } break;
            case FL_READFILE:
            {
                cout << "LEYENDO ARCHIVO" << endl;
                // Leer fichero
                int fileSize = unpack<int>(packetIn);
                char *fileName;
                unpackv<char>(packetIn, fileName, fileSize);
                
                pack(packetOut, 1);
            } break;
            case FL_WRITEFILE:
            {
                // Escribir fichero
                pack(packetOut, 1);
            } break;
            default:
            {
                std::cout << "ERROR: operacion no valida\n";
            } break;
        };

        sendMSG(clientID, packetOut);
    }
};

#endif // FILEMANAGER_IMP_H
