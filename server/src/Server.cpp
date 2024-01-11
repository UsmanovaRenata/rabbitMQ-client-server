#include <boost/log/trivial.hpp>
#include "../include/Server.h"

using namespace std;

Server::Server(std::shared_ptr<ConfigParser> &config) {
    hostname = config->get("SERVER", "host");
    port = std::stoi(config->get("SERVER", "port"));
    conn = amqp_new_connection();
    socket = amqp_tcp_socket_new(conn);
    BOOST_LOG_TRIVIAL(info) << "Server start";
    open_socket();
    open_channel();
}

void Server::open_socket() const {
    if (!socket) {
        BOOST_LOG_TRIVIAL(warning) << "Creating TCP socket";
    }
    int status = amqp_socket_open(socket, hostname.c_str(), port);
    if (status != AMQP_STATUS_OK) {
        BOOST_LOG_TRIVIAL(error) << "Error opening TCP socket. Status: " << status;
    }
    BOOST_LOG_TRIVIAL(info) << "Socket opened successfully";
}

void Server::open_channel() const {
    amqp_login(conn, "/", 0, 131072, 0, AMQP_SASL_METHOD_PLAIN, "guest", "guest");
    amqp_channel_open(conn, 1);
    amqp_rpc_reply_t r = amqp_get_rpc_reply(conn);
    if (AMQP_RESPONSE_NORMAL != r.reply_type) {
        BOOST_LOG_TRIVIAL(error) << "Error opening channel: " << r.reply_type;
    } else {
        BOOST_LOG_TRIVIAL(info) << "Channel opened successfully";
    }
}

const amqp_connection_state_t &Server::getConn() const {
    return conn;
}

Server::~Server() {
    try {
        amqp_channel_close(conn, 1, AMQP_REPLY_SUCCESS);
        amqp_connection_close(conn, AMQP_REPLY_SUCCESS);
        amqp_destroy_connection(conn);
        BOOST_LOG_TRIVIAL(info) << "Server closed successfully";
    } catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Error during server destruction: " << e.what();
    }
}


