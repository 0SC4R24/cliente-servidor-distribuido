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
#include "../utils/peticiones.h"

class MultmatrixStub
{
private:
    // Definir conexion con la implementacion
    connection_t server = {};

    // Definir ip y port de la conexion
    std::string ipaddr = "127.0.0.1";
    int ipport = 10002;

public:
    MultmatrixStub()
    {
        // Definir conexion con el broker
        this->server = initClient(this->ipaddr, this->ipport);

        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Iniciar conexion con el broker
        prepara_y_envia_cliente_broker(this->server.serverId, packet_out, CL_MULTMATRIX);

        // Recibir respuesta del broker y cerrar la conexion
        recvMSG(this->server.serverId, packet_in);

        // Procesar la respuesta del broker
        int exit_code = 0;
        switch (unpack<e_resultado_broker>(packet_in))
        {
            case BK_OK:
                std::cout << "MultmatrixStub: Conectado con el broker. Continuando..." << std::endl;
                break;

            case BK_WAIT:
                // Esperar a que haya un servidor disponible
                std::cout << "MultmatrixStub: No hay servidores disponibles. Esperando..." << std::endl;
                recvMSG(this->server.serverId, packet_in);
                std::cout << "MultmatrixStub: Servidor disponible. Continuando..." << std::endl;
                break;

            case BK_NOSERVERAVAILABLE:
                std::cout << "MultmatrixStub: No hay servidores disponibles. Terminando..." << std::endl;
                exit_code = 2;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce. Terminando..." << std::endl;
                exit_code = 3;
                break;
        }

        // Cerrar la conexion con el broker
        closeConnection(this->server.serverId);

        // Salir si no se ha podido conectar con el broker
        if (exit_code) exit(exit_code);

        // Procesar los datos del servidor
        t_server *servidor_solicitado = deserializar_server(packet_in);

        // Init de la conexion con la implementacion
        this->server = initClient(servidor_solicitado->ipaddr, servidor_solicitado->port);

        // Mostar mensaje de conexion y datos del servidor
        std::cout << "MultmatrixStub: Conectado con el servidor" << std::endl;
        std::cout << "MultmatrixStub: IP: " << servidor_solicitado->ipaddr << std::endl;
        std::cout << "MultmatrixStub: Port: " << servidor_solicitado->port << std::endl;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_CONSTRUCTOR);
        sendMSG(this->server.serverId, packet_out);
        recvMSG(this->server.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                std::cout << "MultmatrixStub: Objeto inicializado correctamente. Continuando..." << std::endl;
                break;

            case MM_NOCONSTRUCTOR:
                std::cout << "MultmatrixStub: Ya se ha inicializado un objeto. Continuando..." << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce. Ignorando..." << std::endl;
                break;
        }

        // Liberar memoria
        delete[] servidor_solicitado->ipaddr;
        delete servidor_solicitado;
    }

    ~MultmatrixStub()
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_DESTRUCTOR);
        sendMSG(this->server.serverId, packet_out);
        recvMSG(this->server.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                std::cout << "MultmatrixStub: Objeto destruido correctamente. Cerrando conexion..." << std::endl;
                break;

            case MM_NODESTRUCTOR:
                std::cout << "MultmatrixStub: No se ha destruido un objeto. Cerrando conexion..." << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce. Cerrando conexion..." << std::endl;
                break;
        }

        closeConnection(this->server.serverId);
    }

    matrix_t *createRandMatrix(int rows, int cols) const
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;
        matrix_t *matrix = new matrix_t;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_CREATERANDMATRIX);
        pack(packet_out, rows);
        pack(packet_out, cols);
        sendMSG(this->server.serverId, packet_out);
        recvMSG(this->server.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                // Desempaquetar matrix
                deserializar_matrix(packet_in, matrix);

                std::cout << "MultmatrixStub: Matriz aleatoria creada correctamente. Continuando..." << std::endl;
                break;

            case MM_NOCREATERANDMATRIX:
                std::cout << "MultmatrixStub: No se ha creado la matriz aleatoria. Continuando..." << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce. Ignorando..." << std::endl;
                break;
        }

        return matrix;
    }

    matrix_t *createIdentity(int rows, int cols) const
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;
        matrix_t *matrix = new matrix_t;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_CREATEIDENTITY);
        pack(packet_out, rows);
        pack(packet_out, cols);
        sendMSG(this->server.serverId, packet_out);
        recvMSG(this->server.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                // Desempaquetar matrix
                deserializar_matrix(packet_in, matrix);

                std::cout << "MultmatrixStub: Matriz identidad creada correctamente. Continuando..." << std::endl;
                break;

            case MM_NOCREATEIDENTITY:
                std::cout << "MultmatrixStub: No se ha creado la matriz identidad. Continuando..." << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce. Ignorando..." << std::endl;
                break;
        }

        return matrix;
    }

    matrix_t *multMatrices(matrix_t *matrix_1, matrix_t *matrix_2) const
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;
        matrix_t *matrix = new matrix_t;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_MULTMATRIX);
        serializar_matrix(packet_out, matrix_1);
        serializar_matrix(packet_out, matrix_2);
        sendMSG(this->server.serverId, packet_out);
        recvMSG(this->server.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                // Desempaquetar matrix
                deserializar_matrix(packet_in, matrix);

                std::cout << "MultmatrixStub: Matrices multiplicadas correctamente. Continuando..." << std::endl;
                break;

            case MM_NOMULTMATRIX:
                std::cout << "MultmatrixStub: No se han multiplicado las matrices. Continuando..." << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce. Ignorando..." << std::endl;
                break;
        }

        return matrix;
    }

    void writeMatrix(matrix_t *matrix, std::string file)
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_WRITEMATRIX);
        serializar_char_array(packet_out, file.c_str(), (int) file.length() + 1);
        serializar_matrix(packet_out, matrix);
        sendMSG(this->server.serverId, packet_out);
        recvMSG(this->server.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                std::cout << "MultmatrixStub: Matriz escrita correctamente. Continuando..." << std::endl;
                break;

            case MM_NOWRITEMATRIX:
                std::cout << "MultmatrixStub: No se ha escrito la matriz. Continuando..." << std::endl;
                break;

            case MM_INVALIDMATRIX:
                std::cout << "MultmatrixStub: La matriz seleccionada no es valida. Continuando..." << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce. Ignorando..." << std::endl;
                break;
        }
    }

    matrix_t *readMatrix(std::string file)
    {
        // Definir paquetes
        std::vector<unsigned char> packet_out, packet_in;
        matrix_t *matrix = new matrix_t;

        // Envio de operacion y recepcion de packet_in
        pack(packet_out, MM_READMATRIX);
        serializar_char_array(packet_out, file.c_str(), (int) file.length() + 1);
        sendMSG(this->server.serverId, packet_out);
        recvMSG(this->server.serverId, packet_in);

        // Analizar la packet_in
        switch (unpack<e_resultado_multmatrix>(packet_in))
        {
            case MM_OK:
                // Desempaquetar matrix
                deserializar_matrix(packet_in, matrix);

                std::cout << "MultmatrixStub: Matriz leida correctamente. Continuando..." << std::endl;
                break;

            case MM_NOREADMATRIX:
                std::cout << "MultmatrixStub: No se ha leido la matriz. Continuando..." << std::endl;
                break;

            case MM_INVALIDMATRIX:
                std::cout << "MultmatrixStub: La matriz seleccionada no es valida. Continuando..." << std::endl;
                break;

            default:
                std::cout << "MultmatrixStub: La operacion enviada no se reconoce. Ignorando..." << std::endl;
                break;
        }

        return matrix;
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
