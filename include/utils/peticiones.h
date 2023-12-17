//
// Created by adria on 31/10/2023.
//

#ifndef FM_CLIENT_PETICIONES_H
#define FM_CLIENT_PETICIONES_H

#define IPBROKER "172.31.84.232"
#define IPBROKERPORT 31000

#include "../utils/socket.h"
#include "../utils/serializacion.h"
#include "../utils/tipos.h"


inline void preparar_y_enviar_registro_servidor_broker(int broker_id, std::vector<unsigned char>& packet, t_server& server)
{
    pack(packet, BK_SERVIDOR);
    serializar_server(packet, server);
    sendMSG(broker_id, packet);
}

inline void preparar_y_enviar_delete_servidor_broker(int broker_id, std::vector<unsigned char>& packet, t_server& server)
{
    pack(packet, BK_DELSERVIDOR);
    serializar_server(packet, server);
    sendMSG(broker_id, packet);
}

inline void prepara_y_envia_cliente_broker(int broker_id, std::vector<unsigned char>& packet, e_tipos_cliente cl_type)
{
    pack(packet, BK_CLIENTE);
    pack(packet, cl_type);
    sendMSG(broker_id, packet);
}

inline bool registrar_servidor_en_broker(t_server& servidor)
{
    // Registrar el servidor en el broker
    std::cout << "IP DEL BROKER: " << IPBROKER << std::endl;

    std::vector<unsigned char> packet_in, packet_out;
    connection_t broker = initClient(IPBROKER, IPBROKERPORT);

    // Enviar datos del servidor al broker
    preparar_y_enviar_registro_servidor_broker(broker.serverId, packet_out, servidor);

    // Recibir respuesta del broker
    recvMSG(broker.serverId, packet_in);
    if (unpack<e_resultado_broker>(packet_in) != BK_OK)
    {
        std::cout << "UTILS: No se ha podido registrar el servidor en el broker. Cerrando..." << std::endl;
        return false;
    }

    // Cerrar la conexion con el broker
    std::cout << "UTILS: Servidor registrado en el broker. Continuando..." << std::endl;
    std::cout << "UTILS: Terminando registro en el broker. Cerrando conexion..." << std::endl;
    closeConnection(broker.serverId);

    return true;
}

inline void eliminar_servidor_en_broker(t_server& servidor)
{
    // Registrar el servidor en el broker
    std::vector<unsigned char> packet_in, packet_out;
    connection_t broker = initClient(IPBROKER, IPBROKERPORT);

    // Enviar datos del servidor al broker
    preparar_y_enviar_delete_servidor_broker(broker.serverId, packet_out, servidor);

    // Recibir respuesta del broker
    recvMSG(broker.serverId, packet_in);
    switch (unpack<e_resultado_broker>(packet_in))
    {
    case BK_OK:
        std::cout << "UTILS: Servidor eliminado del broker. Cerrando..." << std::endl;
        break;

    case BK_NOSERVERFOUND:
        std::cout << "UTILS: Servidor no registrado en el broker. No se puede eliminar. Cerrando..." << std::endl;
        break;

    case BK_ERROR:
        std::cout << "UTILS: Error al eliminar el servidor del broker. Cerrando..." << std::endl;
        break;

    default:
        std::cout << "UTILS: Enviado tipo de servidor incorrecto. Cerrando..." << std::endl;
        break;
    }

    // Cerrar la conexion con el broker
    closeConnection(broker.serverId);
}

inline t_server* pedir_servidor_a_broker(e_tipos_cliente tipo)
{
    // Definir conexion con el broker
    connection_t broker = initClient(IPBROKER, IPBROKERPORT);

    // Definir paquetes
    std::vector<unsigned char> packet_out, packet_in;

    // Iniciar conexion con el broker
    prepara_y_envia_cliente_broker(broker.serverId, packet_out, tipo);

    // Recibir respuesta del broker y cerrar la conexion
    recvMSG(broker.serverId, packet_in);

    // Procesar la respuesta del broker
    int exit_code = 0;
    switch (unpack<e_resultado_broker>(packet_in))
    {
    case BK_OK:
        std::cout << "UTILS: Conectado con el broker. Continuando..." << std::endl;
        break;

    case BK_WAIT:
        // Esperar a que haya un servidor disponible
        std::cout << "UTILS: No hay servidores disponibles. Esperando..." << std::endl;
        recvMSG(broker.serverId, packet_in);
        std::cout << "UTILS: Servidor disponible. Continuando..." << std::endl;
        break;

    case BK_NOSERVERAVAILABLE:
        std::cout << "UTILS: No hay servidores disponibles. Terminando..." << std::endl;
        exit_code = 2;
        break;

    default:
        std::cout << "UTILS: La operacion enviada no se reconoce. Terminando..." << std::endl;
        exit_code = 3;
        break;
    }

    // Cerrar la conexion con el broker
    closeConnection(broker.serverId);

    // Salir si no se ha podido conectar con el broker
    if (exit_code) exit(exit_code);

    // Procesar los datos del servidor
    return deserializar_server(packet_in);
}

#endif //FM_CLIENT_PETICIONES_H
