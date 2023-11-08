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
    std::cout << std::endl << "BROKER: Terminando instancia del broker. Adios..." << std::endl;
}

void notificar_cliente(int client_id, std::list<t_server *> *servidores)
{
    // Esperar hasta que haya servidores disponibles
    while (servidores->empty()) usleep(2000);

    // Variables
    std::vector<unsigned char> packet_out;
    t_server *server_info = servidores->front();
    servidores->pop_front();

    // Enviar informacion del servidor
    serializar_server(packet_out, *server_info);

    // Meter el servidor al final de la lista
    servidores->push_back(server_info);

    // Mostrar informacion del cliente y del servidor
    std::cout << "BROKER: Cliente conectado" << std::endl;
    std::cout << "BROKER: Servidor asignado:" << std::endl;
    std::cout << "BROKER: IP: " << server_info->ipaddr << std::endl;
    std::cout << "BROKER: Puerto: " << server_info->port << std::endl;
    std::cout << "BROKER: Tipo: " << "__THREAD__" << std::endl;

    // Enviar informacion del servidor
    sendMSG(client_id, packet_out);
}

int main(int argc, char **argv)
{
    // Manejo de señales para cerrar la conexion
    signal(SIGINT, sigstop);

    int socket = initServer(10000);
    std::map<e_tipos_server, std::list<t_server *>> servidores;

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
                    e_tipos_server tipo_servidor_solicitado = SV_NONE;

                    // Desempaquetar el servicio
                    switch (unpack<e_tipos_cliente>(packet_in))
                    {
                        case CL_MULTMATRIX:
                            tipo_servidor_solicitado = SV_MULTMATRIX;
                            break;

                        case CL_FILEMANAGER:
                            tipo_servidor_solicitado = SV_FILEMANAGER;
                            break;

                        default:
                            std::cout << "BROKER: Tipo de cliente desconocido. Ignorando..." << std::endl;
                            break;
                    }

                    // Enviar informacion del servidor si esta disponible
                    // Comprobar si hay servidores disponibles
                    if (tipo_servidor_solicitado == SV_NONE) pack(packet_out, BK_NOSERVERAVAILABLE);
                    else if (servidores[tipo_servidor_solicitado].empty())
                    {
                        std::cout << "BROKER: No hay servidores disponibles. Apuntando cliente en la cola..." << std::endl;
                        new std::thread(notificar_cliente, client_id, &servidores[tipo_servidor_solicitado]);
                        pack(packet_out, BK_WAIT);
                    }
                    else
                    {
                        // Seleccionar el primer servidor disponible
                        t_server *server_info = servidores[tipo_servidor_solicitado].front();
                        servidores[tipo_servidor_solicitado].pop_front();

                        // Enviar informacion del servidor
                        pack(packet_out, BK_OK);
                        serializar_server(packet_out, *server_info);

                        // Meter el servidor al final de la lista
                        servidores[tipo_servidor_solicitado].push_back(server_info);

                        // Mostrar informacion del cliente y del servidor
                        std::cout << "BROKER: Cliente conectado" << std::endl;
                        std::cout << "BROKER: Servidor asignado:" << std::endl;
                        std::cout << "BROKER: IP: " << server_info->ipaddr << std::endl;
                        std::cout << "BROKER: Puerto: " << server_info->port << std::endl;
                        std::cout << "BROKER: Tipo: " << tipo_servidor_solicitado << std::endl;
                    }
                }
                    break;

                case BK_SERVIDOR:
                {
                    // Desempaquetar los datos del servidor
                    t_server *datos_servidor_solicitado = deserializar_server_con_id(packet_in, client_id);
                    e_resultado_broker resultado = BK_ERROR;

                    // Guardar informacion del servidor
                    switch (datos_servidor_solicitado->type)
                    {
                        case SV_MULTMATRIX:
                        case SV_FILEMANAGER:
                            servidores[datos_servidor_solicitado->type].push_back(datos_servidor_solicitado);
                            resultado = BK_OK;
                            break;

                        case SV_BOTH:
                            // Se guarda la misma referencia en ambas listas
                            // Luego solo se eliminara de una de ellas
                            // Da igual en que lista este, se eliminara de ambas ya que es la misma referencia
                            // TODO: Revisar que si elimino una referencia de una lista, se elimina de la otra
                            servidores[SV_MULTMATRIX].push_back(datos_servidor_solicitado);
                            servidores[SV_FILEMANAGER].push_back(datos_servidor_solicitado);
                            resultado = BK_OK;
                            break;

                        default:
                            std::cout << "BROKER: Tipo de servidor desconocido. Ignorando..." << std::endl;
                            break;
                    }

                    // Empaquetar el resultado
                    pack(packet_out, resultado);

                    // Mostrar informacion del servidor
                    if (resultado == BK_OK)
                    {
                        std::cout << "BROKER: Servidor conectado" << std::endl;
                        std::cout << "BROKER: Servidor guardado:" << std::endl;
                        std::cout << "BROKER: IP: " << datos_servidor_solicitado->ipaddr << std::endl;
                        std::cout << "BROKER: Puerto: " << datos_servidor_solicitado->port << std::endl;
                        std::cout << "BROKER: Tipo: " << datos_servidor_solicitado->type << std::endl;
                    }
                    else std::cout << "BROKER: Servidor no guardado. Ignorando..." << std::endl;
                }
                    break;

                case BK_DELSERVIDOR:
                {
                    // Desempaquetar los datos del servidor
                    t_server *datos_servidor_eliminar = deserializar_server_con_id(packet_in, client_id);
                    e_resultado_broker resultado = BK_ERROR;

                    // Eliminar el servidor
                    switch (datos_servidor_eliminar->type)
                    {
                        case SV_BOTH:
                            datos_servidor_eliminar->type = SV_MULTMATRIX; // Da igual en que lista este, se eliminara de ambas, ya que es la misma referencia

                        case SV_MULTMATRIX:
                        case SV_FILEMANAGER:
                        {
                            for (auto &server: servidores[datos_servidor_eliminar->type])
                            {
                                if (!strcmp(datos_servidor_eliminar->ipaddr, server->ipaddr) && datos_servidor_eliminar->port == server->port)
                                {
                                    // Eliminar el servidor
                                    servidores[datos_servidor_eliminar->type].remove(server);
                                    delete[] server->ipaddr;
                                    delete server;

                                    // Guardar estado de la peticion
                                    resultado = BK_OK;
                                    break;
                                }
                            }
                        }
                            break;

                        default:
                            std::cout << "BROKER: Tipo de servidor desconocido. Ignorando..." << std::endl;
                            break;
                    }

                    // Empaquetar el resultado
                    pack(packet_out, resultado);

                    // Comprobar si se ha eliminado el servidor y mostrar los datos
                    if (resultado == BK_OK)
                    {
                        std::cout << "BROKER: Servidor eliminado:" << std::endl;
                        std::cout << "BROKER: IP: " << datos_servidor_eliminar->ipaddr << std::endl;
                        std::cout << "BROKER: Puerto: " << datos_servidor_eliminar->port << std::endl;
                        std::cout << "BROKER: Tipo: " << datos_servidor_eliminar->type << std::endl;
                    }
                    else std::cout << "BROKER: Servidor no encontrado" << std::endl;

                    // Liberar memoria
                    delete[] datos_servidor_eliminar->ipaddr;
                    delete datos_servidor_eliminar;
                }
                    break;

                default:
                    std::cout << "BROKER: Tipos de broker desconocido. Ignorando..." << std::endl;
                    pack(packet_out, BK_ERROR);
                    break;
            }

            // Enviar confirmacion
            sendMSG(client_id, packet_out);
        }
    }

    // Liberar memoria
    for (auto &server: servidores)
    {
        for (auto &server_info: server.second) delete[] server_info->ipaddr;
        server.second.clear();
    }
    servidores.clear();

    // Cierre de la conexion
    close(socket);
}