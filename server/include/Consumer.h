

#ifndef RABBITMQ_SERVER_CONSUMER_H
#define RABBITMQ_SERVER_CONSUMER_H
#include "Server.h"
#include "Producer.h"
class Consumer {
public:
    explicit Consumer(std::shared_ptr<ConfigParser> &config);
    void declare_queue() const;
    std::string read_queue() const;
private:
    std::shared_ptr<Server> server;
    std::string queueName;
};


#endif //RABBITMQ_SERVER_CONSUMER_H
