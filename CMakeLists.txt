cmake_minimum_required(VERSION 2.8.11)

project(filemanager LANGUAGES CXX)
add_executable(
        filemanager
        source/filemanager/main_fm.cpp
        source/filemanager/filemanager.cpp
)

project(multmatrix LANGUAGES CXX)
add_executable(
        multmatrix
        source/multmatrix/main_multMatrix.cpp
        source/multmatrix/multmatrix.cpp
)

project(multmatrix_server LANGUAGES CXX)
add_executable(
        multmatrix_server
        source/multmatrix/mm_server.cpp
        source/multmatrix/multmatrix.cpp
        source/utils/socket.cpp
        include/multmatrix/multmatrix_imp.h
)

project(multmatrix_client LANGUAGES CXX)
add_executable(
        multmatrix_client
        source/multmatrix/mm_client.cpp
        source/utils/socket.cpp
        include/multmatrix/multmatrix_stub.h
)

