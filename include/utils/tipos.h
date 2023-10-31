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
    MM_CREATERANDMATRIX = 7
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
    MM_INVALIDMATRIX = 9,
    MM_ERROR = 10,
} e_resultado_multmatrix;

typedef enum
{
    BK_CLIENTE = 1,
    BK_SERVIDOR = 2,
    BK_DELSERVIDOR = 3
} e_tipos_broker;

typedef enum
{
    BK_OK = 1,
    BK_ERROR = 2,
    BK_NOSERVERAVAILABLE = 3,
    BK_NOSERVERFOUND = 4
} e_resultado_broker;

typedef enum
{
    CL_MULTMATRIX = 1,
    CL_FILEMANAGER = 2
} e_tipos_cliente;

typedef enum
{
    SV_MULTMATRIX = 1,
    SV_FILEMANAGER = 2,
    SV_BOTH = 3,
    SV_NONE = 4
} e_tipos_server;

typedef struct
{
    char *ipaddr;
    int ipaddr_len;
    int port;
    int server_id;
} t_server;

#endif //MULTMATRIX_TIPOS_H


//
// Created by Oscar on 31/10/2023.
//

#ifndef FILEMANAGER_TIPOS_H
#define FILEMANAGER_TIPOS_H

typedef enum
{
    FL_CONSTRUCTOR = 1
} e_operacion_filemanager;

#endif //FILEMANAGER_TIPOS_H