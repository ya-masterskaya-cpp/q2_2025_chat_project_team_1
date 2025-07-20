# Документация для классов Query и QueryHandler

## Обзор

Классы `Query` и `QueryHandler` предоставляют гибкую систему для работы с HTTP-запросами, позволяя настраивать заголовки, тело запроса, параметры и функции обработки. Эти шаблонные классы работают с различными типами для тела запроса, заголовков и параметров.

## Класс: Query<Body, Header, Parameters>

Шаблонный класс, представляющий HTTP-запрос с настраиваемыми компонентами.

### Параметры шаблона
- `Body`: Тип для тела запроса
- `Header`: Тип для заголовков запроса (обычно `std::unordered_map<std::string, std::string>`)
- `Parameters`: Тип для параметров запроса (обычно `std::unordered_map<std::string, std::string>`)

### Публичные методы

#### Управление токеном
```cpp
Query& SetToken(const std::string& token)
```
Устанавливает токен авторизации в заголовок с префиксом "Bearer ".  
**Возвращает:** Ссылку на себя для цепочки вызовов.

#### Управление заголовками
```cpp
Query& SetHeader(Header&& header)
```
Устанавливает полный объект заголовков.  
**Возвращает:** Ссылку на себя для цепочки вызовов.

```cpp
const Header& GetHeader() const
```
**Возвращает:** Константную ссылку на текущие заголовки.

#### Управление телом запроса
```cpp
Query& SetBody(Body&& body)
```
Устанавливает тело запроса.  
**Возвращает:** Ссылку на себя для цепочки вызовов.

```cpp
const Body& GetBody() const
```
**Возвращает:** Константную ссылку на текущее тело запроса.

#### Управление параметрами
```cpp
Query& SetParameters(Parameters&& parameters)
```
Устанавливает параметры запроса.  
**Возвращает:** Ссылку на себя для цепочки вызовов.

```cpp
const Parameters& GetParameters() const
```
**Возвращает:** Константную ссылку на текущие параметры.

#### Управление конечной точкой
```cpp
Query& SetEndpoint(std::string_view endpoint)
```
Устанавливает путь конечной точки (относительно базового URL).  
**Возвращает:** Ссылку на себя для цепочки вызовов.

```cpp
std::string_view GetEndpoint() const
```
**Возвращает:** Текущий путь конечной точки.

#### Функция обработки
```cpp
Query& SetProcessFunction(ProcessFunction process_function)
```
Устанавливает функцию, которая будет вызвана при обработке запроса.  
**Возвращает:** Ссылку на себя для цепочки вызовов.

```cpp
void Process()
```
Выполняет зарегистрированную функцию обработки.

### Приватные поля
- `endpoint_`: Путь к API конечной точки
- `body_`: Тело запроса
- `header_`: Заголовок запроса
- `parameters_`: Параметры запроса
- `process_function_`: Функция для выполнения при обработке запроса

## Класс: QueryHandler<Body, Header, Parameters>

Шаблонный класс для управления несколькими экземплярами Query для разных конечных точек.

### Публичные методы

#### Управление конечными точками
```cpp
Query<Body,Header,Parameters>* AddEndpoint(std::string_view endpoint)
```
Добавляет новую конечную точку в обработчик.  
**Возвращает:** Указатель на созданный объект Query.

```cpp
Query<Body,Header,Parameters>& At(std::string_view endpoint)
```
**Возвращает:** Ссылку на объект Query для указанной конечной точки.  

#### Управление URL
```cpp
void SetUrl(const std::string& url)
```
Устанавливает базовый URL для всех конечных точек.

```cpp
const std::string& GetUrl() const
```
**Возвращает:** Текущий базовый URL.

### Приватные поля
- `endpoints_`: Словарь путей конечных точек к объектам Query
- `url_`: Базовый URL для всех конечных точек

## Пример использования

```cpp
using Header = std::unordered_map<std::string, std::string>;
using Body = std::string;
using Parameters = std::unordered_map<std::string, std::string>;

domain::QueryHandler<Body, Header, Parameters> handler;
handler.SetUrl("https://api.example.com");

// Добавление конечной точки
auto* query = handler.AddEndpoint("/users");
query->SetToken("abc123")
     .SetBody(R"({"name":"John"})")
     .SetProcessFunction([](){
         std::cout << "Обработка запроса пользователя\n";
     });

// Обработка запроса
handler.At("/users").Process();
```

## Примечания

1. Классы поддерживают цепочки вызовов (fluent interface)
2. Методы работы с заголовками предполагают, что заголовки хранятся в структуре типа словаря
3. Реальная отправка HTTP-запроса должна быть реализована в ProcessFunction

# Документация для класса `MessageHandler`

