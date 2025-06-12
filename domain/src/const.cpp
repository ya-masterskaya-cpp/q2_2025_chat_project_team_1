#include"const.h"
const std::string CONSTANTS::ACT_DISCONNECT = "DISCONNECT";
const std::string CONSTANTS::ACT_GET_USERS = "GET_USERS";
const std::string CONSTANTS::ACT_SEND_MESSAGE = "SEND_MESSAGE";
const std::string CONSTANTS::ACT_LOGIN = "LOGIN";
const std::string CONSTANTS::ACT_CREATE_ROOM = "CREATE_ROOM";
const std::string CONSTANTS::ACT_ROOM_LIST = "ROOM_LIST";
const std::string CONSTANTS::ACT_CREATE_USER = "CREATE_USER"; 
const std::string CONSTANTS::ACT_EARLIER_MESS = "EARLIER_MESS";
const std::string CONSTANTS::ACT_USER_MESSAGE = "ACT_USER_MESSAGE"; 

// Map Parameters
const std::string CONSTANTS::LF_ACTION = "ACTION";
const std::string CONSTANTS::LF_NAME = "NAME";
const std::string CONSTANTS::LF_TOKEN = "TOKEN";
const std::string CONSTANTS::LF_MESSAGE = "MESSAGE";
const std::string CONSTANTS::LF_REASON = "REASON";
const std::string CONSTANTS::LF_RESULT = "RESULT";
const std::string CONSTANTS::LF_INITIATOR = "INITIATOR";
const std::string CONSTANTS::LF_USERS = "USERS";
const std::string CONSTANTS::LF_ROOMNAME = "ROOMNAME";
const std::string CONSTANTS::LF_LOGIN = "LOGIN";
const std::string CONSTANTS::LF_ROOMLIST = "ROOMLIST";
const std::string CONSTANTS::LF_PASSWORD = "PASSWORD";
const std::string CONSTANTS::LF_LAST_MSG = "LF_LAST_MSG"; 

const std::string CONSTANTS::RF_SUCCESS = "SUCCESS";
const std::string CONSTANTS::RF_ERROR = "ERROR";

//
const std::string CONSTANTS::RF_ERR_INITIATOR_SERVER = "INITIATOR_SERVER";
const std::string CONSTANTS::RF_ERR_INITIATOR_CHATROOM = "INITIATOR_CLENT";
const std::string CONSTANTS::RF_ERR_PERMISSION_DENIDED = "YOUR TOKEN INCORRECT. PERMISSION DENIDED";

const std::string CONSTANTS::LF_DIRECTION = "DIRECTION";
const std::string CONSTANTS::RF_DIRECTION_SERVER = "DIRECTION_SERVER";
const std::string CONSTANTS::RF_DIRECTION_CHATROOM = "DIRECTION_CHATROOM";

const std::string CONSTANTS::IP = "IP"; 
const std::string CONSTANTS::PORT = "PORT"; 
const std::string CONSTANTS::CHATROOMS = "DEFAULT_CHATROOMS"; 
const std::string CONSTANTS::UNKNOWN = "UNKNOWN";


const size_t CONSTANTS::N_TOKEN_LEN = 32;
const size_t CONSTANTS::N_MAX_MESSAGE_LEN = 512;

const char CONSTANTS::SERIAL_SYM = '\0';

// Log Tags
const std::string LogTag::ServerStart = "ServerStart";
const std::string LogTag::ServerStop = "ServerStop";
const std::string LogTag::Listening = "Listening";
const std::string LogTag::ClientConnect = "ClientConnect";
const std::string LogTag::ClientDisconnect = "ClientDisconnect";
const std::string LogTag::AuthSuccess = "AuthSuccess";
const std::string LogTag::AuthFail = "AuthFail";
const std::string LogTag::CreateRoom = "CreateRoom";
const std::string LogTag::ListRooms = "ListRooms";
const std::string LogTag::ListUsers = "ListUsers";
const std::string LogTag::UserJoinRoom = "UserJoinRoom";
const std::string LogTag::UserLeaveRoom = "UserLeaveRoom";
const std::string LogTag::MessageSent = "MessageSent";
const std::string LogTag::MessageRejected = "MessageRejected";
const std::string LogTag::ExecuteCommand = "ExecuteCommand";
const std::string LogTag::ReceiveRequest = "ReceiveRequest";
const std::string LogTag::FatalError = "FatalError";
const std::string LogTag::ParseError = "ParseError";