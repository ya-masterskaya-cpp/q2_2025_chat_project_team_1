// Этот заголовочный файл надо подключить в одном и только одном .cpp-файле программы
#include <boost/json/src.hpp>
#include "srv.h"

void MainServer::init()
{

    try
    {
        //std::string load_var = "loaderserv.conf";
        std::string load_var = "../data/loaderserv.conf";
        std::ifstream ifs(load_var);
        std::string json = Service::ReadFromFstream(ifs);

        if (json == CONSTANTS::RF_ERROR)
        {
            std::abort();
        }

        auto obj = boost::json::parse(std::move(json));
        auto error = ServiceChatroomServer::CHK_ServerLoadObject(obj);

        if (error)
        {
            std::cerr << "Server build error: " << *error;
            std::abort();
        }

        auto ip = obj.as_object().at(CONSTANTS::IP).as_string();
        auto port = obj.as_object().at(CONSTANTS::PORT).as_int64();

        std::cout<<"|" << ip << " |" << port << "|\n";
        err ec;
        endpoint_ = tcp::endpoint(net::ip::make_address(ip), port);

        
        acceptor_.open(endpoint_.protocol() , ec);
        acceptor_.set_option(net::socket_base::reuse_address(true));
        acceptor_.bind(endpoint_);
        acceptor_.listen(net::socket_base::max_listen_connections);

        if (obj.as_object().contains(CONSTANTS::CHATROOMS))
        {
            const auto &rooms = obj.as_object().at(CONSTANTS::CHATROOMS).as_array();
            for (const auto &room : rooms)
            {
                this->CreateRoom(std::string(room.as_string()));
            }
        }
      
    }
    catch (const std::exception &ex)
    {
        std::cerr << "Server build error: " << ex.what() << '\n';
    }
};

void MainServer::Listen()
{
    std::cout << "Listening...\n";

    acceptor_.async_accept([&](boost::system::error_code ec, tcp::socket socket)
                           {
      if(ec){
              std::cout << ec.value() << "  " << ec.message() << '\n' <<
              ec.what() << '\n';
            }                                           
             std::shared_ptr<ServerSession> servsess = std::make_shared<ServerSession>
             (this, std::make_shared<beast::tcp_stream>(std::move(socket))); 
             servsess->HandleSession();
             
             
             Listen();});
}

MainServer::MainServer(net::io_context &ioc) : ioc_(ioc), acceptor_(net::make_strand(ioc_))
{
}

void MainServer::PrintRooms()
{
    for (auto &&room : rooms_)
    {
        std::cout << room.first << " members:" << room.second->users_.size() << '\n';
    }
}


