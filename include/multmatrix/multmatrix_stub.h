//
// Created by adria on 29/10/2023.
//

#pragma once
#ifndef MULTMATRIX_MULTMATRIX_STUB_H
#define MULTMATRIX_MULTMATRIX_STUB_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "multmatrix.h"
#include "../utils/socket.h"
#include "../utils/serializacion.h"
#include "../utils/tipos.h"

class MultmatrixStub
{
private:
    // Definir conexion con la implementacion
    connection_t server_connection = {};

    // Definir ip y port de la conexion
    std::string ip = "172.28.197.211";
    int port = 10001;

public:
    MultmatrixStub()
    {
        // Definir conexion con la implementacion
        this->server_connection = initClient(this->ip, this->port);

        // Definir paquetes
        std::vector<unsigned char> mensaje, respuesta;

        // Envio de operacion y recepcion de respuesta
        pack(mensaje, MM_CONSTRUCTOR);
        sendMSG(this->server_connection.serverId, mensaje);
        recvMSG(this->server_connection.serverId, respuesta);

        // Analizar la respuesta
        switch (unpack<int>(respuesta))
        {
            case MM_OK:
                std::cout << "MultmatrixStub: Objeto inicializado correctamente" << std::endl;
                break;

            case MM_NOCONSTRUCTOR:
                std::cout << "MultmatrixStub: Ya se ha inicializado un objeto" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }

    ~MultmatrixStub()
    {
        // Definir paquetes
        std::vector<unsigned char> mensaje, respuesta;

        // Envio de operacion y recepcion de respuesta
        pack(mensaje, MM_DESTRUCTOR);
        sendMSG(this->server_connection.serverId, mensaje);
        recvMSG(this->server_connection.serverId, respuesta);

        // Analizar la respuesta
        switch (unpack<int>(respuesta))
        {
            case MM_OK:
                std::cout << "MultmatrixStub: Objeto destruido correctamente" << std::endl;
                break;

            case MM_NODESTRUCTOR:
                std::cout << "MultmatrixStub: No se ha destruido un objeto" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }

    void create_rand(int rows, int cols) const
    {
        // Definir paquetes
        std::vector<unsigned char> mensaje, respuesta;

        // Envio de operacion y recepcion de respuesta
        pack(mensaje, MM_CREATERANDMATRIX);
        pack(mensaje, rows);
        pack(mensaje, cols);
        sendMSG(this->server_connection.serverId, mensaje);
        recvMSG(this->server_connection.serverId, respuesta);

        // Analizar la respuesta
        switch (unpack<int>(respuesta))
        {
            case MM_OK:
                std::cout << "MultmatrixStub: Matriz aleatoria creada correctamente" << std::endl;
                break;

            case MM_NOCREATERANDMATRIX:
                std::cout << "MultmatrixStub: No se ha creado la matriz aleatoria" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }

    void create_identity(int rows, int cols) const
    {
        // Definir paquetes
        std::vector<unsigned char> mensaje, respuesta;

        // Envio de operacion y recepcion de respuesta
        pack(mensaje, MM_CREATEIDENTITY);
        pack(mensaje, rows);
        pack(mensaje, cols);
        sendMSG(this->server_connection.serverId, mensaje);
        recvMSG(this->server_connection.serverId, respuesta);

        // Analizar la respuesta
        switch (unpack<int>(respuesta))
        {
            case MM_OK:
                std::cout << "MultmatrixStub: Matriz identidad creada correctamente" << std::endl;
                break;

            case MM_NOCREATEIDENTITY:
                std::cout << "MultmatrixStub: No se ha creado la matriz identidad" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }

    void mult_matrix() const
    {
        // Definir paquetes
        std::vector<unsigned char> mensaje, respuesta;

        // Envio de operacion y recepcion de respuesta
        pack(mensaje, MM_MULTMATRIX);
        sendMSG(this->server_connection.serverId, mensaje);
        recvMSG(this->server_connection.serverId, respuesta);

        // Analizar la respuesta
        switch (unpack<int>(respuesta))
        {
            case MM_OK:
                std::cout << "MultmatrixStub: Matrices multiplicadas correctamente" << std::endl;
                break;

            case MM_NOMULTMATRIX:
                std::cout << "MultmatrixStub: No se han multiplicado las matrices" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }
};

#endif //MULTMATRIX_MULTMATRIX_STUB_H
