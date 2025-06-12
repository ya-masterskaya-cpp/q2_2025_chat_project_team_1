#include "logger.h"
#include "log_functions.h" // типовый фунции логирования
#include "srv.h"



int main()
{

    // Инициализируем логирование
    logger::Logger().Init(logger::Logger::Level::Debug);
    
    try{
    setlocale(LC_ALL, "Ru-ru");
    const unsigned num_threads = std::thread::hardware_concurrency();
    net::io_context ioc(num_threads);
    MainServer server(ioc);
    server.init();
    server.Listen();
    Service::MtreadRunContext(ioc);
    }
    catch(const std::exception& ex){
        log_functions::LogFatalError(ex.what());
        ZyncPrint("MAINSERVER EX", ex.what()); // TODO заменить все вызовы ZyncPrint логгированием
    }
}
