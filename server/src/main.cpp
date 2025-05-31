#include <iostream>
#include <pqxx/pqxx>  // Для libpqxx
#include <boost/asio.hpp>  // Для Boost.Asio (пример использования Boost)

int main() {
    try {
        // ===== Часть 1: Проверка libpqxx (PostgreSQL) =====
        std::cout << "Testing libpqxx (PostgreSQL)..." << std::endl;
        try{
        pqxx::connection conn("dbname=test user=postgres password=1234 host=localhost port=5432");

        if (conn.is_open()) {
            std::cout << "Connected to PostgreSQL! Database: " << conn.dbname() << std::endl;
        } else {
            std::cout << "Connection failed!" << std::endl;
        }
        } catch(...) {

        }

        // ===== Часть 2: Проверка Boost (на примере asio) =====
        std::cout << "\nTesting Boost.Asio..." << std::endl;
        boost::asio::io_context io;
        boost::asio::steady_timer timer(io, boost::asio::chrono::seconds(1));

        timer.async_wait([](const boost::system::error_code&) {
            std::cout << "Boost.Asio timer fired after 1 second!" << std::endl;
        });

        io.run();

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
