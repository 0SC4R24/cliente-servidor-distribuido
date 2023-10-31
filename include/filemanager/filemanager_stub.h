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

void sendStringOp(int serverId, std::string dato, e_operacion_filemanager op) {
    std::vector<unsigned char> mensaje;
    std::vector<unsigned char> res;

    int size = dato.length() + 1;
    pack(mensaje, op);
    pack(mensaje, size);
    packv(mensaje, dato.c_str(), size);

    sendMSG(serverId, mensaje);
    recvMSG(serverId, res);

    int ok = unpack<int>(res);

    if (!ok) {
        std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
    }
}

std::string reciveStringOp(int serverId, e_operacion_filemanager op) {
    std::string dato = "";

    std::vector<unsigned char> mensaje;
    std::vector<unsigned char> res;

    pack(mensaje, op);

    sendMSG(serverId, mensaje);
    recvMSG(serverId, res);
    
    int ok = unpack<int>(res);
    if (ok != 1) {
        std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << "\n";
    } else {
        int size = unpack<int>(res);
        dato.resize(size);

        char* d = new char[size];
        unpackv<char>(res, d, size);
        dato = std::string(d);
        delete[] d;
    }

    return dato;
}

class FileManager_stub {
    private:
        std::string ip = "172.31.63.230";
        int puerto = 10001;
        connection_t serverConnection;
    public:
        FileManager_stub() {
            serverConnection = initClient(ip, puerto);
            
            std::vector<unsigned char> mensaje;
            std::vector<unsigned char> res;

            pack(mensaje, FL_CONSTRUCTOR);

            sendMSG(serverConnection.serverId, mensaje);
            recvMSG(serverConnection.serverId, res);

            int ok = unpack<int>(res);

            if (!ok) {
                std::cout << "ERROR " << __FILE__ << ":" << __LINE__ << " error instanciando FileManager\n";
            }
        };
};