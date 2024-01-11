
#include "../include/Producer.h"
#include "../cmake-build-debug/protocol.pb.h"
#include <boost/log/trivial.hpp>

Producer::Producer(std::shared_ptr<ConfigParser> &config) {
    server = std::make_shared<Server>(config);
    exchangeName = config->get("EXCHANGE", "name");
    type = config->get("EXCHANGE", "type");
}

void Producer::send_response(const std::string &msg) {

    declare_exchange();
    std::string response = make_response(msg);
    declare_queue();
    binding_queue();
    amqp_connection_state_t conn = server->getConn();
    amqp_basic_publish(conn, 1, amqp_cstring_bytes(exchangeName.c_str()),
                       amqp_cstring_bytes(queueName.c_str()), 0, 0,
                       nullptr, amqp_cstring_bytes(response.c_str()));
    amqp_rpc_reply_t r = amqp_get_rpc_reply(conn);
    if (AMQP_RESPONSE_NORMAL != r.reply_type) {
        BOOST_LOG_TRIVIAL(error) << "Failed to publish message. Reply type: " << r.reply_type;
    } else {
        BOOST_LOG_TRIVIAL(info) << "Message is published";
    }
}


std::string Producer::make_response(const std::string &msg) {
    try {
        auto request = std::make_shared<TestTask::Messages::Request>();
        auto response = std::make_shared<TestTask::Messages::Response>();
        request->ParseFromString(msg);
        response->set_id(request->id());
        response->set_res(2 * request->req());
        BOOST_LOG_TRIVIAL(info) << response->res();
        std::string ret = response->SerializeAsString();
        queueName = request->id();
        return ret;
    }
    catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Error in make_response function: " << e.what();
        return "";
    }
}

void Producer::declare_exchange() {
    amqp_exchange_declare(server->getConn(), 1, amqp_cstring_bytes(exchangeName.c_str()),
                          amqp_cstring_bytes(type.c_str()), 0, 0, 0, 0, amqp_empty_table);
    amqp_rpc_reply_t ret = amqp_get_rpc_reply(server->getConn());
    if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
        BOOST_LOG_TRIVIAL(error) << "Error exchange declare" << ret.reply_type;
    } else {
        BOOST_LOG_TRIVIAL(info) << "Declare exchange done";
    }
}

void Producer::declare_queue() {
    amqp_queue_declare(server->getConn(), 1, amqp_cstring_bytes(queueName.c_str()), 0, 0, 0, 0, amqp_empty_table);
    amqp_rpc_reply_t ret = amqp_get_rpc_reply(server->getConn());
    if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
        BOOST_LOG_TRIVIAL(error) << "Producer: Error queue declare " << ret.reply_type;
    } else {
        BOOST_LOG_TRIVIAL(info) << "Producer: Declare queue done";
    }
}

void Producer::binding_queue() {
    amqp_queue_bind(server->getConn(), 1, amqp_cstring_bytes(queueName.c_str()),
                    amqp_cstring_bytes(exchangeName.c_str()), amqp_cstring_bytes(queueName.c_str()),
                    amqp_empty_table);
    amqp_rpc_reply_t ret = amqp_get_rpc_reply(server->getConn());
    if (AMQP_RESPONSE_NORMAL != ret.reply_type) {

        BOOST_LOG_TRIVIAL(error) << "Error bind " << ret.reply_type;
    } else {
        BOOST_LOG_TRIVIAL(info) << "Bind done";
    }
}
