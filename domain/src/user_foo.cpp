#include "service.h"

using namespace std;

namespace UserInterface {

    /// @brief Устанавливает направление действия для чатрума
    /// @param action Словарь параметров действия, куда записывается направление
    void ChrSetChatroomDirection(unordered_map<string, string>& action) {
        action[CONSTANTS::LF_DIRECTION] = CONSTANTS::RF_DIRECTION_CHATROOM;
    }

    /// @brief Создаёт сериализованный объект отключения пользователя
    /// @param token Токен пользователя
    /// @return Сериализованная строка с данными отключения
    std::string US_ChrMakeObjDisconnect(std::string token) {
        unordered_map<string, string> res{
            {CONSTANTS::LF_ACTION, CONSTANTS::ACT_DISCONNECT},
            {CONSTANTS::LF_TOKEN, token}
        };
        ChrSetChatroomDirection(res);
        return Service::SerializeUmap(res);
    }

    /// @brief Создаёт сериализованный объект отправки сообщения в чатрум
    /// @param token Токен пользователя
    /// @param message Сообщение для отправки
    /// @return Сериализованная строка с данными сообщения
    std::string US_ChrMakeSendMessage(std::string token, std::string message) {
        unordered_map<string, string> res{
            {CONSTANTS::LF_ACTION, CONSTANTS::ACT_SEND_MESSAGE},
            {CONSTANTS::LF_TOKEN, token},
            {CONSTANTS::LF_MESSAGE, message}
        };
        ChrSetChatroomDirection(res);
        return Service::SerializeUmap(res);
    }

    /// @brief Устанавливает направление действия для сервера
    /// @param action Словарь параметров действия, куда записывается направление
    void ChrSetServerDirection(unordered_map<string, string>& action) {
        action[CONSTANTS::LF_DIRECTION] = CONSTANTS::RF_DIRECTION_SERVER;
    }

    /// @brief Создаёт сериализованный объект запроса списка пользователей в комнате
    /// @param name Название комнаты
    /// @return Сериализованная строка с запросом списка пользователей
    std::string US_SrvMakeObjGetUsers(std::string name) {
        unordered_map<string, string> res{
            {CONSTANTS::LF_ACTION, CONSTANTS::ACT_GET_USERS},
            {CONSTANTS::LF_ROOMNAME, std::move(name)}
        };
        ChrSetServerDirection(res);
        return Service::SerializeUmap(res);
    }

    /// @brief Создаёт сериализованный объект входа пользователя
    /// @param name Имя пользователя
    /// @param password Пароль пользователя
    /// @param roomname Название комнаты
    /// @return Сериализованная строка с данными входа
    std::string US_SrvMakeObjLogin(std::string name, std::string password, std::string roomname) {
        unordered_map<string, string> res{
            {CONSTANTS::LF_ACTION, CONSTANTS::ACT_LOGIN},
            {CONSTANTS::LF_NAME, std::move(name)},
            {CONSTANTS::LF_ROOMNAME, std::move(roomname)},
            {CONSTANTS::LF_PASSWORD, std::move(password)}
        };
        ChrSetServerDirection(res);
        return Service::SerializeUmap(res);
    }

    /// @brief Создаёт сериализованный объект для создания нового пользователя
    /// @param name Имя пользователя
    /// @param password Пароль пользователя
    /// @return Сериализованная строка с данными нового пользователя
    std::string US_SrvMakeObjCreateUser(std::string name, std::string password) {
        unordered_map<string, string> res{
            {CONSTANTS::LF_ACTION, CONSTANTS::ACT_CREATE_USER},
            {CONSTANTS::LF_NAME, std::move(name)},
            {CONSTANTS::LF_PASSWORD, std::move(password)}
        };
        ChrSetServerDirection(res);
        return Service::SerializeUmap(res);
    }

    /// @brief Создаёт сериализованный объект для создания новой комнаты
    /// @param name Название комнаты
    /// @return Сериализованная строка с данными новой комнаты
    std::string US_SrvMakeObjCreateRoom(std::string name) {
        unordered_map<string, string> res{
            {CONSTANTS::LF_ACTION, CONSTANTS::ACT_CREATE_ROOM},
            {CONSTANTS::LF_ROOMNAME, std::move(name)}
        };
        ChrSetServerDirection(res);
        return Service::SerializeUmap(res);
    }

    /// @brief Создаёт сериализованный объект для запроса списка комнат
    /// @return Сериализованная строка с запросом списка комнат
    std::string US_SrvMakeObjRoomList() {
        unordered_map<string, string> res{
            {CONSTANTS::LF_ACTION, CONSTANTS::ACT_ROOM_LIST}
        };
        ChrSetServerDirection(res);
        return Service::SerializeUmap(res);
    }

} // namespace UserInterface
