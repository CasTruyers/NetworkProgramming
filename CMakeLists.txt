cmake_minimum_required (VERSION 3.18.3)

project(NP)

add_compile_definitions(ZMQ_STATIC)

set (CMAKE_CXX_STANDARD 23)

include_directories(
	libraries/include/
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

target_link_libraries(server ${ALL_LIBS})
target_link_libraries(client ${ALL_LIBS})