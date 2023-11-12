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
#include "../utils/peticiones.h"

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

        char* d = new char[size];
        unpackv<char>(res, d, size);
        dato = std::string(d);
        delete[] d;
    }

    return dato;
}

class FileManager_stub
{
private:
    connection_t serverConnection = {};

public:
    FileManager_stub(string path)
    {
        // Procesar los datos del servidor
        t_server* servidor_solicitado = pedir_servidor_a_broker(CL_FILEMANAGER);

        // Init de la conexion con la implementacion
        this->serverConnection = initClient(servidor_solicitado->ipaddr, servidor_solicitado->port);

        // Mostar mensaje de conexion y datos del servidor
        std::cout << "========================================" << std::endl;
        std::cout << "FileManagerStub: Conectado con el servidor" << std::endl;
        std::cout << "   IP: " << servidor_solicitado->ipaddr << std::endl;
        std::cout << "   Port: " << servidor_solicitado->port << std::endl;
        std::cout << "========================================" << std::endl;

        // Enviar operacion al servidor
        sendStringOp(serverConnection.serverId, path, FL_CONSTRUCTOR);

        // Liberar memoria
        delete[] servidor_solicitado->ipaddr;
        delete servidor_solicitado;
    };

    ~FileManager_stub()
    {
        std::vector<unsigned char> mensaje;
        std::vector<unsigned char> res;

        pack(mensaje, FL_DESTRUCTOR);
        sendMSG(serverConnection.serverId, mensaje);
        recvMSG(serverConnection.serverId, res);

        int ok = unpack<int>(res);
        if (!ok)
        {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << " \n";
        }

        closeConnection(serverConnection.serverId);
    }

    vector<string *>* listFiles()
    {
        vector<string *>* flist = new vector<string *>();

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

    void freeListedFiles(vector<string *>* fileList)
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

    void readFile(char* fileName, char*&data, unsigned long int&dataLength)
    {
        std::vector<unsigned char> mensaje;
        std::vector<unsigned char> res;

        string file = string(fileName);
        int size = file.length() + 1;
        pack(mensaje, FL_READFILE);
        pack(mensaje, size);
        packv(mensaje, fileName, size);

        sendMSG(serverConnection.serverId, mensaje);
        recvMSG(serverConnection.serverId, res);

        int ok = unpack<int>(res);
        if (!ok)
        {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
        }
        else
        {
            unsigned long int strSize = unpack<unsigned long int>(res);
            dataLength = strSize;
            data = new char[dataLength];

            unpackv<char>(res, data, dataLength);
        }
    }

    void writeFile(char* fileName, char* data, unsigned long int dataLength)
    {
        std::vector<unsigned char> mensaje;
        std::vector<unsigned char> res;

        string file = string(fileName);
        int size = file.length() + 1;
        pack(mensaje, FL_WRITEFILE);
        pack(mensaje, size);
        packv(mensaje, fileName, size);
        pack(mensaje, dataLength);
        packv(mensaje, data, dataLength);

        sendMSG(serverConnection.serverId, mensaje);
        recvMSG(serverConnection.serverId, res);

        int ok = unpack<int>(res);
        if (!ok)
        {
            std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
        }
    }
};
