#include <iostream>

#include "tcp_client.h"

void TcpCLient::Connect(const std::string& host, unsigned short port) {
    try {
        boost::asio::ip::tcp::resolver resolver(io_context_);
        auto endpoints = resolver.resolve(host, std::to_string(port));

        boost::asio::connect(socket_, endpoints);
        connected_ = true;
        std::cout << "Connected to " << host << ":" << port << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Connection error: " << e.what() << std::endl;
        connected_ = false;
        throw;
    }
}

void TcpCLient::Send(const std::string& message) {
    if (!IsConnected()) {
        throw std::runtime_error("Not connected");
    }

    try {
        boost::asio::write(socket_, boost::asio::buffer(message));
    } catch (const std::exception& e) {
        connected_ = false;
        std::cerr << "Send error: " << e.what() << std::endl;
        throw;
    }
}

//TODO: Change throw to ErrorHandler template
std::string TcpCLient::Receive() {
    if (!IsConnected()) {
        throw std::runtime_error("Not connected");
    }

    try {
        boost::asio::streambuf buffer;
        boost::asio::read_until(socket_, buffer, '\n');

        std::istream is(&buffer);
        std::string message;
        std::getline(is, message);

        return message;
    } catch (const std::exception& e) {
        connected_ = false;
        std::cerr << "Receive error: " << e.what() << std::endl;
        throw;
    }
}

void TcpCLient::Close() {
    if (socket_.is_open()) {
        try {
            socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
            socket_.close();
            connected_ = false;
            std::cout << "Disconnected" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Disconnection error: " << e.what() << std::endl;
        }
    }
}

bool TcpCLient::IsConnected() const {
    return connected_ && socket_.is_open();
}
