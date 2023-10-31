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
                dato.resize(size);

                char *d = new char[size];
                unpackv<char>(packetIn, d, size);
                dato = string(d);
                delete[] d;

                fm = new FileManager(dato);

                int ok = 1;
                pack(packetOut, ok);
            }
                break;

            default:
            {
                std::cout << "ERROR: operacion no valida\n";
            }
                break;
        };

        sendMSG(clientID, packetOut);
    }
};

#endif // FILEMANAGER_IMP_H