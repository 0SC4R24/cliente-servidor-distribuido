//
// Created by adria on 31/10/2023.
//

#ifndef FM_CLIENT_PETICIONES_H
#define FM_CLIENT_PETICIONES_H

#include "../utils/socket.h"
#include "../utils/serializacion.h"
#include "../utils/tipos.h"

void preparar_y_enviar_registro_servidor_broker(int broker_id, std::vector<unsigned char> &packet, t_server &server)
{
    pack(packet, BK_SERVIDOR);
    serializar_server(packet, server);
    sendMSG(broker_id, packet);
}

void preparar_y_enviar_delete_servidor_broker(int broker_id, std::vector<unsigned char> &packet, t_server &server)
{
    pack(packet, BK_DELSERVIDOR);
    serializar_server(packet, server);
    sendMSG(broker_id, packet);
}

void prepara_y_envia_cliente_broker(int broker_id, std::vector<unsigned char> &packet, e_tipos_cliente cl_type)
{
    pack(packet, BK_CLIENTE);
    pack(packet, cl_type);
    sendMSG(broker_id, packet);
}

#endif //FM_CLIENT_PETICIONES_H
