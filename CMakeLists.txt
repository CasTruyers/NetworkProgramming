cmake_minimum_required (VERSION 3.4)
project(NP)

add_subdirectory(pybind11)
add_compile_definitions(ZMQ_STATIC)

set (CMAKE_CXX_STANDARD 23)

include_directories(
	libraries/include/
	pybind11/include/
)

link_directories(
	libraries/lib/
)

set(server_headers
	connectFour.hpp
)

set(client_headers
	connectFour.hpp
)

set(server_sources
	server.cpp
	connectFour.cpp
)

set(client_sources
	client.cpp
	connectFour.cpp
)

set(ALL_LIBS
	libzmq.a
)

add_executable (server ${server_sources} ${server_headers})
add_executable (client ${client_sources} ${client_headers})
pybind11_add_module(module_name pyApi.cpp ${client_sources} ${client_headers})

target_link_libraries(module_name PRIVATE ${ALL_LIBS})
target_link_libraries(server ${ALL_LIBS})
target_link_libraries(client ${ALL_LIBS})
