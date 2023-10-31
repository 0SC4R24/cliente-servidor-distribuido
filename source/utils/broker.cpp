//
// Created by adria on 30/10/2023.
//

#include <signal.h>
#include <thread>
#include <map>
#include <list>

#include "../../include/utils/socket.h"
#include "../../include/utils/tipos.h"
#include "../../include/utils/serializacion.h"

bool RUNNING = true;

void sigstop(int signal)
{
    // Terminar el bucle principal
    RUNNING = false;

    // Mostrar mensaje de cierre
    std::cout << "BROKER: Cerrando servidor" << std::endl;
}

int main(int argc, char **argv)
{
    // Manejo de señales para cerrar la conexion
    signal(SIGINT, sigstop);

    int server_socket = initServer(10002);
    std::map<e_tipos_server, std::list<t_server *>> servers;

    // Bucle principal
    while (RUNNING)
    {
        if (!checkClient()) usleep(1000);
        else
        {
            // Variables
            int client_id = getLastClientID();

            // Definir paquetes
            std::vector<unsigned char> packet_in, packet_out;

            // Recibir paquete y desempaquetar la operation
            recvMSG(client_id, packet_in);
            switch (unpack<e_tipos_broker>(packet_in))
            {
                case BK_CLIENTE:
                {
                    e_tipos_server server = SV_NONE;

                    // Desempaquetar el servicio
                    switch (unpack<e_tipos_cliente>(packet_in))
                    {
                        case CL_MULTMATRIX:
                            server = SV_MULTMATRIX;
                            break;

                        case CL_FILEMANAGER:
                            server = SV_FILEMANAGER;
                            break;

                        default:
                            std::cout << "BROKER: Recibido paquete de cliente desconocido" << std::endl;
                            break;
                    }

                    // Enviar informacion del servidor
                    // Comprobar si hay servidores disponibles
                    if (servers[server].empty()) pack(packet_out, BK_NOSERVERAVAILABLE);
                    else
                    {
                        // Seleccionar el primer servidor disponible
                        t_server *server_info = servers[server].front();
                        servers[server].pop_front();

                        // Enviar informacion del servidor
                        pack(packet_out, BK_OK);
                        pack(packet_out, server_info->ipaddr_len);
                        packv(packet_out, server_info->ipaddr, server_info->ipaddr_len);
                        pack(packet_out, server_info->port);

                        // Meter el servidor al final de la lista
                        servers[server].push_back(server_info);

                        // Mostrar informacion del cliente y del servidor
                        std::cout << "BROKER: Cliente conectado" << std::endl;
                        std::cout << "BROKER: IP: " << server_info->ipaddr << std::endl;
                        std::cout << "BROKER: Puerto: " << server_info->port << std::endl;
                        std::cout << "BROKER: Tipo: " << server << std::endl;
                    }

                    // Enviar confirmacion
                    sendMSG(client_id, packet_out);
                }
                    break;

                case BK_SERVIDOR:
                {
                    int port = 0, ipaddr_len = 0;
                    char *ipaddr = nullptr;
                    e_tipos_server type = SV_NONE;

                    // Desempaquetar los datos del servidor
                    ipaddr_len = unpack<int>(packet_in);
                    ipaddr = new char[ipaddr_len];
                    unpackv(packet_in, ipaddr, ipaddr_len);
                    port = unpack<int>(packet_in);
                    type = unpack<e_tipos_server>(packet_in);

                    // Guardar informacion del servidor
                    switch (type)
                    {
                        case SV_MULTMATRIX:
                        case SV_FILEMANAGER:
                            servers[type].push_back(new t_server{ipaddr, ipaddr_len, port, client_id});
                            pack(packet_out, BK_OK);
                            break;

                        case SV_BOTH:
                            servers[SV_MULTMATRIX].push_back(new t_server{ipaddr, ipaddr_len, port, client_id});
                            servers[SV_FILEMANAGER].push_back(new t_server{ipaddr, ipaddr_len, port, client_id});
                            pack(packet_out, BK_OK);
                            break;

                        default:
                            std::cout << "BROKER: Recibido paquete de servidor desconocido" << std::endl;
                            pack(packet_out, BK_ERROR);
                            break;
                    }

                    // Enviar confirmacion
                    sendMSG(client_id, packet_out);

                    // Mostrar informacion del servidor
                    std::cout << "BROKER: Servidor registrado" << std::endl;
                    std::cout << "BROKER: IP: " << ipaddr << std::endl;
                    std::cout << "BROKER: Puerto: " << port << std::endl;
                    std::cout << "BROKER: Tipo: " << type << std::endl;
                }
                    break;

                default:
                    std::cout << "BROKER: Recibido paquete desconocido" << std::endl;
                    break;
            }
        }
    }

    // Liberar memoria
    for (auto &server: servers)
    {
        for (auto &server_info: server.second) delete[] server_info->ipaddr;
        server.second.clear();
    }
    servers.clear();

    // Cierre de la conexion
    close(server_socket);
}