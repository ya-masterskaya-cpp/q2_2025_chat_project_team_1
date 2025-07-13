#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "websocket_client.h"

using namespace transfer;
using namespace ix;

class WebSocketClientTest : public ::testing::Test {
protected:
    WebSocketClientTest() : client{"127.0.0.1", 8080} {
    }

    WebSocketClient client;
};

TEST_F(WebSocketClientTest, WebSocketDefault) {
    auto ws_ptr = client.GetWebSocket();
    ASSERT_EQ(ws_ptr,nullptr);
}

TEST_F(WebSocketClientTest, OnRun) {
    client.Run();
    auto ws_ptr = client.GetWebSocket();

    ASSERT_NE(ws_ptr,nullptr);
    EXPECT_EQ(ws_ptr->getUrl(), "ws://127.0.0.1:8080/ws/chat");
}

TEST_F(WebSocketClientTest, OnStop) {
    client.Run();
    auto ws_ptr = client.GetWebSocket();

    ASSERT_NE(ws_ptr,nullptr);
    EXPECT_EQ(ws_ptr->getUrl(), "ws://127.0.0.1:8080/ws/chat");

    client.Stop();
    auto ws_ptr1 = client.GetWebSocket();
    ASSERT_EQ(ws_ptr1,nullptr);
}

TEST_F(WebSocketClientTest, SetToken) {
    client.SetToken("test_token");
    client.Run();

    auto ws_ptr = client.GetWebSocket();
    EXPECT_EQ(ws_ptr->getUrl(), "ws://127.0.0.1:8080/ws/chat?token=test_token");
}
