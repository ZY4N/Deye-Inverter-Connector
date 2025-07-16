cmake_minimum_required(VERSION 3.18)

project(deye_simple_example_project)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -Werror -Ofast")

set(DEYE_LIB_PATH "../../lib")
add_executable(deye_simple_example main.cpp ${DEYE_LIB_PATH}/asio_tcp_socket.cpp
        ../../lib/deye_config.hpp
        ../../lib/deye_config.hpp
)
target_include_directories(deye_simple_example PRIVATE ${DEYE_LIB_PATH})

find_package(Boost REQUIRED COMPONENTS system)
set(BOOST_ENABLE_CMAKE ON)
include_directories(asio INTERFACE ${boost_asio_SOURCE_DIR}/include)
target_link_libraries(deye_simple_example PRIVATE Boost::system)

