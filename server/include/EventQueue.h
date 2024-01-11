
#ifndef RABBITMQ_SERVER_EVENTQUEUE_H
#define RABBITMQ_SERVER_EVENTQUEUE_H
#include <iostream>
#include <memory>
#include <condition_variable>
#include <queue>
#include <mutex>

class EventQueue {
private:
    std::queue<std::string> queue;
    std::mutex mutex;
    std::condition_variable cv;

public:
    void push(const std::string &event);
    std::string pop();
};

#endif //RABBITMQ_SERVER_EVENTQUEUE_H
