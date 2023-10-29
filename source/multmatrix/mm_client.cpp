//
// Created by adria on 29/10/2023.
//

#include "../../include/multmatrix/multmatrix_stub.h"

int main(int argc, char **argv)
{
    auto *mm_stub = new MultmatrixStub();

    mm_stub->create_rand(5, 5);
    mm_stub->create_identity(5, 5);
    mm_stub->mult_matrix();

    delete mm_stub;
}