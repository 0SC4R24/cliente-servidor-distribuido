cmake_minimum_required(VERSION 2.8.11)

project(filemanager)
add_executable(filemanager
        source/filemanager/main_fm.cpp
        source/filemanager/filemanager.cpp
        include/filemanager/filemanager.h
        include/utils/socket.h
        source/utils/socket.cpp
)

project(multmatrix)
add_executable(multmatrix
        source/multmatix/main_multMatrix.cpp
        source/multmatix/multmatrix.cpp
        include/multmatix/multmatrix.h
        include/utils/socket.h
        source/utils/socket.cpp
)
