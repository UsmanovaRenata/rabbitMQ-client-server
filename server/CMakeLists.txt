cmake_minimum_required(VERSION 3.26)
project(RabbitMq_server)

set(CMAKE_CXX_STANDARD 17)
find_package(Boost REQUIRED COMPONENTS log log_setup )

include_directories(/usr/local/include ${Boost_INCLUDE_DIRS} ${Protobuf_INCLUDE_DIRS})
find_package(Protobuf REQUIRED)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS protocol.proto)

add_executable(rabbitmq_server main.cpp
        ${PROTO_HDRS}
        ${PROTO_SRCS}
        src/Server.cpp
        include/Server.h
        include/ConfigParser.h
        src/Consumer.cpp
        include/Consumer.h
        src/Producer.cpp
        include/Producer.h
        src/EventQueue.cpp
        include/EventQueue.h)

target_link_libraries(rabbitmq_server /usr/local/lib/librabbitmq.so ${Boost_LIBRARIES} ${Protobuf_LIBRARIES})
