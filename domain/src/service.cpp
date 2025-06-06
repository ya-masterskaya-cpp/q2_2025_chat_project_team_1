#include "service.h"

using namespace std;

namespace Service {
    const std::unordered_map<std::string, ACTION> Additional::action_scernario{
        {CONSTANTS::ACT_CREATE_ROOM, ACTION::CREATE_ROOM},
        {CONSTANTS::ACT_CREATE_USER, ACTION::CREATE_USER},
        {CONSTANTS::ACT_DISCONNECT, ACTION::DISCONNECT},
        {CONSTANTS::ACT_GET_USERS, ACTION::GET_USERS},
        {CONSTANTS::ACT_LOGIN, ACTION::LOGIN},
        {CONSTANTS::ACT_ROOM_LIST, ACTION::ROOM_LIST},
        {CONSTANTS::ACT_SEND_MESSAGE, ACTION::SEND_MESSAGE}};

    const std::unordered_set<std::string> Service::Additional::chatroom_actions{
        CONSTANTS::ACT_DISCONNECT, CONSTANTS::ACT_SEND_MESSAGE};

    const std::unordered_set<std::string> Service::Additional::server_actions{
        CONSTANTS::ACT_CREATE_ROOM, CONSTANTS::ACT_CREATE_USER,
        CONSTANTS::ACT_GET_USERS, CONSTANTS::ACT_LOGIN, CONSTANTS::ACT_ROOM_LIST};

    const std::unordered_set<std::string> Service::Additional::request_directions{
        CONSTANTS::RF_DIRECTION_SERVER, CONSTANTS::RF_DIRECTION_CHATROOM};

    std::mutex Service::Additional::mtx;

    /// @brief Запускает контекст ввода-вывода в нескольких потоках
    /// @param ioc Контекст ввода-вывода
    void MtreadRunContext(net::io_context &ioc) {
        std::vector<jthread> run;
        for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
            run.push_back(jthread([&ioc] { ioc.run(); }));
        }
        ioc.run();
    }

    /// @brief Считывает всё содержимое из файла в строку
    /// @param ifs Поток чтения файла
    /// @return Содержимое файла в виде строки или CONSTANTS::RF_ERROR при ошибке
    std::string ReadFromFstream(std::ifstream &ifs) {
        if (!ifs) {
            std::cerr << "Bad Filestream\n";
            return CONSTANTS::RF_ERROR;
        }
        std::stringstream strm;
        while (ifs) {
            std::string tmp;
            std::getline(ifs, tmp);
            strm << tmp;
        }
        return strm.str();
    }

    /// @brief Создаёт shared_ptr для strand с заданным контекстом ioc
    /// @param ioc Контекст ввода-вывода
    /// @return Умный указатель на strand
    shared_strand MakeSharedStrand(net::io_context &ioc) {
        return std::make_shared<strand>(net::make_strand(ioc));
    }

    /// @brief Создаёт shared_ptr для MutableBufferHolder (пустой)
    /// @return Умный указатель на MutableBufferHolder
    std::shared_ptr<MutableBufferHolder> MakeSharedMutableGuffer() {
        return std::shared_ptr<Service::MutableBufferHolder>();
    }

    /// @brief Создаёт shared_ptr для streambuf
    /// @return Умный указатель на streambuf
    std::shared_ptr<net::streambuf> MakeSharedStreambuf() {
        return std::make_shared<net::streambuf>();
    }
}

namespace Service {
    /// @brief Проверяет, жив ли сокет
    /// @param sock Ссылка на TCP сокет
    /// @return true если сокет открыт и соединение активно, иначе false
    bool IsAliveSocket(tcp::socket &sock) {
        if (!sock.is_open()) {
            return false;
        }
        boost::system::error_code ec;
        char data;
        size_t len = sock.receive(boost::asio::buffer(&data, 1), boost::asio::socket_base::message_peek, ec);

        if (ec) {
            if (ec == boost::asio::error::would_block || ec == boost::asio::error::try_again) {
                return true;
            }
            if (ec == boost::asio::error::eof) {
                return false;
            } else {
                return false;
            }
        }
        // Данные доступны для чтения — соединение живо
        return len > 0;
    }

    /// @brief Проверяет, жив ли сокет из shared_ptr
    /// @param sock Умный указатель на TCP сокет
    /// @return true если сокет открыт и соединение активно, иначе false
    bool IsAliveSocket(shared_socket sock) {
        return IsAliveSocket(*sock);
    }

    /// @brief Корректно закрывает и выключает сокет
    /// @param sock Ссылка на TCP сокет
    void ShutDownSocket(tcp::socket &sock) {
        boost::system::error_code ec;
        sock.cancel(ec);
        if (ec) {
            std::cout << ec.what() << std::endl;
        }
        sock.shutdown(sock.shutdown_both, ec);
        if (ec) {
            std::cout << ec.what() << std::endl;
        }
        sock.close();
        if (ec) {
            std::cout << ec.what() << std::endl;
        }
    }

    /// @brief Корректно закрывает и выключает сокет из shared_ptr
    /// @param sock Умный указатель на TCP сокет
    void ShutDownSocket(shared_socket sock) {
        ShutDownSocket(*sock);
    }
}

namespace Service {
    /// @brief Извлекает строку из буфера streambuf
    /// @param buffer Буфер, из которого извлекается строка
    /// @param extract Количество символов для извлечения
    /// @return Извлечённая и очищенная от пробелов строка
    // std::string ExtractStrFromStreambuf(net::streambuf &buffer, size_t extract) {
    //     const char *data = boost::asio::buffer_cast<const char *>(buffer.data());
    //     std::string str(data, extract);
    //     boost::algorithm::trim(str);
    //     return str;
    // }

    /// @brief Извлекает объект из буфера streambuf и десериализует его
    /// @param buffer Буфер с сериализованными данными
    /// @param extract Количество символов для извлечения
    /// @return Задача с десериализованными данными (task)
    task ExtractObjectsfromBuffer(net::streambuf &buffer, size_t extract) {
        std::string str(ExtractStrFromStreambuf(buffer, extract));
        return DeserializeUmap<std::string, std::string>(str);
    }

    /// @brief Извлекает объект из буфера streambuf и десериализует его, возвращая shared_ptr на task
    /// @param buffer Буфер с сериализованными данными
    /// @param extract Количество символов для извлечения
    /// @return Shared задача с десериализованными данными (shared_task)
    shared_task ExtractSharedObjectsfromBuffer(net::streambuf &buffer, size_t extract) {
        std::string str(ExtractStrFromStreambuf(buffer, extract));
        return std::make_shared<task>(DeserializeUmap<std::string, std::string>(str));
    }
}
