
#include "../include/EventQueue.h"

void EventQueue::push(const std::string &event) {
    std::unique_lock<std::mutex> lock(mutex);
    queue.push(event);
    cv.notify_one();
}

std::string EventQueue::pop() {
    std::unique_lock<std::mutex> lock(mutex);
    if (cv.wait_for(lock, std::chrono::seconds(10), [this] { return !queue.empty(); })) {
        std::string event = queue.front();
        queue.pop();
        return event;
    }
    return "";
}
