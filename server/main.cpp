
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/expressions.hpp>
#include <thread>
#include "include/Server.h"
#include "include/Consumer.h"
#include "include/Producer.h"
#include "include/EventQueue.h"
using namespace std;
namespace logging = boost::log;

void init_logger(std::shared_ptr<ConfigParser> &config) {
    logging::add_file_log(
            config->get("LOGGING", "file"),
            logging::keywords::format = "[%TimeStamp(format=\"%Y-%m-%d %H:%M:%S\")%]: [%Severity%] %Message%",
            logging::keywords::auto_flush = true
    );
    logging::core::get()->set_filter(logging::trivial::severity >= std::stoi(config->get("LOGGING", "level")));
    logging::add_common_attributes();
}

void read(std::shared_ptr<EventQueue> &eventQueue, std::shared_ptr<Consumer> &consumer){
    consumer->declare_queue();
    while(true){
        std::string msg = consumer->read_queue();
        if(!msg.empty()){
            eventQueue->push(msg);
        }else{
            return;
        }
    }
}

void send(std::shared_ptr<EventQueue> &eventQueue, std::shared_ptr<Producer> &producer)
{
    while (true) {
        std::string event = eventQueue->pop();
        if (!event.empty()) {
            producer->send_response(event);
        }
        else {
            return;
        }
    }

}

int main() {
    std::shared_ptr<ConfigParser> config = std::make_shared<ConfigParser>();
    init_logger(config);
    BOOST_LOG_TRIVIAL(info) << "Program start";
    std::shared_ptr<Consumer> consumer = std::make_shared<Consumer>(config);
    std::shared_ptr<Producer> producer = std::make_shared<Producer>(config);
    std::shared_ptr<EventQueue> eventQueue = std::make_shared<EventQueue>();
    std::thread sendMsg(send, std::ref(eventQueue), std::ref(producer));
    read(std::ref(eventQueue), std::ref(consumer));
    sendMsg.join();
    return 0;
    
}
