# IRC Chat Database Layer (C++ / PostgreSQL)

## 📦 Описание

Этот модуль реализует слой работы с базой данных для IRC-чата с поддержкой PostgreSQL, пулом соединений, идентификаторами на основе UUID и репозиториями для пользователей, комнат, сообщений и участников комнат.  
Вся бизнес-логика отделена от слоя хранения данных (repository pattern).

---

## 🗃️ Структуры и классы

### 1. `util::Tagged`, `util::TaggedUUID`

- **Назначение:** Безопасные идентификаторы на основе UUID с типобезопасностью (UserId, RoomId и др.).
- **Использование:** Предотвращает случайное смешение id разных сущностей.

---

### 2. `postgres::ConnectionPool`

- **Назначение:** Эффективное переиспользование соединений с PostgreSQL из разных потоков.
- **Ключевые методы:**
    - `GetConnection()`
    - `GetConnection(std::chrono::milliseconds timeout)`

---

### 3. `postgres::UserRecord`, `RoomRecord`, `MessageRecord`, `RoomMemberRecord`

- **Назначение:** Cтруктуры для хранения данных, возвращаемых из БД.
- **Поля:** UUID, имена, текст сообщений, время, и т.д.

---

### 4. Репозитории (Repository classes)

#### `UsersRepository`

- **Назначение:** Доступ к таблице пользователей.
- **Основные методы:**
    - `Save(const std::string& username, const std::string& password_hash)`
    - `std::vector<UserRecord> LoadAll() const`
    - `std::optional<UserRecord> FindByUsername(const std::string&) const`

#### `RoomsRepository`

- **Назначение:** Доступ к таблице комнат.
- **Методы:** `Save`, `LoadAll`

#### `MessagesRepository`

- **Назначение:** Доступ к сообщениям чата.
- **Методы:** `Save`, `LoadRecent`

#### `RoomMembersRepository`

- **Назначение:** Хранение информации о том, кто состоит в какой комнате.
- **Методы:**
    - `Save(const UserId&, const RoomId&)`
    - `Remove(const UserId&, const RoomId&)`
    - `LoadMembers(const RoomId&)`
    - `LoadRooms(const UserId&)`

---

### 5. `postgres::Database`

- **Назначение:** Инициализация базы данных и создание необходимых таблиц.
- **Метод:** `GetTransaction()` — получить соединение из пула для транзакций.

---

## 🏗️ Структура таблиц

```sql
CREATE TABLE IF NOT EXISTS users (
    id UUID PRIMARY KEY,
    username VARCHAR(100) UNIQUE NOT NULL,
    password_hash VARCHAR(256) NOT NULL,
    registered_at TIMESTAMP NOT NULL DEFAULT NOW()
);
CREATE TABLE IF NOT EXISTS rooms (
    id UUID PRIMARY KEY,
    name VARCHAR(100) UNIQUE NOT NULL,
    created_at TIMESTAMP NOT NULL DEFAULT NOW()
);
CREATE TABLE IF NOT EXISTS messages (
    id UUID PRIMARY KEY,
    user_id UUID NOT NULL REFERENCES users(id),
    room_id UUID NOT NULL REFERENCES rooms(id),
    message TEXT NOT NULL,
    sent_at TIMESTAMP NOT NULL DEFAULT NOW()
);
CREATE TABLE IF NOT EXISTS room_members (
    id UUID PRIMARY KEY,
    room_id UUID NOT NULL REFERENCES rooms(id),
    user_id UUID NOT NULL REFERENCES users(id),
    joined_at TIMESTAMP NOT NULL DEFAULT NOW(),
    UNIQUE(room_id, user_id)
);
```

---

## 🛠️ Примеры использования

### Инициализация пула и базы

```cpp
auto db = std:: make_shared<postgres::Database>(std::make_shared<postgres::ConnectionPool>(num_threads, []() {
    return std::make_shared<pqxx::connection>("<Строка подключения к БД>");
}));
```

### Добавление пользователя с хешированием пароля

```cpp
#include <openssl/sha.h>
#include <iomanip>
#include <sstream>

std::string HashPassword(const std::string& password) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256(reinterpret_cast<const unsigned char*>(password.c_str()), password.size(), hash);
    std::ostringstream oss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        oss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
    return oss.str();
}

auto conn = db.GetTransaction();
pqxx::work tx(*conn);
UsersRepository users_repo(tx);
users_repo.Save("alice", HashPassword("secure_password"));
tx.commit();
```

### Проверка пароля пользователя

```cpp
pqxx::work tx(*conn);
UsersRepository users_repo(tx);
auto user = users_repo.FindByUsername("alice");
if (user && user->password_hash == HashPassword("secure_password")) {
    // Успешная авторизация
}
```

### Работа с комнатами и участниками

```cpp
RoomsRepository rooms_repo(tx);
rooms_repo.Save("general");
auto rooms = rooms_repo.LoadAll();

RoomMembersRepository members_repo(tx);
members_repo.Save(user->id, rooms[0].id); // Добавить пользователя в комнату

auto members = members_repo.LoadMembers(rooms[0].id); // Все участники комнаты
```

### Сохранение и получение сообщений

```cpp
MessagesRepository messages_repo(tx);
messages_repo.Save(user->id, rooms[0].id, "Hello, world!");
auto recent = messages_repo.LoadRecent(rooms[0].id, 10);
```

---

## 📚 Архитектурные принципы

- **Repository Pattern:** каждый репозиторий занимается только своей сущностью и не содержит бизнес-логики.
- **Thread-safe Connection Pool:** позволяет использовать базу в многопоточной среде.
- **UUID Everywhere:** для идентификаторов используется UUID с тэгом (типы UserId, RoomId и т.д.).
- **Хеширование паролей:** пароли не хранятся в открытом виде.

---

## 📝 Пример инициализации и добавления пользователя (полный)

```cpp
auto db = std:: make_shared<postgres::Database>(std::make_shared<postgres::ConnectionPool>(num_threads, []() {
    return std::make_shared<pqxx::connection>("<Строка подключения к БД>");
}));

auto conn = db.GetTransaction();
pqxx::work tx(*conn);
postgres::UsersRepository users_repo(tx);
users_repo.Save("bob", HashPassword("mysecretpass"));
tx.commit();
```

---

## ⚡ Рекомендации

- Для реальных проектов рекомендуем использовать bcrypt/argon2 вместо SHA256 для хеширования паролей.
- Логику аутентификации (проверку пароля) выносить в отдельный слой (auth service).
- Весь код thread-safe, легко масштабируется для большого чата.

---

## 📎 Связанные файлы

- `tagged.h` — шаблон для типобезопасных идентификаторов
- `tagged_uuid.h` — UUID-идентификаторы для всех сущностей
- `postgres.h/.cpp` — репозитории, ConnectionPool и Database

---

*Документация сгенерирована автоматически. Актуально для IRC-чата на C++ с PostgreSQL.*
