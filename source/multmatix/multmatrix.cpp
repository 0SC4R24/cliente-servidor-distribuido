#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <cstring>
#include <sys/types.h>
#include <unistd.h>

#include "../../include/multmatix/multmatrix.h"

/**
 * @brief multMatrix::multMatrix Constructor de la clase multMatrix.
 * No tiene parámetros y sólo inicializa el generador de números aleatorios
 */
multMatrix::multMatrix()
{
    srand(getpid());
}

/**
 * @brief multMatrix::readMatrix Método que lee un fichero de disco duro y devuelve un objeto "matrix_t". En caso de no
 * existir el fichero, devolverá NULL. El fichero se encuetra en formato texto, la primera línea contiene el número de
 * filas y columnas, y las siguientes líneas son los datos (un dato por línea)
 * @param fileName Nombre del fichero
 * @return En caso de éxito, retorna una estructura de tipo "matrix_t" con los datos del fichero. En otro caso retornará NULL
 */
matrix_t *multMatrix::readMatrix(const char *fileName)
{
    FILE *f = fopen(fileName, "r");
    if (f == 0)
    {
        std::cout << "ERROR: Fichero " << std::string(fileName) << " no existe\n";
        return NULL;
    }
    matrix_t *matrix = new matrix_t[1];

    fscanf(f, "%d %d", &matrix->rows, &matrix->cols);
    std::cout << "Leidos fila y columna: " << matrix->rows << " " << matrix->cols << "\n";
    matrix->data = new int[matrix->rows * matrix->cols];
    for (int i = 0; i < matrix->rows * matrix->cols; i++)
    {
        fscanf(f, "%d", &matrix->data[i]);
    }

    fclose(f);
    return matrix;
}

/**
 * @brief multMatrix::multMatrices Método que implementa la multiplicación de matrices.
 * Recibe dos matrices de tipo matrix_t y devuelve otra matriz matrix_t con el resultado
 * de la multiplicación
 * @param m1 Primera matriz en la multiplicación
 * @param m2 Segunda matriz en la multiplicación
 * @return En caso de éxito, retorna una estructura de tipo matrix_t, con el resultado de la multiplicación.
 *  NULL en otro caso.
 */
matrix_t *multMatrix::multMatrices(matrix_t *m1, matrix_t *m2)
{

    if (m1->cols != m2->rows)
    {
        std::cout << "ERROR: Matrices no compatibles\n";
        return NULL;
    }
    matrix_t *mres = new matrix_t[1];
    mres->rows = m1->rows;
    mres->cols = m2->cols;
    mres->data = new int[mres->rows * mres->cols];
    for (int i = 0; i < m1->rows; i++)//each row m1
    {
        for (int j = 0; j < m2->cols; j++) //each col m2
        {
            mres->data[i * mres->cols + j] = 0;

            for (int k = 0; k < m1->cols; k++) //dot mult m1 row by m2 col
            {
                mres->data[i * mres->cols + j] += m1->data[i * m1->cols + k] * m2->data[k * m2->cols + j];
            }
        }
    }

    return mres;
}

/**
 * @brief multMatrix::writeMatrix Método que escribe a fichero el contenido de la matriz pasada por parámetros
 * @param m Matriz cuyos datos se escribirán en fichero
 * @param fileName Nombre del fichero a generar
 */
void multMatrix::writeMatrix(matrix_t *m, const char *fileName)
{
    FILE *f = fopen(fileName, "w");
    fprintf(f, "%d %d\n", m->rows, m->cols);

    for (int i = 0; i < m->rows * m->cols; i++)
    {
        fprintf(f, "%d\n", m->data[i]);
    }
    fclose(f);
}

/**
 * @brief createRandMatrix Método que crea una matriz con datos random de un tamaño dado
 * @param rows Número de filas
 * @param cols Número de columnas
 * @return Una estructura de tipo "matrix_t" con los datos rellenados
 */

matrix_t *multMatrix::createRandMatrix(int rows, int cols)
{

    matrix_t *m = new matrix_t[1];
    m->rows = rows;
    m->cols = cols;
    m->data = new int[rows * cols];
    for (int i = 0; i < rows * cols; i++)
        m->data[i] = rand();

    return m;
}

/**
 * @brief createIdentity Método que crea una matriz identidad de un tamaño dado
 * @param rows Número de filas
 * @param cols Número de columnas
 * @return Una estructura de tipo "matrix_t" con los datos rellenados
 */
matrix_t *multMatrix::createIdentity(int rows, int cols)
{
    matrix_t *m = new matrix_t[1];
    m->rows = rows;
    m->cols = cols;
    m->data = new int[rows * cols];
    memset(m->data, 0, sizeof(int) * rows * cols);
    for (int i = 0; i < rows; i++)
        m->data[i * cols + i] = 1;
    return m;
}


/**
 * @brief multMatrix::~multMatrix Destructor de la clase, no tiene nada ahora mismo
 */
multMatrix::~multMatrix()
{
    //empty destructor
}

