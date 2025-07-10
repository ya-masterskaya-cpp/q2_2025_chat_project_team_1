#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "websocket_client.h"

using namespace transfer;
using namespace ix;
using ::testing::_;
using ::testing::MockFunction;
using ::testing::StrictMock;
using ::testing::SaveArg;

// Mock класс для ix::WebSocket
class MockWebSocket : public ix::WebSocket {
public:
    MOCK_METHOD(void, setUrl, (const std::string& url));
    MOCK_METHOD(void, setOnMessageCallback, (const OnMessageCallback& callback));
    MOCK_METHOD(void, start, ());
    MOCK_METHOD(void, stop, ());
};

class WebSocketClientTest : public ::testing::Test {
protected:
    void SetUp() override {
        mock_ws = new MockWebSocket();
        client.SetWebSocket(mock_ws);
    }

    void TearDown() override {
    }

    MockWebSocket* mock_ws;
    WebSocketClient client{"127.0.0.1", 8080, "test_token"};
};


