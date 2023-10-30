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
    int port = 10002;

public:
    MultmatrixStub()
    {
        // Definir conexion con el broker
        this->server_connection = initClient(this->ip, this->port);

        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Iniciar conexion con el broker
        pack(packet_out, BK_CLIENTE);
        pack(packet_out, CL_MULTMATRIX);
        sendMSG(this->server_connection.serverId, packet_out);

        // Recibir respuesta del broker y cerrar la conexion
        recvMSG(this->server_connection.serverId, packet_in);
        closeConnection(this->server_connection.serverId);

        // Procesar la respuesta del broker
        if (unpack<e_resultado_broker>(packet_in) != BK_OK)
        {
            std::cout << "MultmatrixStub: No se ha podido conectar con el servidor" << std::endl;
            exit(1);
        }

        // Procesar los datos del servidor
        int ipaddr_len = unpack<int>(packet_in);
        char *ipaddr = new char[ipaddr_len];
        unpackv(packet_in, ipaddr, ipaddr_len);
        int ipport = unpack<int>(packet_in);

        // Init de la conexion con la implementacion
        this->server_connection = initClient(ipaddr, ipport);

        // Mostar mensaje de conexion y datos del servidor
        std::cout << "MultmatrixStub: Conectado con el servidor" << std::endl;
        std::cout << "MultmatrixStub: IP: " << ipaddr << std::endl;
        std::cout << "MultmatrixStub: Port: " << ipport << std::endl;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_CONSTRUCTOR);
        sendMSG(this->server_connection.serverId, packet_out);
        recvMSG(this->server_connection.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
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

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }

        // Liberar memoria
        delete[] ipaddr;
    }

    ~MultmatrixStub()
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_DESTRUCTOR);
        sendMSG(this->server_connection.serverId, packet_out);
        recvMSG(this->server_connection.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
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

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }

        closeConnection(this->server_connection.serverId);
    }

    void create_rand(int rows, int cols, matrix_t *matrix) const
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_CREATERANDMATRIX);
        pack(packet_out, rows);
        pack(packet_out, cols);
        sendMSG(this->server_connection.serverId, packet_out);
        recvMSG(this->server_connection.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                // Desempaquetar matrix
                deserializar_matrix(packet_in, matrix);

                std::cout << "MultmatrixStub: Matriz aleatoria creada correctamente" << std::endl;
                break;

            case MM_NOCREATERANDMATRIX:
                std::cout << "MultmatrixStub: No se ha creado la matriz aleatoria" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }

    void create_identity(int rows, int cols, matrix_t *matrix) const
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_CREATEIDENTITY);
        pack(packet_out, rows);
        pack(packet_out, cols);
        sendMSG(this->server_connection.serverId, packet_out);
        recvMSG(this->server_connection.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                // Desempaquetar matrix
                deserializar_matrix(packet_in, matrix);

                std::cout << "MultmatrixStub: Matriz identidad creada correctamente" << std::endl;
                break;

            case MM_NOCREATEIDENTITY:
                std::cout << "MultmatrixStub: No se ha creado la matriz identidad" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }

    void mult_matrix(matrix_t *matrix_1, matrix_t *matrix_2, matrix_t *matrix_res) const
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_MULTMATRIX);
        serializar_matrix(packet_out, matrix_1);
        serializar_matrix(packet_out, matrix_2);
        sendMSG(this->server_connection.serverId, packet_out);
        recvMSG(this->server_connection.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                // Desempaquetar matrix
                deserializar_matrix(packet_in, matrix_res);

                std::cout << "MultmatrixStub: Matrices multiplicadas correctamente" << std::endl;
                break;

            case MM_NOMULTMATRIX:
                std::cout << "MultmatrixStub: No se han multiplicado las matrices" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }

    void write_matrix(std::string file, matrix_t *matrix)
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_WRITEMATRIX);
        pack(packet_out, (int) file.length() + 1);
        packv(packet_out, file.c_str(), (int) file.length() + 1);
        serializar_matrix(packet_out, matrix);
        sendMSG(this->server_connection.serverId, packet_out);
        recvMSG(this->server_connection.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                std::cout << "MultmatrixStub: Matriz escrita correctamente" << std::endl;
                break;

            case MM_NOWRITEMATRIX:
                std::cout << "MultmatrixStub: No se ha escrito la matriz" << std::endl;
                break;

            case MM_INVALIDMATRIX:
                std::cout << "MultmatrixStub: La matriz seleccionada no es valida" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }

    void read_matrix(std::string file, matrix_t *matrix)
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_READMATRIX);
        pack(packet_out, (int) file.length() + 1);
        packv(packet_out, file.c_str(), (int) file.length() + 1);
        sendMSG(this->server_connection.serverId, packet_out);
        recvMSG(this->server_connection.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                // Desempaquetar matrix
                deserializar_matrix(packet_in, matrix);

                std::cout << "MultmatrixStub: Matriz leida correctamente" << std::endl;
                break;

            case MM_NOREADMATRIX:
                std::cout << "MultmatrixStub: No se ha leido la matriz" << std::endl;
                break;

            case MM_INVALIDMATRIX:
                std::cout << "MultmatrixStub: La matriz seleccionada no es valida" << std::endl;
                break;

            case MM_ERROR:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce" << std::endl;
                break;
        }
    }

    static void print_matrix(matrix_t *m)
    {
        for (int i = 0; i < m->rows; i++)
        {
            for (int j = 0; j < m->cols; j++) std::cout << m->data[i * m->cols + j] << " ";
            std::cout << std::endl;
        }
        std::cout << std::endl;
    }
};

#endif //MULTMATRIX_MULTMATRIX_STUB_H
