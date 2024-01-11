
#ifndef RABBITMQ_SERVER_PRODUCER_H
#define RABBITMQ_SERVER_PRODUCER_H
#include "Server.h"
class Producer {
public:
    explicit Producer(std::shared_ptr<ConfigParser> &config);
    void send_response(const std::string &msg);
private:
    void declare_exchange();
    void declare_queue();
    void binding_queue();
    std::string make_response(const std::string &msg);
    std::shared_ptr<Server> server;
    std::string exchangeName;
    std::string type;
    std::string queueName;
};


#endif //RABBITMQ_SERVER_PRODUCER_H
