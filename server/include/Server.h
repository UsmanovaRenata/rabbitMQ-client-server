
#ifndef RABBITMQ_SERVER_SERVER_H
#define RABBITMQ_SERVER_SERVER_H
#include <rabbitmq-c/amqp.h>
#include <rabbitmq-c/tcp_socket.h>
#include "ConfigParser.h"

class Server {
public:
    explicit Server(std::shared_ptr<ConfigParser> &config);
    const amqp_connection_state_t& getConn() const;
    ~Server();

private:
    void open_socket() const;
    void open_channel() const;
    int port;
    std::string hostname;
    amqp_socket_t *socket;
    amqp_connection_state_t conn;
};


#endif //RABBITMQ_SERVER_SERVER_H
