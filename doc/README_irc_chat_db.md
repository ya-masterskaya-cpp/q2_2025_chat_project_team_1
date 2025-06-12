# IRC Chat Database Layer (C++ / PostgreSQL)

## 📦 Описание

Модуль реализует слой работы с базой данных для IRC-чата с поддержкой PostgreSQL: пул соединений, идентификаторы на основе UUID, репозитории для пользователей, комнат, сообщений и участников.  
Вся бизнес-логика отделена от слоя хранения данных (реализация *Repository Pattern*).

---

## 🗃️ Структуры и классы

### 1. `util::Tagged`, `util::TaggedUUID`

- **Назначение:** Типобезопасные идентификаторы на основе UUID (например, `UserId`, `RoomId`).
- **Преимущество:** Исключает смешение идентификаторов разных сущностей на этапе компиляции.

---

### 2. `postgres::ConnectionPool`

- **Назначение:** Thread-safe пул соединений с PostgreSQL для работы из разных потоков.
- **Основные методы:**
  - `GetConnection()`
  - `GetConnection(std::chrono::milliseconds timeout)`

---

### 3. `UserRecord`, `RoomRecord`, `MessageRecord`, `RoomMemberRecord`

- **Назначение:** Структуры данных, описывающие сущности для работы с базой (пользователь, комната, сообщение, членство).
- **Поля:** UUID, имена, текст сообщений, временные метки и др.

---

### 4. Репозитории

Каждый репозиторий отвечает за одну сущность:

#### `UsersRepository`
- **Методы:**
  - `Save(const std::string& username, const std::string& password_hash)`
  - `std::vector<UserRecord> LoadAll() const`
  - `std::optional<UserRecord> FindByUsername(const std::string&) const`
  - `std::vector<UserRecord> LoadPage(int offset, int limit) const;`

#### `RoomsRepository`
- **Методы:** `Save`, `LoadAll`, `LoadPage`

#### `MessagesRepository`
- **Методы:** `Save`, `LoadRecent`, `LoadPage`

#### `RoomMembersRepository`
- **Методы:**
  - `Save(const UserId&, const RoomId&)`
  - `Remove(const UserId&, const RoomId&)`
  - `LoadMembers(const RoomId&)`
  - `LoadRooms(const UserId&)`

---

### 5. `postgres::Database`

- **Назначение:** Инициализация и миграция базы данных (создание таблиц), предоставление транзакций.
- **Метод:** `GetTransaction()` — получить соединение для транзакции.

---

## 🔄 Пагинация (постраничная выборка)

В репозиториях пользователей, комнат и сообщений теперь доступны методы **постраничной выборки** (пагинации):

### Методы пагинации

#### `UsersRepository`
- `std::vector<UserRecord> LoadPage(int offset, int limit) const`
    - Загружает пользователей, начиная с позиции `offset`, не более `limit` штук (сортировка: самые новые сначала).

#### `RoomsRepository`
- `std::vector<RoomRecord> LoadPage(int offset, int limit) const`
    - Загружает комнаты постранично по аналогии с пользователями.

#### `MessagesRepository`
- `std::vector<MessageRecord> LoadPage(const RoomId& room_id, int offset, int limit) const`
    - Получает сообщения из заданной комнаты постранично (от новых к старым).

---

### Пример использования пагинации

```cpp
pqxx::work tx(*conn);

// --- Пагинация пользователей
UsersRepository users_repo(tx);
auto users_page = users_repo.LoadPage(0, 20); // первые 20 пользователей

// --- Пагинация комнат
RoomsRepository rooms_repo(tx);
auto rooms_page = rooms_repo.LoadPage(20, 10); // начиная с 21-й комнаты, 10 штук

// --- Пагинация сообщений в комнате
MessagesRepository messages_repo(tx);
auto messages_page = messages_repo.LoadPage(room_id, 0, 50); // первые 50 сообщений в комнате
```

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

### Инициализация

