set(DEYE_LIB_PATH "../../../lib")

idf_component_register(
	SRCS
        "main.cpp"
		"wifi_client_handler.cpp"
		"${DEYE_LIB_PATH}/lwip_tcp_socket.cpp"


	INCLUDE_DIRS
		"./"
		${DEYE_LIB_PATH}
)

target_compile_options(${COMPONENT_LIB} PRIVATE -std=c++20)
