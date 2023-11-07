#pragma once

//
// Created by Oscar on 31/10/2023.
//

#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include "../utils/tipos.h"
#include "../utils/socket.h"
#include "../utils/serializacion.h"

void sendStringOp(int serverId, std::string dato, e_operacion_filemanager op)
{
    std::vector<unsigned char> mensaje;
    std::vector<unsigned char> res;

    int size = dato.length() + 1;
    pack(mensaje, op);
    pack(mensaje, size);
    packv(mensaje, dato.c_str(), size);

    sendMSG(serverId, mensaje);
    recvMSG(serverId, res);

    int ok = unpack<int>(res);

    if (!ok)
    {
        std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
    }
}

std::string reciveStringOp(int serverId, e_operacion_filemanager op)
{
    std::string dato = "";

    std::vector<unsigned char> mensaje;
    std::vector<unsigned char> res;

    pack(mensaje, op);

    sendMSG(serverId, mensaje);
    recvMSG(serverId, res);

    int ok = unpack<int>(res);
    if (ok != 1)
    {
        std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
    }
    else
    {
        int size = unpack<int>(res);

        char *d = new char[size];
        unpackv<char>(res, d, size);
        dato = std::string(d);
        delete[] d;
    }

    return dato;
}

class FileManager_stub
{
private:
    std::string ip = "172.31.49.180";
    int puerto = 10001;
    connection_t serverConnection;
public:
    FileManager_stub(string path)
    {
        serverConnection = initClient(ip, puerto);

        sendStringOp(serverConnection.serverId, path, FL_CONSTRUCTOR);
    };

    ~FileManager_stub()
    {
        std::vector<unsigned char> mensaje;
        std::vector<unsigned char> res;
        
        pack(mensaje, FL_DESTRUCTOR);
        sendMSG(serverConnection.serverId, mensaje);
        recvMSG(serverConnection.serverId, res);
        
        int ok=unpack<int>(res);
        if(!ok)
        {
            std::cout<<"ERROR "<<__FILE__<<":"<<__LINE__<<" \n";
        }
        
        closeConnection(serverConnection.serverId);
    }

    vector<string *> *listFiles()
    {
        vector<string *> *flist = new vector<string *>();
        
        vector<unsigned char> mensaje;
        vector<unsigned char> res;

        pack(mensaje, FL_LISTFILES);
        sendMSG(serverConnection.serverId, mensaje);
        recvMSG(serverConnection.serverId, res);

        int ok = unpack<int>(res);
        if (ok != 1)
        {
            cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
        }
        else
        {
            deserializar_lista_ficheros(res, flist);
        }

        return flist;
    }

    void freeListedFiles(vector<string *> *fileList)
    {
        std::vector<unsigned char> mensaje;
        std::vector<unsigned char> res;

        pack(mensaje, FL_FREELISTEDFILES);
        serializar_lista_ficheros(mensaje, fileList);

        sendMSG(serverConnection.serverId, mensaje);
        recvMSG(serverConnection.serverId, res);

        int ok = unpack<int>(res);

        if (!ok)
        {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
        }
    }

    void readFile(char *fileName, char *&data, unsigned long int &dataLength)
    {
        std::vector<unsigned char> mensaje;
        std::vector<unsigned char> res;

        string file = new string(fileName);
        int size = file.length() + 1;
        pack(mensaje, FL_READFILE);
        pack(mensaje, size);
        packv(mensaje, file.c_str(), size);

        sendMSG(serverConnection.serverId, mensaje);
        recvMSG(serverConnection.serverId, res);

        int ok = unpack<int>(res);
        if (!ok)
        {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
        }
        else
        {
            int strSize = unpack<int>(res);
            dataLength = strSize;
            char *d = new char[size];
            unpackv<char>(res, d, size);
            data = d;
            delete[] d;
        }
    }
};
