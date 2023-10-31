//
// Created by adria on 29/10/2023.
//

#pragma once
#ifndef MULTMATRIX_SERIALIZACION_H
#define MULTMATRIX_SERIALIZACION_H

#include <vector>

#include "tipos.h"
#include "../multmatrix/multmatrix.h"
#include "../filemanager/filemanager.h"

template<typename T>
inline void pack(std::vector<unsigned char> &packet, T data)
{
    auto *ptr = (unsigned char *) &data;
    int dsize = sizeof(T), init = packet.size();

    packet.resize(init + dsize);
    std::copy(ptr, ptr + dsize, packet.begin() + init);
}

template<typename T>
inline T unpack(std::vector<unsigned char> &packet)
{
    T *ptr = (T *) packet.data();
    T data = ptr[0];
    int dsize = sizeof(T), init = packet.size();

    for (int i = dsize; i < packet.size(); i++) packet[i - dsize] = packet[i];
    packet.resize(init - dsize);

    return data;
}

template<typename T>
inline void packv(std::vector<unsigned char> &packet, T *arr, int size)
{
    for (int i = 0; i < size; i++) pack(packet, arr[i]);
}

template<typename T>
inline void unpackv(std::vector<unsigned char> &packet, T *arr, int size)
{
    for (int i = 0; i < size; i++) arr[i] = unpack<T>(packet);
}

void serializar_matrix(std::vector<unsigned char> &packet, matrix_t *matrix)
{
    pack(packet, matrix->rows);
    pack(packet, matrix->cols);
    packv(packet, matrix->data, matrix->rows * matrix->cols);
}

void deserializar_matrix(std::vector<unsigned char> &packet, matrix_t *matrix)
{
    matrix->rows = unpack<int>(packet);
    matrix->cols = unpack<int>(packet);
    matrix->data = new int[matrix->rows * matrix->cols];
    unpackv(packet, matrix->data, matrix->rows * matrix->cols);
}

void serializar_char_array(std::vector<unsigned char> &packet, const char *arr, int size)
{
    pack(packet, size);
    packv(packet, arr, size);
}

void serializar_server(std::vector<unsigned char> &packet, t_server &server)
{
    serializar_char_array(packet, server.ipaddr, server.ipaddr_len);
    pack(packet, server.port);
    pack(packet, server.type);
}

t_server *deserializar_server(std::vector<unsigned char> &packet)
{
    t_server *server = new t_server;
    server->ipaddr_len = unpack<int>(packet);
    server->ipaddr = new char[server->ipaddr_len];
    unpackv(packet, server->ipaddr, server->ipaddr_len);
    server->port = unpack<int>(packet);
    server->type = unpack<e_tipos_server>(packet);
    return server;
}

t_server *deserializar_server_con_id(std::vector<unsigned char> &packet, int server_id)
{
    t_server *s = deserializar_server(packet);
    s->server_id = server_id;
    return s;
}

#endif //MULTMATRIX_SERIALIZACION_H
