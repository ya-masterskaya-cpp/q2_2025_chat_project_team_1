#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <boost/asio.hpp>


#include "transfer_interface.h"

namespace transfer {

using boost::asio::ip::tcp;
using boost::asio::io_context;

class TcpCLient : public ClientTransferInterface {
public:
    TcpCLient() : io_context_{}, socket_{io_context_} {}

    ~TcpCLient() override {
        Close();
    }

    void Connect(const std::string& host, uint16_t port) override;
    void Send(const std::string& message) override;
    std::string Receive(char delimeter = '\0') override;
    void Close() override;
    bool IsConnected() const override;

    template<typename ErrorHandler>
    void Connect(const std::string& host, unsigned short port, ErrorHandler err_handler) {
        try {
            Connect(host,port);
        } catch (...) {
            err_handler();
        }
    }

    template <typename ErrorHandler>
    void Send(const std::string& message, ErrorHandler err_handler) {
        try {
            Send(message);
        } catch (...) {
            err_handler();
        }
    }

    template<typename ErrorHandler>
    std::string Receive(ErrorHandler err_handler, char delimeter = '\0') {
        try {
            Receive(delimeter);
        } catch (...) {
            err_handler();
        }
    }



private:
    io_context io_context_;
    tcp::socket socket_;
    bool connected_ = false;
};


}//namespace transfer
#endif // TCP_CLIENT_H
