#include "tcp_client.h"


namespace transfer {

void TcpCLient::Connect(const std::string& host, unsigned short port) {
    try {
        boost::asio::ip::tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, std::to_string(port));

        boost::asio::connect(socket_, endpoints);
        connected_ = true;
    } catch (...) {
        connected_ = false;
        throw;
    }
}

//TODO: maybe exception change to boost::system::error_code
void TcpCLient::Send(const std::string& message) {
    if (!IsConnected()) {
        throw std::runtime_error("Not connected");
    }

    try {
        boost::asio::write(socket_, boost::asio::buffer(message));
    } catch (...) {
        connected_ = false;
        throw;
    }
}

//TODO: Change throw to ErrorHandler template
std::string TcpCLient::Receive(char delimeter) {
    if (!IsConnected()) {
        throw std::runtime_error("Not connected");
    }

    try {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket_, buffer, delimeter);

        std::string message(
            boost::asio::buffers_begin(buffer.data()),
            boost::asio::buffers_end(buffer.data())
        );

        return message;
    } catch (const std::exception& e) {
        connected_ = false;
        throw;
    }
}

void TcpCLient::Close() {
    if (socket_.is_open()) {
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket_.close();
        connected_ = false;
    }
}

bool TcpCLient::IsConnected() const {
    return connected_ && socket_.is_open();
}

} //namespace transfer
