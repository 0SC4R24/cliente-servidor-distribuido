//
// Created by adria on 29/10/2023.
//

#ifndef MULTMATRIX_TIPOS_H
#define MULTMATRIX_TIPOS_H

typedef enum
{
    MM_CONSTRUCTOR = 1,
    MM_DESTRUCTOR = 2,
    MM_READMATRIX = 3,
    MM_MULTMATRIX = 4,
    MM_WRITEMATRIX = 5,
    MM_CREATEIDENTITY = 6,
    MM_CREATERANDMATRIX = 7,
    MM_INVALID = 8
} e_operacion_multmatrix;

typedef enum
{
    MM_OK = 1,
    MM_NOCONSTRUCTOR = 2,
    MM_NODESTRUCTOR = 3,
    MM_NOREADMATRIX = 4,
    MM_NOMULTMATRIX = 5,
    MM_NOWRITEMATRIX = 6,
    MM_NOCREATEIDENTITY = 7,
    MM_NOCREATERANDMATRIX = 8,
    MM_ERROR = 9
} e_resultado_multmatrix;

#endif //MULTMATRIX_TIPOS_H
