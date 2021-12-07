cmake_minimum_required(VERSION 3.1)
project(MiddlewareLoader "CXX")
set(CMAKE_CXX_STANDARD 17)

file(GLOB sourcesServer "${CMAKE_CURRENT_SOURCE_DIR}/src/server/*.cpp")
file(GLOB sourcesClient "${CMAKE_CURRENT_SOURCE_DIR}/src/client/*.cpp")
file(GLOB library "${CMAKE_CURRENT_SOURCE_DIR}/include/*.cpp")
file(GLOB libraryTcp "${CMAKE_CURRENT_SOURCE_DIR}/include/Tcp/*.cpp")

link_libraries(ws2_32)

#Criando target com codigo src a ser compilado
add_executable(Server ${sourcesServer} ${library} ${libraryTcp})
add_executable(Client ${sourcesClient} ${library} ${libraryTcp})
#target_include_directories(MiddlewareLoader PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Server PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories(Client PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

#set_target_properties(MiddlewareLoader 
#    PROPERTIES
#    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
#    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
#    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
#)

set_target_properties(Client 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Client/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Client/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Client/bin
)

set_target_properties(Server 
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Server/lib
    LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Server/lib
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Server/bin
)