#include <iostream>
#include <thread>
#include <signal.h>

#include "../../include/utils/socket.h"
#include "../../include/utils/tipos.h"
#include "../../include/filemanager/filemanager_imp.h"
#include "../../include/utils/peticiones.h"


bool RUNNING = true;

void atiende_cliente(int clientID)
{
    FileManager_imp* fm_imp = new FileManager_imp();

    while (!fm_imp->conexionCerrada) fm_imp->recOp(clientID);

    delete fm_imp;
}

void sigstop(int signal)
{
    // Terminar el bucle principal
    RUNNING = false;

    // Mostrar mensaje de cierre
    std::cout << std::endl << "FM_Server: Terminando instancia del servidor. Adios..." << std::endl;
}

int main(int argc, char** argv)
{
    // Variables del servidor
    std::string ipaddr = "172.31.84.232";
    int ipport = 10002, ipportkubectl = 30002;

    // Inicializacion del servidor
    int socket = initServer(ipport);
    std::cout << "FM_Server: Creando instancia del servidor. Iniciando..." << std::endl;

    // Manejo de señales para cerrar la conexion
    signal(SIGINT, sigstop);

    // Crear la estructura de datos del servidor
    t_server* server = new t_server;
    server->ipaddr_len = (int)ipaddr.length() + 1;
    server->ipaddr = (char*)ipaddr.c_str();
    server->port = ipportkubectl;
    server->type = SV_FILEMANAGER;

    // Registrar en broker
    if (!registrar_servidor_en_broker(*server)) return 1;

    // Bucle principal
    while (RUNNING)
    {
        if (!checkClient()) usleep(1000);
        else
        {
            // Crear un nuevo thread para atender al cliente
            std::cout << "FM_Server: Nuevo cliente conectado. Creando thread para atenderlo..." << std::endl;
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
