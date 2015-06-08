#pragma once

#include <functional>
#include <string>
#include <sstream>
#include <queue>

#include <stdint.h>

#include <account.h>
#include <sslconn.h>

#include <thrift/transport/TTransport.h>

#include "wrapper.hpp"

class LineHttpTransport : public apache::thrift::transport::TTransport {

    enum class ConnectionState {
        DISCONNECTED = 0,
        CONNECTED = 1,
        RECONNECTING = 2,
    };

    class Request {
    public:
        std::string method;
        std::string path;
        std::string content_type;
        std::string body;
        std::function<void()> callback;
    };

    static const size_t BUFFER_SIZE = 4096;

    PurpleAccount *acct;
    PurpleConnection *conn;

    std::string host;
    uint16_t port;
    bool ls_mode;
    std::string x_ls;

    ConnectionState state;

    bool auto_reconnect;
    guint reconnect_timeout_handle;
    int reconnect_timeout;

    PurpleSslConnection *ssl;
    guint input_handle;
    int connection_id;

    uint8_t buf[BUFFER_SIZE];

    std::stringbuf request_buf;

    size_t request_written;
    std::string request_data;

    bool in_progress;
    std::string response_str;
    std::stringbuf response_buf;

    std::queue<Request> request_queue;

    bool keep_alive;
    int status_code_;
    int content_length_;

public:

    LineHttpTransport(PurpleAccount *acct, PurpleConnection *conn,
        std::string host, uint16_t port,
        bool plain_http);
    ~LineHttpTransport();

    void set_auto_reconnect(bool auto_reconnect);

    virtual void open();
    virtual void close();

    virtual uint32_t read_virt(uint8_t *buf, uint32_t len);
    void write_virt(const uint8_t *buf, uint32_t len);

    void request(std::string method, std::string path, std::string content_type,
        std::function<void()> callback);
    int status_code();
    int content_length();

    //virtual const uin8_t* borrow_virt(uint8_t *buf, uint32_t *len);
    //virtual void consume_virt(uint32_t len);

private:

    void write_request();

    void ssl_connect(PurpleSslConnection *, PurpleInputCondition);
    void ssl_error(PurpleSslConnection *, PurpleSslErrorType err);
    void ssl_write(int, PurpleInputCondition);
    void ssl_read(int, PurpleInputCondition);

    int reconnect_timeout_cb();

    void send_next();

    void try_parse_response_header();
};
