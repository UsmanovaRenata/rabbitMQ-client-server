#include "../include/Consumer.h"
#include "../cmake-build-debug/protocol.pb.h"
#include <boost/log/trivial.hpp>
#include <string>

using namespace std;


Consumer::Consumer(std::shared_ptr<ConfigParser> &config) {
    server = std::make_shared<Server>(config);
    queueName = config->get("QUEUE", "name");

}

std::string Consumer::read_queue() const {
    try {
        amqp_connection_state_t conn = server->getConn();
        amqp_bytes_t_ queue = amqp_cstring_bytes(queueName.c_str());
        amqp_envelope_t envelope;
        std::string msg;
        amqp_maybe_release_buffers(conn);
        amqp_basic_consume(conn, 1, queue, amqp_empty_bytes, 0, 0, 0, amqp_empty_table);
        amqp_rpc_reply_t ret = amqp_consume_message(conn, &envelope, nullptr, 0);
        if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
            BOOST_LOG_TRIVIAL(error) << "Failed to consume message. Reply type: " << ret.reply_type;
        } else {
            msg = string(static_cast<char *>(envelope.message.body.bytes), envelope.message.body.len);
            BOOST_LOG_TRIVIAL(info) << "Message received.";
        }
        amqp_basic_ack(conn, envelope.channel, envelope.delivery_tag, 0);
        amqp_destroy_envelope(&envelope);
        return msg;
    }
    catch (const std::exception &e) {
        BOOST_LOG_TRIVIAL(error) << "Error in read_queue function: " << e.what();
        return "";
    }

}

void Consumer::declare_queue() const {

    amqp_queue_declare(server->getConn(), 1, amqp_cstring_bytes(queueName.c_str()), 0, 0, 0, 0, amqp_empty_table);
    amqp_rpc_reply_t ret = amqp_get_rpc_reply(server->getConn());
    if (AMQP_RESPONSE_NORMAL != ret.reply_type) {
        BOOST_LOG_TRIVIAL(error) << "Consumer: Error queue declare " << ret.reply_type;
    } else {
        BOOST_LOG_TRIVIAL(info) << "Consumer: Declare queue done";
    }
}


