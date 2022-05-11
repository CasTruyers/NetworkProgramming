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
)

set(client_headers
	connectFour.hpp
)

set(serverOOP_headers
	connectFour.hpp
)

set(main_headers
	connectFour.hpp
)

set(server_sources
	server.cpp
)

set(client_sources
	client.cpp
	connectFour.cpp
)

set(serverOOP_sources
	serverOOP.cpp
	connectFour.cpp
)

set(main_sources
	main.cpp
	connectFour.cpp
)

set(ALL_LIBS
	libzmq.a
)

add_executable (server ${server_sources} ${server_headers})
add_executable (client ${client_sources} ${client_headers})
add_executable (serverOOP ${serverOOP_sources} ${serverOOP_headers})
add_executable (main ${main_sources} ${main_headers})

target_link_libraries(server ${ALL_LIBS})
target_link_libraries(serverOOP ${ALL_LIBS})
target_link_libraries(client ${ALL_LIBS})
target_link_libraries(main ${ALL_LIBS})