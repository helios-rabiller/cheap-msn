cmake_minimum_required(VERSION 3.10)
project(ServerMySQL)

set(CMAKE_CXX_STANDARD 17)

set(MYSQL_INCLUDE_DIR "C:/Program Files/MySQL/MySQL Connector C++ 9.6/include")
set(MYSQL_LIB_DIR "C:/Program Files/MySQL/MySQL Connector C++ 9.6/lib64/vs14")

if(EXISTS "${MYSQL_INCLUDE_DIR}")
    message(STATUS "MySQL Include trouvé: ${MYSQL_INCLUDE_DIR}")
    include_directories(${MYSQL_INCLUDE_DIR})
else()
    message(FATAL_ERROR "MySQL Include non trouvé: ${MYSQL_INCLUDE_DIR}")
endif()

if(EXISTS "${MYSQL_LIB_DIR}")
    message(STATUS "MySQL Lib trouvé: ${MYSQL_LIB_DIR}")
    link_directories(${MYSQL_LIB_DIR})
else()
    message(FATAL_ERROR "MySQL Lib non trouvé: ${MYSQL_LIB_DIR}")
endif()

execute_process(
    COMMAND cmd /c "dir \"${MYSQL_LIB_DIR}\\*.lib\" /B"
    OUTPUT_VARIABLE MYSQL_LIBS
    OUTPUT_STRIP_TRAILING_WHITESPACE
)
message(STATUS "Bibliothèques disponibles: ${MYSQL_LIBS}")

add_executable(ServerMySQL server.cpp)

target_link_libraries(ServerMySQL 
    ws2_32
    mysqlcppconn 
)

add_custom_command(TARGET ServerMySQL POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "C:/Program Files/MySQL/MySQL Connector C++ 9.6/lib64/mysqlcppconn-10-vs14.dll"
        $<TARGET_FILE_DIR:ServerMySQL>
    COMMENT "Copie de la DLL MySQL"
)