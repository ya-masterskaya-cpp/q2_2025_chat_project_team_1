#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <boost/asio.hpp>

#include "transfer_interface.h"

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
    std::string Receive() override;
    void Close() override;
    bool IsConnected() const override;

private:
    io_context io_context_;
    tcp::socket socket_;
    bool connected_ = false;
};

#endif // TCP_CLIENT_H
