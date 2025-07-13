#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "message_handler.h"
extern std::string IP_GLB;
extern std::string PORT_GLB;


class MessageHandlerTest : public ::testing::Test {
public:
    MessageHandlerTest() : msg_handler{user_data,  IP_GLB + ":" +  PORT_GLB} {

    }

    domain::UserData user_data;
    domain::MessageHandler msg_handler;
    Json::Value val;
    Json::CharReaderBuilder readerBuilder;
    std::string parseErrors;
};


TEST_F(MessageHandlerTest, RegisterUser) {
    const std::string name  = "test";
    auto res = msg_handler.RegisterUser(name, "password_test");


    if(res.status) {
        std::stringstream jsonStream(res.msg);
        Json::parseFromStream(readerBuilder, jsonStream, &val, &parseErrors);

        EXPECT_EQ("Registration successful: " + name,val["info"].asString());
    } else {
        std::stringstream jsonStream(res.msg);
        Json::parseFromStream(readerBuilder, jsonStream, &val, &parseErrors);

        EXPECT_EQ("User " + name  + " already registered",val["error"].asString());
    }
}

TEST_F(MessageHandlerTest, LoginUser_Logout) {
    const std::string name  = "test4";
    const std::string password = "password_test";
    msg_handler.RegisterUser(name, password);

    //LOGIN
    auto res = msg_handler.LoginUser(name, password);

    std::stringstream jsonStream(res.msg);
    Json::parseFromStream(readerBuilder, jsonStream, &val, &parseErrors);

    EXPECT_EQ(name,val["user"].asString());
    EXPECT_EQ(user_data.token,val["token"].asString());

    //TRY SECOND LOGIN
    auto res_second_try = msg_handler.LoginUser(name, password);

    std::stringstream jsonStream_try_login(res_second_try.msg);
    Json::parseFromStream(readerBuilder, jsonStream_try_login, &val, &parseErrors);

    EXPECT_EQ("User is not registered or User already logged in",val["error"].asString());

    //LOGOUT
    auto res_out = msg_handler.LogoutUser();
    std::stringstream jsonStream_out(res_out.msg);
    Json::parseFromStream(readerBuilder, jsonStream_out, &val, &parseErrors);

    EXPECT_EQ("Logged out",val["info"].asString());
}

TEST_F(MessageHandlerTest, Rooms_Create) {
    const std::string name  = "test6";
    const std::string password = "password_test";
    msg_handler.RegisterUser(name, password);
    msg_handler.LoginUser(name, password);

    const std::string room_name = "test_room_name_2";

    //CREATE ROOM
    auto res = msg_handler.CreateRoom(room_name);

    if(res.status) {
        std::stringstream jsonStream(res.msg);
        Json::parseFromStream(readerBuilder, jsonStream, &val, &parseErrors);

        EXPECT_EQ("Room created",val["info"].asString());
    } else {
        std::stringstream jsonStream(res.msg);
        Json::parseFromStream(readerBuilder, jsonStream, &val, &parseErrors);

        EXPECT_EQ("Room already created",val["error"].asString());
    }

    //SECOND CREATE SAME ROOM
    auto res_second_try = msg_handler.CreateRoom(room_name);
    std::stringstream jsonStream_second_try(res_second_try.msg);
    Json::parseFromStream(readerBuilder, jsonStream_second_try, &val, &parseErrors);

    EXPECT_EQ("Room already created",val["error"].asString());

    msg_handler.LogoutUser();
}

TEST_F(MessageHandlerTest, Rooms_Join_Leave) {
    const std::string name  = "test9";
    const std::string password = "password_test";
    msg_handler.RegisterUser(name, password);
    msg_handler.LoginUser(name, password);

    const std::string room_name = "test_room_name_3";
    msg_handler.CreateRoom(room_name);

    //JOIN ROOM
    auto res = msg_handler.JoinRoom(room_name);
    std::stringstream jsonStream(res.msg);
    Json::parseFromStream(readerBuilder, jsonStream, &val, &parseErrors);

    EXPECT_EQ(room_name,val["info"].asString());

    //TRY JOIN
    auto try_join = msg_handler.JoinRoom("sfsdg2345wtesgdfvhsertsdfg2345345rsdgvrdhe54");
    std::stringstream jsonStream_try_join(try_join.msg);
    Json::parseFromStream(readerBuilder, jsonStream_try_join, &val, &parseErrors);

    EXPECT_EQ("Invalid room join",val["error"].asString());

    //LEAVE
    auto leave = msg_handler.LeaveRoom();
    std::stringstream jsonStream_leave(leave.msg);
    Json::parseFromStream(readerBuilder, jsonStream_leave, &val, &parseErrors);

    EXPECT_EQ("Left to general room",val["info"].asString());


    msg_handler.LogoutUser();
}