## Общее описание
Класс `MessageHandler` предназначен для обработки сообщений и взаимодействия с сервером через REST API. Он предоставляет методы для аутентификации пользователей, работы с сообщениями и комнатами, а также управления пользовательскими данными.

## Публичные методы

### Конструктор
```cpp
MessageHandler(UserData& user, const std::string& url)
```
- **Параметры:**
  - `user` - ссылка на объект `UserData`, содержащий данные пользователя.
  - `url` - базовый URL сервера для REST API.
- **Описание:** Инициализирует обработчик сообщений, устанавливает базовый URL и настраивает конечные точки API.

---

### Аутентификация
```cpp
ServerResponse RegisterUser(const std::string& login, const std::string& password)
```
- **Параметры:**
  - `login` - логин пользователя.
  - `password` - пароль пользователя (хешируется перед отправкой).
- **Возвращает:** `ServerResponse` с результатом регистрации.
- **Описание:** Регистрирует нового пользователя на сервере.

---

```cpp
ServerResponse LoginUser(const std::string& login, const std::string& password)
```
- **Параметры:**
  - `login` - логин пользователя.
  - `password` - пароль пользователя (хешируется перед отправкой).
- **Возвращает:** `ServerResponse` с результатом входа и токеном аутентификации.
- **Описание:** Аутентифицирует пользователя и сохраняет токен в `UserData`.

---

```cpp
ServerResponse LogoutUser()
```
- **Возвращает:** `ServerResponse` с результатом выхода.
- **Описание:** Завершает сеанс пользователя и очищает токен.

---

### Пользователи
```cpp
ServerResponse GetOnlineUsers()
```
- **Возвращает:** `ServerResponse` со списком онлайн-пользователей.
- **Описание:** Получает список пользователей, находящихся в сети.

---

### Сообщения
```cpp
ServerResponse SendMessage(const std::string& text, const std::string& to = "")
```
- **Параметры:**
  - `text` - текст сообщения.
  - `to` (опционально) - получатель сообщения (если пусто, сообщение отправляется всем).
- **Возвращает:** `ServerResponse` с результатом отправки.
- **Описание:** Отправляет сообщение указанному пользователю или всем (broadcast).

---

```cpp
ServerResponse GetRoomsRecentMessages(const std::string& room_name, size_t messages_count = 10)
```
- **Параметры:**
  - `room_name` - название комнаты.
  - `messages_count` (опционально) - количество сообщений для получения (по умолчанию 10).
- **Возвращает:** `ServerResponse` с последними сообщениями в комнате.
- **Описание:** Получает последние сообщения из указанной комнаты.

---

```cpp
ServerResponse UploadMessageToDB(const std::string& message)
```
- **Параметры:**
  - `message` - текст сообщения для сохранения.
- **Возвращает:** `ServerResponse` с результатом сохранения.
- **Описание:** Сохраняет сообщение в базе данных.

---

### Комнаты
```cpp
ServerResponse CreateRoom(const std::string& name)
```
- **Параметры:**
  - `name` - название комнаты.
- **Возвращает:** `ServerResponse` с результатом создания.
- **Описание:** Создает новую комнату.

---

```cpp
ServerResponse JoinRoom(const std::string& name)
```
- **Параметры:**
  - `name` - название комнаты.
- **Возвращает:** `ServerResponse` с результатом входа.
- **Описание:** Присоединяет пользователя к указанной комнате.

---

```cpp
ServerResponse LeaveRoom()
```
- **Возвращает:** `ServerResponse` с результатом выхода.
- **Описание:** Покидает текущую комнату.

---

```cpp
ServerResponse ListRooms()
```
- **Возвращает:** `ServerResponse` со списком доступных комнат.
- **Описание:** Получает список всех комнат.

---

```cpp
ServerResponse GetCurrentRoom()
```
- **Возвращает:** `ServerResponse` с названием текущей комнаты.
- **Описание:** Получает название комнаты, в которой находится пользователь.

---

```cpp
ServerResponse GetUsersInRoom(const std::string& roomName)
```
- **Параметры:**
  - `roomName` - название комнаты.
- **Возвращает:** `ServerResponse` со списком пользователей в комнате.
- **Описание:** Получает список пользователей в указанной комнате.

---

### Настройки
```cpp
void SetUrl(const std::string& url)
```
- **Параметры:**
  - `url` - новый базовый URL сервера.
- **Описание:** Устанавливает новый базовый URL для REST API.

---

## Приватные методы
```cpp
bool ParseTokenFromJson(const std::string& jsonText)
```
- **Параметры:**
  - `jsonText` - JSON-строка, содержащая токен.
- **Возвращает:** `true`, если токен успешно извлечен и сохранен, иначе `false`.
- **Описание:** Извлекает токен из JSON-ответа сервера и сохраняет его в `UserData`.

---

