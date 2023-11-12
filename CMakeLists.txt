cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXE_LINKER_FLAGS "-static")

project(broker LANGUAGES CXX)
add_executable(
        broker
        source/utils/broker.cpp
        source/utils/socket.cpp
)
target_link_libraries(broker pthread)

project(mm_server LANGUAGES CXX)
add_executable(
        mm_server
        source/multmatrix/mm_server.cpp
        source/multmatrix/multmatrix.cpp
        source/utils/socket.cpp
        include/multmatrix/multmatrix_imp.h
)
target_link_libraries(mm_server pthread)

project(mm_client LANGUAGES CXX)
add_executable(
        mm_client
        source/multmatrix/main_multMatrix.cpp
        source/utils/socket.cpp
        include/multmatrix/multmatrix_stub.h
)
target_link_libraries(mm_client pthread)

project(fm_server LANGUAGES CXX)
add_executable(
        fm_server
        source/filemanager/fm_server.cpp
        source/filemanager/filemanager.cpp
        source/utils/socket.cpp
        include/filemanager/filemanager_imp.h
)
target_link_libraries(fm_server pthread)

project(fm_client LANGUAGES CXX)
add_executable(
        fm_client
        source/filemanager/main_fm.cpp
        source/filemanager/filemanager.cpp
        source/utils/socket.cpp
        include/filemanager/filemanager_stub.h
)
target_link_libraries(fm_client pthread)
