
#include "chat_service.h"
#include "room_manager.h"
#include "token_manager.h"
#include "user_manager.h"

#include <iostream>


using namespace chat;

int main() {
    auto room_manager = std::make_shared<RoomManager>();
    auto user_manager = std::make_shared<UserManager>();
    auto token_manager = std::make_shared<TokenManager>();
    ChatService chat(user_manager, room_manager, token_manager);

    // alice пытается войти до регистрации
    auto t1 = chat.Login("alice", "hash");
    std::cout << "Alice tries to login (should fail): " << (t1 ? "FAILED" : "OK") << "\n";

    // регистрация alice
    auto u1 = chat.Register("alice", "hash");
    std::cout << "Alice registers: " << (u1 ? "OK" : "FAILED") << "\n";

    // повторная регистрация
    auto u1_dup = chat.Register("alice", "hash");
    std::cout << "Alice registers again: " << (u1_dup ? "FAILED" : "OK") << "\n";

    // логин alice
    auto token_alice = chat.Login("alice", "hash");
    std::cout << "Alice logs in: " << (token_alice ? "OK" : "FAILED") << "\n";

    // список комнат
    std::cout << "Room list:\n";
    for (const auto& name : chat.GetRoomNames()) std::cout << "- " << name << "\n";

    // все пользователи онлайн
    std::cout << "Chat users:\n";
    for (const auto& name : chat.GetOnlineUserNames()) std::cout << "- " << name << "\n";

    // текущая комната alice
    std::cout << "Alice current room: " << chat.GetCurrentRoomName(*token_alice) << "\n";

    // пользователи в комнате alice
    std::cout << "Users in Alice's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_alice)) std::cout << "- " << name << "\n";

    // создание комнаты new_room
    chat.CreateRoom("new_room");
    std::cout << "Created room 'new_room'\n";

    // переход alice в fail_room (не существует)
    bool fail_join = chat.JoinRoom(*token_alice, "fail_room");
    std::cout << "Alice joins 'fail_room' (should fail): " << (fail_join ? "FAILED" : "OK") << "\n";

    // переход alice в new_room
    bool join_new = chat.JoinRoom(*token_alice, "new_room");
    std::cout << "Alice joins 'new_room': " << (join_new ? "OK" : "FAILED") << "\n";

    // текущая комната alice
    std::cout << "Alice current room: " << chat.GetCurrentRoomName(*token_alice) << "\n";

    // пользователи в комнате alice
    std::cout << "Users in Alice's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_alice)) std::cout << "- " << name << "\n";

    // регистрация bob
    auto u2 = chat.Register("bob", "bobhash");
    std::cout << "Bob registers: " << (u2 ? "OK" : "FAILED") << "\n";

    // логин bob
    auto token_bob = chat.Login("bob", "bobhash");
    std::cout << "Bob logs in: " << (token_bob ? "OK" : "FAILED") << "\n";

    // все пользователи
    std::cout << "Chat users:\n";
    for (const auto& name : chat.GetOnlineUserNames()) std::cout << "- " << name << "\n";

    // текущая комната bob
    std::cout << "Bob current room: " << chat.GetCurrentRoomName(*token_bob) << "\n";

    // пользователи в комнате bob
    std::cout << "Users in Bob's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_bob)) std::cout << "- " << name << "\n";

    // пользователи в комнате alice
    std::cout << "Users in Alice's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_alice)) std::cout << "- " << name << "\n";

    // bob переходит в new_room
    chat.JoinRoom(*token_bob, "new_room");

    // все пользователи
    std::cout << "Chat users:\n";
    for (const auto& name : chat.GetOnlineUserNames()) std::cout << "- " << name << "\n";

    // текущая комната bob
    std::cout << "Bob current room: " << chat.GetCurrentRoomName(*token_bob) << "\n";

    // пользователи в комнате bob
    std::cout << "Users in Bob's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_bob)) std::cout << "- " << name << "\n";

    // пользователи в комнате alice
    std::cout << "Users in Alice's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_alice)) std::cout << "- " << name << "\n";

    // alice делает leave-room
    chat.LeaveRoom(*token_alice);
    std::cout << "Alice left her room\n";

    // все пользователи
    std::cout << "Chat users:\n";
    for (const auto& name : chat.GetOnlineUserNames()) std::cout << "- " << name << "\n";

    // текущая комната alice
    std::cout << "Alice current room: " << chat.GetCurrentRoomName(*token_alice) << "\n";

    // пользователи в комнате bob
    std::cout << "Users in Bob's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_bob)) std::cout << "- " << name << "\n";

    // пользователи в комнате alice
    std::cout << "Users in Alice's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_alice)) std::cout << "- " << name << "\n";

    // bob → general
    chat.JoinRoom(*token_bob, "general");

    // текущая комната bob
    std::cout << "Bob current room: " << chat.GetCurrentRoomName(*token_bob) << "\n";

    // пользователи в комнате bob
    std::cout << "Users in Bob's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_bob)) std::cout << "- " << name << "\n";

    // все пользователи
    std::cout << "Chat users:\n";
    for (const auto& name : chat.GetOnlineUserNames()) std::cout << "- " << name << "\n";

    // alice выходит
    chat.Logout(*token_alice);
    std::cout << "Alice logged out\n";

    // пользователи в комнате bob
    std::cout << "Users in Bob's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_bob)) std::cout << "- " << name << "\n";

    // все пользователи
    std::cout << "Chat users:\n";
    for (const auto& name : chat.GetOnlineUserNames()) std::cout << "- " << name << "\n";

    // bob → new_room
    chat.JoinRoom(*token_bob, "new_room");
    std::cout << "Bob joined 'new_room' again\n";

    // пользователи в комнате bob
    std::cout << "Users in Bob's room:\n";
    for (const auto& name : chat.GetUserNamesInCurrentRoom(*token_bob)) std::cout << "- " << name << "\n";

    // все пользователи
    std::cout << "Chat users:\n";
    for (const auto& name : chat.GetOnlineUserNames()) std::cout << "- " << name << "\n";

    // bob выходит
    chat.Logout(*token_bob);
    std::cout << "Bob logged out\n";

    return 0;
}
