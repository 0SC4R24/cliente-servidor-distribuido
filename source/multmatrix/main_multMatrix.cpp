#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "../../include/multmatrix/multmatrix_stub.h"

void freeMatrix(matrix_t *m)
{
    delete[] m->data;
    delete[] m;

}

int main()
{
    // Crear stub
    auto *mmatrix = new MultmatrixStub();

    // Crear matrices
    matrix_t *m1 = mmatrix->createRandMatrix(5, 5);
    matrix_t *m2 = mmatrix->createIdentity(5, 5);
    matrix_t *mres = mmatrix->multMatrices(m1, m2);

    // Escribir matrices
    mmatrix->writeMatrix(mres, "resultado.txt");

    // Leer matrices
    matrix_t *m3 = mmatrix->readMatrix("resultado.txt");

    // Multiplicar matrices y escribir resultado
    matrix_t *mres2 = mmatrix->multMatrices(m1, m3);
    mmatrix->writeMatrix(mres2, "resultado2.txt");

    // Mostar matrices
    std::cout << "Matriz 1:" << std::endl;
    MultmatrixStub::print_matrix(m1);
    MultmatrixStub::print_matrix(m2);
    MultmatrixStub::print_matrix(mres);
    MultmatrixStub::print_matrix(m3);
    MultmatrixStub::print_matrix(mres2);

    // Liberar memoria
    freeMatrix(m1);
    freeMatrix(m2);
    freeMatrix(mres);
    freeMatrix(m3);
    freeMatrix(mres2);
    delete mmatrix;

}
