cmake_minimum_required(VERSION 3.18)

project(deye_influxdb_example_project)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "-std=c++20 -Wall -Wextra -Werror -Ofast")

set(DEYE_LIB_PATH "../../lib")
add_executable(deye_influxdb_example main.cpp ${DEYE_LIB_PATH}/asio_tcp_socket.cpp)
target_include_directories(deye_influxdb_example PRIVATE ${DEYE_LIB_PATH})

find_package(InfluxDB)
find_package(Boost REQUIRED COMPONENTS system)

set(BOOST_ENABLE_CMAKE ON)
include_directories(asio INTERFACE ${boost_asio_SOURCE_DIR}/include)

# Influx-cxx sometimes failes to set the input path.
# You might have to manually find the include directory and modify the next line.
target_include_directories(deye_influxdb_example PRIVATE "/usr/include/influxdb-cxx")

target_link_libraries(deye_influxdb_example PRIVATE
	InfluxData::InfluxDB
	Boost::system
)

get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
foreach(dir ${dirs})
  message(STATUS "dir='${dir}'")
endforeach()