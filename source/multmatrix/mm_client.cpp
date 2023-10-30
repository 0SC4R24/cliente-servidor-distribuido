//
// Created by adria on 29/10/2023.
//

#include "../../include/multmatrix/multmatrix_stub.h"

int main(int argc, char **argv)
{
    auto *mm_stub = new MultmatrixStub();

    // Definir matrices
    matrix_t *m1 = new matrix_t, *m2 = new matrix_t, *m3 = new matrix_t;
    mm_stub->create_identity(5, 5, m1);
    mm_stub->create_rand(5, 5, m2);
    mm_stub->mult_matrix(m1, m2, m3);

    // Mostrar matrices
    MultmatrixStub::print_matrix(m1);
    MultmatrixStub::print_matrix(m2);
    MultmatrixStub::print_matrix(m3);

    // Guardar matrices en el servidor
    mm_stub->write_matrix("m1.txt", m1);
    mm_stub->write_matrix("m2.txt", m2);
    mm_stub->write_matrix("m3.txt", m3);

    // Leer las matrices del servidor
    auto *m4 = new matrix_t, *m5 = new matrix_t, *m6 = new matrix_t;
    mm_stub->read_matrix("m1.txt", m4);
    mm_stub->read_matrix("m2.txt", m5);
    mm_stub->read_matrix("m3.txt", m6);

    // Mostrar matrices
    MultmatrixStub::print_matrix(m4);
    MultmatrixStub::print_matrix(m5);
    MultmatrixStub::print_matrix(m6);

    delete mm_stub;
}