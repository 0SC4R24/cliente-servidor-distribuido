//
// Created by adria on 29/10/2023.
//

#include <signal.h>
#include <thread>

#include "../../include/multmatrix/multmatrix_imp.h"

bool RUNNING = true;

void atiende_cliente(int cliente_id)
{
    auto *mm_imp = new MultmatrixImp();

    while (!mm_imp->conexion_cerrada) mm_imp->recibe_operacion(cliente_id);

    delete mm_imp;
}

void sigstop(int signal)
{
    // Terminar el bucle principal
    RUNNING = false;

    // Mostrar mensaje de cierre
    std::cout << "MM_Server: Cerrando servidor" << std::endl;
}

int main(int argc, char **argv)
{
    // Variables del servidor
    std::string ipaddr = "172.28.197.211", ipbroker = "172.28.197.211";
    int ipport = 10001;

    // Manejo de señales para cerrar la conexion
    signal(SIGINT, sigstop);

    // Registrar el servidor en el broker
    std::vector<unsigned char> packet_in, packet_out;
    connection_t conn_broker = initClient(ipbroker, 10002);

    // Enviar datos del servidor al broker
    pack(packet_out, BK_SERVIDOR);
    pack(packet_out, (int) ipaddr.length() + 1);
    packv(packet_out, ipaddr.c_str(), (int) ipaddr.length() + 1);
    pack(packet_out, ipport);
    pack(packet_out, SV_MULTMATRIX);
    sendMSG(conn_broker.serverId, packet_out);

    // Recibir respuesta del broker
    recvMSG(conn_broker.serverId, packet_in);
    if (unpack<e_resultado_broker>(packet_in) != BK_OK)
    {
        std::cout << "MM_Server: No se ha podido registrar el servidor en el broker. Cerrando servidor" << std::endl;
        return 1;
    }

    // Cerrar la conexion con el broker
    closeConnection(conn_broker.serverId);
    std::cout << "MM_Server: Servidor registrado en el broker" << std::endl;
    std::cout << "MM_Server: Conexion con el broker cerrada" << std::endl;

    // Inicializacion del servidor
    int server_socket = initServer(ipport);
    std::cout << "MM_Server: Servidor iniciado" << std::endl;

    // Bucle principal
    while (RUNNING)
    {
        if (!checkClient()) usleep(1000);
        else
        {
            // Crear un nuevo thread para atender al cliente
            std::cout << "MM_Server: Nuevo cliente conectado" << std::endl;
            auto *thread = new std::thread(atiende_cliente, getLastClientID());

            // Liberar memoria del thread
            thread->detach();
            delete thread;
        }
    }

    // Liberar memoria
    delete &ipaddr;
    delete &ipbroker;

    // Cierre de la conexion
    close(server_socket);
}
