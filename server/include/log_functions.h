#pragma once

#include <string>

#include "logger.h" // методы логирования по уровням Logger::Get().<Level>
#include "const.h" // тэги для Logger::Get().<Level>

// Вспомогательные функции для логирования типовых событий IRC-сервера
// TODO разбить на *.h/*.cpp 
namespace log_functions {
    using namespace logger;

// Логирует успешный запуск сервера с указанием порта
inline void LogServerStarted(int port) {
    Logger::Get().Info("[" + LogTag::ServerStart + "] Server started on port: " + std::to_string(port));
}

// Логирует критическую ошибку сервера
inline void LogFatalError(const std::string& message) {
    Logger::Get().Fatal("[" + LogTag::FatalError + "] " + message);
}

// Логирует подключение клиента
inline void LogClientConnected(const std::string& client_info) { // client_info - токен или IP
    Logger::Get().Info("[" + LogTag::ClientConnect + "] Client connected: " + client_info);
}

// Логирует отключение клиента
inline void LogClientDisconnected(const std::string& client_info) { // client_info - токен или IP
    Logger::Get().Info("[" + LogTag::ClientDisconnect + "] Client disconnected: " + client_info);
}

// Логирует успешную аутентификацию пользователя
inline void LogAuthSuccess(const std::string& user) {
    Logger::Get().Info("[" + LogTag::AuthSuccess + "] Login success: " + user);
}

// Логирует неудачную попытку входа с причиной отказа
inline void LogAuthFail(const std::string& user, const std::string& reason) {
    Logger::Get().Warning("[" + LogTag::AuthFail + "] Login failed for " + user + ": " + reason);
}

// Логирует создание новой комнаты
inline void LogRoomCreated(const std::string& room) {
    Logger::Get().Info("[" + LogTag::CreateRoom + "] Room created: " + room);
}

// Логирует вход пользователя в комнату
inline void LogUserJoinedRoom(const std::string& user, const std::string& room) {
    Logger::Get().Info("[" + LogTag::UserJoinRoom + "] " + user + " joined room " + room);
}

// Логирует выход пользователя из комнаты
inline void LogUserLeftRoom(const std::string& user, const std::string& room) {
    Logger::Get().Info("[" + LogTag::UserLeaveRoom + "] " + user + " left room " + room);
}

// Логирует отправку сообщения клиентом
inline void LogMessageSent(const std::string& user, const std::string& message) {
    Logger::Get().Debug("[" + LogTag::MessageSent + "] " + user + ": " + message);
}

// Логирует факт отклоненного сообщения + описание причины
inline void LogMessageRejected(const std::string& user, const std::string& reason) {
    Logger::Get().Warning("[" + LogTag::MessageRejected + "] Message rejected from " + user + ": " + reason);
}

// Логирует выполнение команды сервера (ACT_SEND_MESSAGE)
inline void LogCommandExecuted(const std::string& command) {
    Logger::Get().Debug("[" + LogTag::ExecuteCommand + "] Command executed: " + command);
}

// Логирует ошибку парсинга некорректного запроса
inline void LogCommandParseError(const std::string& input) {
    Logger::Get().Warning("[" + LogTag::ParseError + "] Failed to parse command: " + input);
}

} // namespace log_functions
