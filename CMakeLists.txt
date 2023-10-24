cmake_minimum_required(VERSION 2.8.11)

project(filemanager)
add_executable(filemanager source/filemanager/main_fm.cpp source/filemanager/filemanager.cpp headers/filemanager/filemanager.h)

project(multmatrix)
add_executable(multmatrix source/multmatix/main_multMatrix.cpp source/multmatix/multmatrix.cpp headers/multmatix/multmatrix.h)