```cpp
auto db = std::make_shared<postgres::Database>(
    std::make_shared<postgres::ConnectionPool>(num_threads, []() {
        return std::make_shared<pqxx::connection>("<Строка подключения>");
    }));
```

### Добавление пользователя

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

auto conn = db->GetTransaction();
pqxx::work tx(*conn);
UsersRepository users_repo(tx);
users_repo.Save("alice", HashPassword("secure_password"));
tx.commit();
```

### Проверка пароля

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
auto members = members_repo.LoadMembers(rooms[0].id);
```

### Сообщения

```cpp
MessagesRepository messages_repo(tx);
messages_repo.Save(user->id, rooms[0].id, "Hello, world!");
auto recent = messages_repo.LoadRecent(rooms[0].id, 10);
```

---

## 🧪 Тестирование

### Сведения

- Покрытие: все основные репозитории и бизнес-операции.
- Проверка: создание, выборка, корректность данных, ошибки, граничные ситуации.

### Для тестирования

- Используется изолированная тестовая база (Docker-контейнер легко сбрасывать между тестами).
- Не использовать production-базу для тестов!

### 🐳 Использование Docker для базы данных

Контейнер с PostgreSQL разворачивается с помощью Docker Compose.

#### Конфигурация

В корне проекта: [`docker-compose.yml`](./docker-compose.yml).

##### Пример содержимого:

```yaml
version: "3.8"
services:
  postgres:
    image: postgres:15
    container_name: irc_chat_postgres
    environment:
      POSTGRES_USER: test_user
      POSTGRES_PASSWORD: test_password
      POSTGRES_DB: test_db
    ports:
      - "5432:5432"
    volumes:
      - pgdata:/var/lib/postgresql/data
volumes:
  pgdata:
```

#### Запуск и остановка

- **Запуск:** `docker compose up -d`
- **Остановка:** `docker compose down`
- **Удаление данных:** `docker compose down -v`

#### Подключение из кода

```cpp
pqxx::connection conn("host=localhost port=5432 user=test_user password=test_password dbname=test_db");
```

---

#### Сборка и запуск тестов

1. **Сборка:**
    ```sh
    mkdir build
    cd build
    cmake ..
    make
    ```
   (Если используешь vcpkg, добавь `-DCMAKE_TOOLCHAIN_FILE=.../vcpkg/scripts/buildsystems/vcpkg.cmake`)

2. **Запуск:**
    ```sh
    docker compose up -d
    ./run_tests
    ```
   Пример вывода:
    ```
    [==========] Running X tests from Y test suites.
    ...
    [  PASSED  ] X tests.
    [  FAILED  ] Y tests.
    ```

3. **Фильтрация тестов:**
    ```sh
    ./run_tests --gtest_filter=PostgresRepoTest.UsersRepository_SaveAndLoadAndFind
    ./run_tests --gtest_filter=PostgresRepoTest.*
    ```

4. **Отладка:**  
   Поддерживается в IDE (CLion, VS Code, tagget `run_tests`).

### Важно

- Тесты используют отдельную тестовую базу.
- Перед запуском тестов — база должна быть “чистой” или запускаться в отдельном контейнере.
- Все миграции и создание таблиц — через `IF NOT EXISTS`.

---

## 📚 Архитектурные принципы

- **Repository Pattern**: каждый репозиторий — только для своей сущности, бизнес-логика вне.
- **Thread-safe Connection Pool**: многопоточность из коробки.
- **UUID everywhere**: идентификаторы только через типизированные UUID.
- **Хеширование паролей:** не хранить пароли в открытом виде.

---

## ⚡ Рекомендации

- Для реальных проектов используйте bcrypt/argon2 для паролей.
- Логику аутентификации выносить в отдельный сервис (auth service).
- Код легко масштабируется и thread-safe.

---

## 📎 Связанные файлы

- `server/tests/test_postgres.cpp` — unit-тесты репозиториев и базы
- `tagged.h` — шаблон типобезопасных идентификаторов
- `tagged_uuid.h` — UUID для сущностей
- `postgres.h/.cpp` — репозитории, пул соединений, Database

---
