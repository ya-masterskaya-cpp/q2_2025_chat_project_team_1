#include "room_controller.h"


void RoomController::CreateRoom(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // Токен авторизации не извлечен
    auto token_opt = http_utils::TryExtractToken(req);
    if (!token_opt) {
        http_utils::RespondWithError("Failed to extract token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string token = token_opt.value();

    // Токен авторизации не найден
    std::string user = "";
    if (!TokenStorage::instance().HasUserByToken(token, user)) {
        http_utils::RespondWithError("Invalid token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    // нет нужных полей, невалидный json
    auto json = req->getJsonObject();
    if (!json || !json->isMember("name")) {
        http_utils::RespondWithError("Invalid JSON format", drogon::k400BadRequest, std::move(callback));
        return;
    }

    const std::string name = (*json)["name"].asString();

    // пустое поле json
    if (name.empty()) {
        http_utils::RespondWithError("Empty room name", drogon::k400BadRequest, std::move(callback));
        return;
    }

    // комната с таким именем уже есть
    if (!RoomManager::instance().CreateRoom(name)) {
        http_utils::RespondWithError("Room already created", drogon::k409Conflict, std::move(callback));
        return;
    }

    // успех
    http_utils::RespondWithSuccess("Room created", drogon::k201Created, std::move(callback));
}

void RoomController::JoinRoom(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // Токен авторизации не извлечен
    auto token_opt = http_utils::TryExtractToken(req);
    if (!token_opt) {
        http_utils::RespondWithError("Failed to extract token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string token = token_opt.value();

    // Токен авторизации не найден
    std::string user = "";
    if (!TokenStorage::instance().HasUserByToken(token, user)) {
        http_utils::RespondWithError("Invalid token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    // нет нужных полей, невалидный json
    auto json = req->getJsonObject();
    if (!json || !json->isMember("name")) {
        http_utils::RespondWithError("Invalid JSON format", drogon::k400BadRequest, std::move(callback));
        return;
    }

    const std::string name = (*json)["name"].asString();

    // пустое имя комнаты
    if (name.empty()) {
        http_utils::RespondWithError("Empty room name", drogon::k400BadRequest, std::move(callback));
        return;
    }

    // нельзя перейти в ту же самую комнату или в несуществующую комнату
    if (!RoomManager::instance().JoinRoom(user, name)) {
        http_utils::RespondWithError("Invalid room join", drogon::k400BadRequest, std::move(callback));
        return;
    }

    // успех
    http_utils::RespondWithSuccess("Joined room", drogon::k200OK, std::move(callback));
}

// возврат в GENERAL_ROOM
void RoomController::LeaveRoom(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // Токен авторизации не извлечен
    auto token_opt = http_utils::TryExtractToken(req);
    if (!token_opt) {
        http_utils::RespondWithError("Failed to extract token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string token = token_opt.value();

    // Токен авторизации не найден
    std::string user = "";
    if (!TokenStorage::instance().HasUserByToken(token, user)) {
        http_utils::RespondWithError("Invalid token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    // нельзя выйти из общей комнаты, находясь в ней
    if (!RoomManager::instance().LeaveRoom(user)) {
        http_utils::RespondWithError("Already in general room", drogon::k400BadRequest, std::move(callback));
        return;
    }

    // успех
    http_utils::RespondWithSuccess("Left to general room", drogon::k200OK, std::move(callback));
}

void RoomController::ListRooms(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // Токен авторизации не извлечен
    auto token_opt = http_utils::TryExtractToken(req);
    if (!token_opt) {
        http_utils::RespondWithError("Failed to extract token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string token = token_opt.value();

    // Токен авторизации не найден
    std::string user = "";
    if (!TokenStorage::instance().HasUserByToken(token, user)) {
        http_utils::RespondWithError("Invalid token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    // успех - возвращаем список всех комнат
    Json::Value result(Json::arrayValue);

    for (const auto &name : RoomManager::instance().ListRooms()) {
        result.append(name);
    }

    auto resp = drogon::HttpResponse::newHttpJsonResponse(result);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
    // http_utils::RespondWithSuccess(?); // TODO создать удобную обертку c логгированием
}

void RoomController::CurrentRoom(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // Токен авторизации не извлечен
    auto token_opt = http_utils::TryExtractToken(req);
    if (!token_opt) {
        http_utils::RespondWithError("Failed to extract token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string token = token_opt.value();

    // Токен авторизации не найден
    std::string user = "";
    if (!TokenStorage::instance().HasUserByToken(token, user)) {
        http_utils::RespondWithError("Invalid token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    // успех - возвращаем текущую комнату пользователя
    Json::Value json;
    json["room"] = RoomManager::instance().GetRoomOfUser(user);

    auto resp = drogon::HttpResponse::newHttpJsonResponse(json);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
    // http_utils::RespondWithSuccess(?); // TODO создать удобную обертку c логгированием
}

void RoomController::ListUsersInRoom(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    // Токен авторизации не извлечен
    auto token_opt = http_utils::TryExtractToken(req);
    if (!token_opt) {
        http_utils::RespondWithError("Failed to extract token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string token = token_opt.value();

    // Токен авторизации не найден
    std::string user = "";
    if (!TokenStorage::instance().HasUserByToken(token, user)) {
        http_utils::RespondWithError("Invalid token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string room = req->getParameter("name");

    // комната не существует
    if (!RoomManager::instance().HasRoom(room)) {
        http_utils::RespondWithError("Room not found", drogon::k404NotFound, std::move(callback));
        return;
    }

    // успех - возвращаем список пользователей в комнате
    Json::Value result(Json::arrayValue);
    for (const auto &username : RoomManager::instance().GetUsersInRoom(room)) {
        result.append(username);
    }

    auto resp = drogon::HttpResponse::newHttpJsonResponse(result);
    resp->setStatusCode(drogon::k200OK);
    callback(resp);
    // http_utils::RespondWithSuccess(?); // TODO создать удобную обертку c логгированием
}
