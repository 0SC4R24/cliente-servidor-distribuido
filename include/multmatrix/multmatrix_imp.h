//
// Created by adria on 29/10/2023.
//

#ifndef MULTMATRIX_MULTMATRIX_IMP_H
#define MULTMATRIX_MULTMATRIX_IMP_H

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <vector>

#include "multmatrix.h"
#include "../utils/socket.h"
#include "../utils/serializacion.h"
#include "../utils/tipos.h"

class MultmatrixImp
{
private:
    multMatrix *multmatrix = nullptr;
    matrix_t *matrix_1 = nullptr, *matrix_2 = nullptr, *matrix_res = nullptr;

public:
    bool conexion_cerrada = false;

    void recibe_operacion(int client_id)
    {
        // Definir paquetes
        std::vector<unsigned char> packet_in, packet_out;

        // Recibir paquete y desempaquetar la operation
        recvMSG(client_id, packet_in);
        auto operation = unpack<e_operacion_multmatrix>(packet_in);

        switch (operation)
        {
            case MM_CONSTRUCTOR:
            {
                if (this->multmatrix == nullptr)
                {
                    this->multmatrix = new multMatrix();
                    pack<int>(packet_out, MM_OK);
                }
                else pack<int>(packet_out, MM_NOCONSTRUCTOR);
            }
                break;

            case MM_DESTRUCTOR:
            {
                if (this->multmatrix != nullptr)
                {
                    delete this->multmatrix;
                    this->multmatrix = nullptr;
                    this->conexion_cerrada = true;
                    pack<int>(packet_out, MM_OK);
                }
                else pack<int>(packet_out, MM_NODESTRUCTOR);

            }
                break;

            case MM_CREATERANDMATRIX:
            {
                if (this->multmatrix == nullptr) pack<int>(packet_out, MM_NOCREATERANDMATRIX);
                else
                {
                    this->matrix_1 = this->multmatrix->createRandMatrix(
                            unpack<int>(packet_in), // rows
                            unpack<int>(packet_in) // cols
                    );
                    pack<int>(packet_out, MM_OK);
                }
            }
                break;

            case MM_CREATEIDENTITY:
            {
                if (this->multmatrix == nullptr) pack<int>(packet_out, MM_NOCREATEIDENTITY);
                else
                {
                    this->matrix_2 = this->multmatrix->createIdentity(
                            unpack<int>(packet_in), // rows
                            unpack<int>(packet_in) // cols
                    );
                    pack<int>(packet_out, MM_OK);
                }
            }
                break;

            case MM_MULTMATRIX:
            {
                if (this->multmatrix == nullptr) pack<int>(packet_out, MM_NOMULTMATRIX);
                else
                {
                    this->matrix_res = this->multmatrix->multMatrices(this->matrix_1, this->matrix_2);
                    pack<int>(packet_out, MM_OK);
                }
            }
                break;

            default:
                std::cout << "MultmatrixImp::recibe_operacion: Operacion no valida" << std::endl;
                pack<int>(packet_out, MM_ERROR);
                break;
        }

        // Enviar paquete
        sendMSG(client_id, packet_out);
    }
};

#endif //MULTMATRIX_MULTMATRIX_IMP_H
