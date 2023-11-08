//
// Created by adria on 29/10/2023.
//

#include <signal.h>
#include <thread>

#include "../../include/utils/serializacion.h"
#include "../../include/utils/peticiones.h"
#include "../../include/multmatrix/multmatrix_imp.h"

bool RUNNING = true;

void atiende_cliente(int cliente_id)
{
    MultmatrixImp *mm_imp = new MultmatrixImp();

    while (!mm_imp->conexion_cerrada) mm_imp->recibe_operacion(cliente_id);

    delete mm_imp;
}

void sigstop(int signal)
{
    // Terminar el bucle principal
    RUNNING = false;

    // Mostrar mensaje de cierre
    std::cout << std::endl << "MM_Server: Terminando instancia del servidor. Adios..." << std::endl;
}

int main(int argc, char **argv)
{
    // Variables del servidor
    std::string ipaddr = "127.0.0.1", ipbroker = "127.0.0.1";
    int ipport = 10001, ipbrokerport = 10000;

    // Inicializacion del servidor
    int socket = initServer(ipport);
    std::cout << "MM_Server: Creando instancia del servidor. Iniciando..." << std::endl;

    // Manejo de señales para cerrar la conexion
    signal(SIGINT, sigstop);

    // Registrar el servidor en el broker
    std::vector<unsigned char> packet_in, packet_out;
    connection_t broker = initClient(ipbroker, ipbrokerport);

    // Crear la estructura de datos del servidor
    t_server *server = new t_server;
    server->ipaddr_len = (int) ipaddr.length() + 1;
    server->ipaddr = (char *) ipaddr.c_str();
    server->port = ipport;
    server->type = SV_MULTMATRIX;

    // Enviar datos del servidor al broker
    preparar_y_enviar_registro_servidor_broker(broker.serverId, packet_out, *server);

    // Recibir respuesta del broker
    recvMSG(broker.serverId, packet_in);
    if (unpack<e_resultado_broker>(packet_in) != BK_OK)
    {
        std::cout << "MM_Server: No se ha podido registrar el servidor en el broker. Cerrando..." << std::endl;
        return 1;
    }

    // Cerrar la conexion con el broker
    std::cout << "MM_Server: Servidor registrado en el broker. Continuando..." << std::endl;
    std::cout << "MM_Server: Terminando registro en el broker. Cerrando conexion..." << std::endl;
    closeConnection(broker.serverId);

    // Bucle principal
    while (RUNNING)
    {
        if (!checkClient()) usleep(1000);
        else
        {
            // Crear un nuevo thread para atender al cliente
            std::cout << "MM_Server: Nuevo cliente conectado. Creando thread para atenderlo..." << std::endl;
            new std::thread(atiende_cliente, getLastClientID());
        }
    }

    // Eliminar el servidor del broker
    broker = initClient(ipbroker, ipbrokerport);

    // Enviar datos del servidor al broker
    preparar_y_enviar_delete_servidor_broker(broker.serverId, packet_out, *server);

    // Recibir respuesta del broker
    recvMSG(broker.serverId, packet_in);
    switch (unpack<e_resultado_broker>(packet_in))
    {
        case BK_OK:
            std::cout << "MM_Server: Servidor eliminado del broker. Cerrando..." << std::endl;
            break;

        case BK_NOSERVERFOUND:
            std::cout << "MM_Server: Servidor no registrado en el broker. No se puede eliminar. Cerrando..." << std::endl;
            break;

        case BK_ERROR:
            std::cout << "MM_Server: Error al eliminar el servidor del broker. Cerrando..." << std::endl;
            break;

        default:
            std::cout << "MM_Server: Enviado tipo de servidor incorrecto. Cerrando..." << std::endl;
            break;
    }

    // Liberar memoria
    delete server;

    // Cierre de la conexion
    closeConnection(broker.serverId);
    close(socket);
}
