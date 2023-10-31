#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "../../include/utils/socket.h"
#include "../../include/utils/serializacion.h"
#include "../../include/utils/tipos.h"
#include "../../include/filemanager/filemanager_imp.h"
#include <thread>

void atiendeCliente(int clientID)
{
    FileManager_imp *fm = new FileManager_imp();

    while (!fm->conexionCerrada)
    {
        fm->recOp(clientID);
    }
}

int main(int argc, char **argv)
{
    int serverSocket = initServer(10001);
    int clientID = -1;

    while (1)
    {
        if (!checkClient())
        {
            usleep(1000);
        }
        else
        {
            std::thread *t = new std::thread(atiendeCliente, getLastClientID());
        }
    }

    close(serverSocket);
    return 0;
}