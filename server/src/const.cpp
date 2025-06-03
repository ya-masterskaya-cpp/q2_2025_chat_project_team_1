#include"const.h"
const std::string CONSTANTS::ACT_DISCONNECT = "DISCONNECT";
const std::string CONSTANTS::ACT_GET_USERS = "GET_USERS";
const std::string CONSTANTS::ACT_SEND_MESSAGE = "SEND_MESSAGE";
const std::string CONSTANTS::ACT_LOGIN = "LOGIN";
const std::string CONSTANTS::ACT_CREATE_ROOM = "CREATE_ROOM";
const std::string CONSTANTS::ACT_ROOM_LIST = "ROOM_LIST";
const std::string CONSTANTS::ACT_CREATE_USER = "CREATE_USER"; 
const std::string CONSTANTS::ACT_EARLIER_MESS = "EARLIER_MESS";

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
const std::string CONSTANTS::CHATROOMS = "CHATROOMS"; 



const size_t CONSTANTS::N_DISCONNECT = 3;       // direction, action, token
const size_t CONSTANTS::N_SEND_MESSAGE = 4; // direction, action, token, message

const size_t CONSTANTS::N_TOKEN_LEN = 32;
const size_t CONSTANTS::N_MAX_MESSAGE_LEN = 512;


const size_t CONSTANTS::N_GET_USERS = 3; // direction, action, roomname

// IN WORK
const size_t CONSTANTS::N_LOGIN = 6;   // direction, action, roomname, username, password (token adds server)
const size_t CONSTANTS::N_CREATE_ROOM = 0; // direction, action,
const size_t CONSTANTS::N_ROOM_LIST = 2;   // direction, action,
const size_t CONSTANTS::N_CREATE_USER = 0; // direction, action,