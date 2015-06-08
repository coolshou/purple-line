#pragma once

#include <string>
#include <functional>
#include <list>

#include <account.h>
#include <util.h>

enum class HTTPFlag {
    NONE =  0,
    AUTH =  1 << 0,
    LARGE = 1 << 1,
};

inline constexpr HTTPFlag operator|(HTTPFlag a, HTTPFlag b) {
    return (HTTPFlag)((int)a | (int)b);
};

inline constexpr bool operator&(HTTPFlag a, HTTPFlag b) {
    return ((int)a & (int)b) != 0;
}

class HTTPClient {
    const int MAX_IN_FLIGHT = 4;

    using CompleteFunc = std::function<void(int, const guchar *, gsize)>;

    struct Request {
        HTTPClient *client;
        std::string url;
        std::string content_type;
        std::string body;
        HTTPFlag flags;
        CompleteFunc callback;
        PurpleUtilFetchUrlData *handle;
    };

    PurpleAccount *acct;

    std::list<Request *> request_queue;
    int in_flight;

    void execute_next();
    void parse_response(const char *res, int &status, const guchar *&body);
    void complete(Request *req, const gchar *url_text, gsize len, const gchar *error_message);

    static void purple_cb(PurpleUtilFetchUrlData *url_data, gpointer user_data,
        const gchar *url_text, gsize len, const gchar *error_message);

public:

    HTTPClient(PurpleAccount *acct);
    ~HTTPClient();

    void request(std::string url, CompleteFunc callback);
    void request(std::string url, HTTPFlag flags, CompleteFunc callback);
    void request(std::string url, HTTPFlag flags,
        std::string content_type, std::string body,
        CompleteFunc callback);

};
