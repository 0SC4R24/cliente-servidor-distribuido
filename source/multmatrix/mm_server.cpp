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
    // Manejo de señales para cerrar la conexion
    signal(SIGINT, sigstop);

    // Inicializacion del servidor
    int server_socket = initServer(10001);

    // Bucle principal
    while (RUNNING)
    {
        if (!checkClient()) usleep(1000);
        else
        {
            auto *thread = new std::thread(atiende_cliente, getLastClientID());
        }
    }

    // Cierre de la conexion
    close(server_socket);
}