```cpp
template<typename Function>
ServerResponse ToRequest(Function function, int code = 200)
```
- **Параметры:**
  - `function` - функция, выполняющая запрос.
  - `code` (опционально) - ожидаемый HTTP-код ответа (по умолчанию 200).
- **Возвращает:** `ServerResponse` с результатом запроса.
- **Описание:** Обрабатывает результат запроса и формирует ответ.

---

## Пример использования
```cpp
UserData user;
MessageHandler handler(user, "http://example.com/api");

// Регистрация и вход
handler.RegisterUser("user1", "password123");
handler.LoginUser("user1", "password123");

// Отправка сообщения
handler.SendMessage("Привет, мир!", "user2");

// Создание комнаты
handler.CreateRoom("chat_room");

// Получение списка комнат
auto roomsResponse = handler.ListRooms();
if (roomsResponse.success) {
    std::cout << "Список комнат: " << roomsResponse.response << std::endl;
}
```

## Зависимости
- Для работы с JSON используется библиотека `jsoncpp`.
- Для HTTP-запросов используется библиотека `cpr`.
- Класс зависит от `UserData` и `QueryHandler` (определены в других модулях).

# Документация для класса WebSocketClient

## Класс WebSocketClient

Класс WebSocketClient реализует клиентское соединение WebSocket, наследуясь от ClientInterface. Предоставляет функциональность для установки соединения, обработки сообщений и событий WebSocket.

### Заголовочный файл
```cpp
#include "websocket_client.h"
```

### Конструктор
```cpp
WebSocketClient(const std::string& ip, int port, const std::string& token = "")
```
Создает экземпляр WebSocket клиента.
- `ip` - IP-адрес сервера
- `port` - порт сервера
- `token` - токен аутентификации (опционально)

### Основные методы

#### `Run()`
```cpp
virtual void Run() override
```
Запускает WebSocket соединение. Устанавливает обработчики событий и начинает прослушивание сообщений.

#### `Stop()`
```cpp
virtual void Stop() override
```
Останавливает WebSocket соединение и освобождает ресурсы.

#### `SetToken(const std::string& token)`
```cpp
void SetToken(const std::string& token)
```
Устанавливает токен аутентификации.
- `token` - строка с токеном

### Методы установки обработчиков событий

#### `SetOnMessage(Callback callback)`
```cpp
void SetOnMessage(Callback callback)
```
Устанавливает обработчик входящих сообщений.
- `callback` - функция вида `void(const std::string& msg)`

#### `SetOnOpen(Callback callback)`
```cpp
void SetOnOpen(Callback callback)
```
Устанавливает обработчик события открытия соединения.
- `callback` - функция вида `void(const std::string& msg)`

#### `SetOnClose(Callback callback)`
```cpp
void SetOnClose(Callback callback)
```
Устанавливает обработчик события закрытия соединения.
- `callback` - функция вида `void(const std::string& msg)`

#### `SetOnError(Callback callback)`
```cpp
void SetOnError(Callback callback)
```
Устанавливает обработчик ошибок соединения.
- `callback` - функция вида `void(const std::string& msg)`

### Методы управления WebSocket

#### `SetWebSocket(std::unique_ptr<ix::WebSocket>&& ws_client)`
```cpp
void SetWebSocket(std::unique_ptr<ix::WebSocket>&& ws_client)
```
Устанавливает пользовательский экземпляр WebSocket.
- `ws_client` - unique_ptr на объект WebSocket

#### `SetWebSocket(ix::WebSocket* ws_client)`
```cpp
void SetWebSocket(ix::WebSocket* ws_client)
```
Устанавливает пользовательский экземпляр WebSocket (сырой указатель).
- `ws_client` - указатель на объект WebSocket

#### `GetWebSocket()`
```cpp
const ix::WebSocket* const GetWebSocket() const
```
Возвращает указатель на текущий экземпляр WebSocket.

### Приватные поля
- `ws_client_` - unique_ptr на экземпляр WebSocket
- `url_` - URL WebSocket соединения
- `token_` - токен аутентификации
- `msg_handler_` - обработчик входящих сообщений
- `open_handler_` - обработчик открытия соединения
- `close_handler_` - обработчик закрытия соединения
- `error_handler_` - обработчик ошибок

### Пример использования
```cpp
WebSocketClient client("127.0.0.1", 8080, "my_token");

client.SetOnMessage([](const std::string& msg) {
    std::cout << "Received: " << msg << std::endl;
});

client.SetOnOpen([](const std::string& msg) {
    std::cout << "Connection opened: " << msg << std::endl;
});

client.Run();
// ...
client.Stop();
```

### Примечания
1. Класс использует библиотеку ixwebsocket для реализации WebSocket функциональности.
2. Все обработчики событий вызываются в потоке WebSocket.
3. Для аутентификации токен добавляется в URL как query-параметр.
