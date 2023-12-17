//
// Created by adria on 29/10/2023.
//

#include <signal.h>
#include <thread>

#include "../../include/utils/peticiones.h"
#include "../../include/multmatrix/multmatrix_imp.h"

bool RUNNING = true;

void atiende_cliente(int cliente_id)
{
    MultmatrixImp* mm_imp = new MultmatrixImp();

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

int main(int argc, char** argv)
{
    // Variables del servidor
    std::string ipaddr = "172.31.84.232";
    int ipport = 10001, ipportkubectl = 30001;

    // Inicializacion del servidor
    int socket = initServer(ipport);
    std::cout << "MM_Server: Creando instancia del servidor. Iniciando..." << std::endl;

    // Manejo de señales para cerrar la conexion
    signal(SIGINT, sigstop);

    // Crear la estructura de datos del servidor
    t_server* server = new t_server;
    server->ipaddr_len = (int)ipaddr.length() + 1;
    server->ipaddr = (char*)ipaddr.c_str();
    server->port = ipportkubectl;
    server->type = SV_MULTMATRIX;

    // Registrar en broker
    if (!registrar_servidor_en_broker(*server)) return 1;

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
    eliminar_servidor_en_broker(*server);

    // Liberar memoria
    delete server;

    // Cierre de la conexion
    close(socket);
}
