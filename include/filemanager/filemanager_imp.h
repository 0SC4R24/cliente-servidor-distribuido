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
                delete fm;
				conexionCerrada = true;
                pack<int>(packetOut, 1);
            } break;
            case FL_LISTFILES:
            {
                vector<string *> *flist;
                flist = fm->listFiles();
                pack(packetOut, 1);
                serializar_lista_ficheros(packetOut, flist);
            } break;
            case FL_FREELISTEDFILES:
            {
                vector<string *> *flist = new vector<string *>();
                deserializar_lista_ficheros(packetIn, flist);
                fm->freeListedFiles(flist);
                pack(packetOut, 1);
            } break;
            case FL_READFILE:
            {
                int fileSize = unpack<int>(packetIn);
                char *fileName = new char[fileSize];
                unpackv<char>(packetIn, fileName, fileSize);

                unsigned long int dataLength;
                char *data;
                fm->readFile(fileName, data, dataLength);

                pack(packetOut, 1); 
                pack(packetOut, dataLength);
                packv(packetOut, data, dataLength);
            } break;
            case FL_WRITEFILE:
            {
                int fileNameSize = unpack<int>(packetIn);
                char* fileName = new char[fileNameSize];
                unpackv<char>(packetIn, fileName, fileNameSize);

                unsigned long int dataLength = unpack<unsigned long int>(packetIn);
                char* data = new char[dataLength];
                unpackv<char>(packetIn, data, dataLength);

                fm->writeFile(fileName, data, dataLength);

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
