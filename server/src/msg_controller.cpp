#include "msg_controller.h"


void MessageController::SendMessage(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto chat_service = ChatServicePlugin::GetService();

    // Токен авторизации не извлечен
    auto token_opt = http_utils::TryExtractToken(req);
    if (!token_opt) {
        http_utils::RespondWithError("Failed to extract token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string token = token_opt.value();

    // Токен авторизации не найден
    auto user_opt = chat_service->GetUserByToken(token);
    if (!user_opt) {
        http_utils::RespondWithError("Invalid token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    // нет нужных полей, невалидный json
    auto json = req->getJsonObject();
    if (!json || !json->isMember("text")) {
        http_utils::RespondWithError("Invalid JSON format", drogon::k400BadRequest, std::move(callback));
        return;
    }

    std::string from = user_opt->username;
    std::string text = (*json)["text"].asString(); // парсим текст

    // пустое поле json
    if (text.empty()) {
        http_utils::RespondWithError("Empty message from", drogon::k400BadRequest, std::move(callback));
        return;
    }

    // успех - выполняем рассылку
    Json::Value msg;
    msg["from"] = from;
    msg["text"] = text;
    std::string serialized = Json::FastWriter().write(msg); 

    ChatWebSocket::Broadcast(token, serialized); // рассылка через WebSocket в пределах комнаты

    http_utils::RespondWithSuccess("Message from " + from + " sent", drogon::k200OK, std::move(callback));
}

void MessageController::UploadMessage(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto chat_service = ChatServicePlugin::GetService();

    // Токен авторизации не извлечен
    auto token_opt = http_utils::TryExtractToken(req);
    if (!token_opt) {
        http_utils::RespondWithError("Failed to extract token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string token = token_opt.value();

    // Токен авторизации не найден
    auto user_opt = chat_service->GetUserByToken(token);
    if (!user_opt) {
        http_utils::RespondWithError("Invalid token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    // нет нужных полей, невалидный json
    auto json = req->getJsonObject();
    if (!json || !json->isMember("text")) {
        http_utils::RespondWithError("Invalid JSON format", drogon::k400BadRequest, std::move(callback));
        return;
    }

    // пустое поле json
    std::string text = (*json)["text"].asString();
    if (text.empty()) {
        http_utils::RespondWithError("Empty message", drogon::k400BadRequest, std::move(callback));
        return;
    }

    // Ошибка при сохранении в БД
    if (!chat_service->SaveMessage(token, text)) {
        http_utils::RespondWithError("Failed to save message to DB", drogon::k500InternalServerError, std::move(callback));
        return;
    }

    // Успех
    http_utils::RespondWithSuccess("Message saved to DB", drogon::k200OK, std::move(callback));
}

void MessageController::GetRecentMessages(const drogon::HttpRequestPtr &req, std::function<void(const drogon::HttpResponsePtr &)> &&callback) {
    auto chat_service = ChatServicePlugin::GetService();

    // Токен авторизации не извлечен
    auto token_opt = http_utils::TryExtractToken(req);
    if (!token_opt) {
        http_utils::RespondWithError("Failed to extract token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string token = token_opt.value();

    // Токен авторизации не найден
    if (!chat_service->GetUserByToken(token)) {
        http_utils::RespondWithError("Invalid token", drogon::k401Unauthorized, std::move(callback));
        return;
    }

    const std::string room = req->getParameter("room");
    int max_items = std::stoi(req->getParameter("max_items"));

    // проверка параметров
    if (room.empty() || max_items <= 0) {
        http_utils::RespondWithError("Invalid parameters", drogon::k400BadRequest, std::move(callback));
        return;
    }

    const auto messages = chat_service->GetRecentMessages(room, max_items);

    Json::Value result(Json::arrayValue);
    for (const auto& msg : messages) {
        Json::Value item;

        item["sent_at"] = msg.sent_at;
        item["text"] = msg.message;

        auto name_opt = chat_service->GetUserNameById(msg.user_id);
        item["from"] = name_opt.value_or(""); // если пользователь не найден - пустая строка // TODO добавить ошибку 400 "User not found"

        result.append(item);
    }

    http_utils::RespondWithJson(result, "Messages extracted from DB", drogon::k200OK, callback);
}
