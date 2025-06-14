# IRCDBWrapper — упрощённый интерфейс для работы с БД IRC-чата

## 📝 Описание

`IRCDBWrapper` — это высокоуровневая обёртка для работы с базой данных IRC-чата, позволяющая выполнять базовые операции с БД *без прямого доступа к репозиториям и транзакциям*.  
Обеспечивает быстрый старт и простое подключение к базе для взаимодействия.
---

## 🚀 Быстрый старт

```cpp
#include "db_wrapper.h"

IRCDBWrapper db("localhost", 5432, "test_db", "test_user", "test_password");

// Добавление пользователя
auto [ok, err] = db.AddUserToDB("alice", "hashed_password");
if (!ok) std::cout << "Ошибка: " << err << std::endl;

// Получение всех пользователей (имя -> хеш)
auto users = db.GetAllUsers();
for (const auto& [name, hash] : users) {
    std::cout << name << ": " << hash << std::endl;
}
```

---

## 🛠️ Методы класса

### `IRCDBWrapper::IRCDBWrapper(...)`
**Конструктор**  
Параметры:
- `host` — адрес сервера PostgreSQL
- `port` — порт (int)
- `dbname` — имя базы
- `user` — пользователь базы
- `password` — пароль
- `pool_size` — (опционально) количество соединений в пуле, по умолчанию 1

### `std::pair<bool, std::string> AddUserToDB(const std::string& name, const std::string& pass_hash)`
Добавляет пользователя.  
- Возвращает `{true, ""}` если успешно.
- `{false, "User already exists"}` — если пользователь уже был.
- `{false, <описание ошибки>}` — если возникло исключение.

### `std::unordered_map<std::string, std::string> GetAllUsers()`
Возвращает map: *имя пользователя* → *хеш пароля*.  
При ошибке — пустая map.

---

## 🔒 Важно

- Пароль передавать **в виде хеша** (например, SHA256), а не открытым текстом!
- Не хранит соединения открытыми между вызовами.
- Автоматически создаёт таблицы при первом вызове (через Database).

---

## 💡 Пример: регистрация и выгрузка пользователей

```cpp
IRCDBWrapper db("localhost", 5432, "test_db", "test_user", "test_password");

// Регистрация пользователя
auto [ok, err] = db.AddUserToDB("bob", "hash_bob");
if (!ok) std::cerr << "Ошибка: " << err << std::endl;

// Получение всех пользователей
auto users = db.GetAllUsers();
for (const auto& [login, hash] : users) {
    std::cout << login << " : " << hash << std::endl;
}
```

---

## 🧪 Тестирование

- Для автоматизированных тестов можно использовать готовые тесты из `test_postgres.cpp`, включая:
    - Проверку добавления новых и существующих пользователей через обёртку.
    - Проверку выгрузки всех пользователей.

---

## 📂 Связанные файлы

- `db_wrapper.h/.cpp` — код обёртки
- `postgres.h/.cpp` — низкоуровневые репозитории и инфраструктура
- [`README_irc_chat_db.md`](./README_irc_chat_db.md) — документация по остальному слою работы с БД

