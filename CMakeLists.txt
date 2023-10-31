cmake_minimum_required(VERSION 2.8.11)

project(filemanager LANGUAGES CXX)
add_executable(
        filemanager
        source/filemanager/main_fm.cpp
        source/filemanager/filemanager.cpp
)
target_link_libraries(filemanager pthread)

project(multmatrix LANGUAGES CXX)
add_executable(
        multmatrix
        source/multmatrix/main_multMatrix.cpp
        source/multmatrix/multmatrix.cpp
)
target_link_libraries(multmatrix pthread)

project(broker LANGUAGES CXX)
add_executable(
        broker
        source/utils/broker.cpp
        source/utils/socket.cpp
)
target_link_libraries(broker pthread)

project(multmatrix_server LANGUAGES CXX)
add_executable(
        multmatrix_server
        source/multmatrix/mm_server.cpp
        source/multmatrix/multmatrix.cpp
        source/utils/socket.cpp
        include/multmatrix/multmatrix_imp.h
)
target_link_libraries(multmatrix_server pthread)

project(multmatrix_client LANGUAGES CXX)
add_executable(
        multmatrix_client
        source/multmatrix/mm_client.cpp
        source/utils/socket.cpp
        include/multmatrix/multmatrix_stub.h
)
target_link_libraries(multmatrix_client pthread)

project(filemanager_server LANGUAGES CXX)
add_executable(
        filemanager_server
        source/filemanager/fm_server.cpp
        source/filemanager/filemanager.cpp
        source/utils/socket.cpp
        include/filemanager/filemanager_imp.h
)
target_link_libraries(filemanager_server pthread)

project(filemanager_client LANGUAGES CXX)
add_executable(
        filemanager_client
        source/filemanager/fm_client.cpp
        source/utils/socket.cpp
        include/filemanager/filemanager_stub.h
)
target_link_libraries(filemanager_client pthread)