TEST_F(MessageHandlerTest, Rooms_List) {
    const std::string name  = "test10";
    const std::string password = "password_test";
    msg_handler.RegisterUser(name, password);
    msg_handler.LoginUser(name, password);

    const std::string room_name = "test_room_name_4";
    msg_handler.CreateRoom(room_name);

    auto res = msg_handler.ListRooms();

    bool is_room_exists{false};

    std::stringstream jsonStream(res.msg);
    Json::parseFromStream(readerBuilder, jsonStream, &val, &parseErrors);

    for(auto& room : val) {
        if(room.asString() == room_name) {
            is_room_exists = true;
        }
    }

    EXPECT_TRUE(is_room_exists);
    msg_handler.LogoutUser();
}

TEST_F(MessageHandlerTest, Rooms_Persons_List) {
    const std::string name  = "test11";
    const std::string password = "password_test";
    msg_handler.RegisterUser(name, password);
    msg_handler.LoginUser(name, password);

    const std::string room_name = "test_room_name_5";
    msg_handler.CreateRoom(room_name);
    msg_handler.JoinRoom(room_name);

    //PERSON CHECK
    auto res = msg_handler.GetUsersInRoom(room_name);
    bool is_person_exists{false};

    std::stringstream jsonStream(res.msg);
    Json::parseFromStream(readerBuilder, jsonStream, &val, &parseErrors);

    for(auto& person : val) {
        if(person.asString() == name) {
            is_person_exists = true;
        }
    }
    EXPECT_TRUE(is_person_exists);

    //ROOM DOESNOT EXIST CHECK
    auto try_room = msg_handler.GetUsersInRoom("sdfgsrgsrg345terstvw4ctq3t4vwgedm9");

    std::stringstream jsonStream_try_room(try_room.msg);
    Json::parseFromStream(readerBuilder, jsonStream_try_room, &val, &parseErrors);

    EXPECT_EQ("Room not found",val["error"].asString());

    msg_handler.LogoutUser();
}

TEST_F(MessageHandlerTest, Messages) {
    size_t messages_sum = 10;
    const std::string name  = "test13";
    const std::string password = "password_test";
    msg_handler.RegisterUser(name, password);
    msg_handler.LoginUser(name, password);

    //SEND AND UPLOAD MESSAGES
    for(size_t idx = 0; idx < messages_sum; ++idx) {
        auto res = msg_handler.SendMessage("msg " + std::to_string(idx));

        std::stringstream jsonStream(res.msg);
        Json::parseFromStream(readerBuilder, jsonStream, &val, &parseErrors);

        EXPECT_EQ("Message from " + name + " sent", val["info"].asString());

        auto res_upload = msg_handler.UploadMessageToDB("msg " + std::to_string(idx));

        if(res_upload.status) {
            std::stringstream jsonStream_upload(res_upload.msg);
            Json::parseFromStream(readerBuilder, jsonStream_upload, &val, &parseErrors);

            EXPECT_EQ("Message saved to DB", val["info"].asString());
        }
    }

    //RECENT MESSAGES
    auto res_recent = msg_handler.GetRoomsRecentMessages("general",10);

    std::stringstream jsonStream_recent(res_recent.msg);
    Json::parseFromStream(readerBuilder, jsonStream_recent, &val, &parseErrors);

    size_t idx{0};
    for(auto& message : val) {
        EXPECT_EQ(name,message["from"].asString());
        EXPECT_EQ("msg " + std::to_string(idx), message["text"].asString());
        ++idx;
    }

    msg_handler.LogoutUser();
}

