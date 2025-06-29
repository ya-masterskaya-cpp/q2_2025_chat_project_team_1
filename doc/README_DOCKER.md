# IRC Chat Project — Docker Guide

## 🐳 Быстрый старт с Docker Compose

### 1. Клонируйте репозиторий и перейдите в каталог проекта
```sh
git clone <your_repo_url>
cd <your_project_folder>
```

### 2. Создайте файл `.env` с переменными для БД  
Пример содержимого `.env` (замените на свои значения!):
```env
POSTGRES_USER=chat_user
POSTGRES_PASSWORD=Очень_сложный_пароль
POSTGRES_DB=irc_chat

IRC_CHAT_DB_URL=host=postgres dbname=irc_chat user=chat_user password=Очень_сложный_пароль
IRC_CHAT_TEST_DB_URL=host=postgres dbname=irc_chat_test user=chat_user password=Очень_сложный_пароль
```
> **Важно:** файл `.env` должен лежать в корне проекта и не попадать в git (добавьте `.env` в `.gitignore`).

### 3. Проверьте структуру проекта  
В каталоге должны быть:
- `docker-compose.yml`
- `Dockerfile`
- `.env`
- исходники (сервер, тесты, etc.)
- (опционально) `wait-for-it.sh` или аналог для корректного старта сервисов

### 4. Постройте и запустите все контейнеры
```sh
docker compose up --build
```
Первая сборка займет больше времени, далее будет быстрее за счет кэша.  
Контейнеры базы данных, сервера и тестов соберутся и стартуют.  
По завершении тестов контейнер `dbtests` остановится.

### 5. Посмотреть логи
```sh
docker compose logs server
docker compose logs dbtests
docker compose logs postgres
```
Для получения свежих логов в реальном времени:
```sh
docker compose logs -f server
```

### 6. Запустить только сервер и БД
```sh
docker compose up --build server postgres
```

### 7. Запустить только тесты
```sh
docker compose up --build dbtests
```
Логи и результаты тестов смотрите через:
```sh
docker compose logs dbtests
```

### 8. Остановить и удалить контейнеры
```sh
docker compose down
```
Удалить все данные (пересоздать volume и БД заново):
```sh
docker compose down -v
```

### 9. Войти в контейнер для ручной диагностики
```sh
docker compose run --rm server /bin/bash
```
или
```sh
docker compose exec postgres psql -U chat_user -d irc_chat
```

### 10. Изменили код? Пересоберите!
```sh
docker compose up --build
```

---

## 🧩 Полезные советы

- Для корректного создания тестовой базы используйте в команде тестового контейнера:
    ```yaml
    command: ["/bin/sh", "-c", "export PGPASSWORD=$POSTGRES_PASSWORD && sleep 10 && psql -U $POSTGRES_USER -h postgres -d postgres -c 'CREATE DATABASE irc_chat_test;' || true && ./build/DbTests"]
    ```
- Убедитесь, что в контейнере с тестами установлен `postgresql-client` (в Dockerfile):
    ```dockerfile
    RUN apt-get update && apt-get install -y postgresql-client
    ```
- Если используете скрипты ожидания (`wait-for-it.sh`), добавьте их в образ и в команду запуска.
- Переменные окружения в `.env` должны совпадать для всех сервисов.

---

## 🚩 Частые ошибки и решения

- **База данных не создана:**  
  Проверьте правильность команды создания через `psql` и что все переменные прокинуты.

- **"connection refused":**  
  Значит, сервер стартует раньше БД. Используйте задержку или `wait-for-it`.

- **Config file not found:**  
  Проверьте, что файл реально копируется (или пробрасывается volume) в Dockerfile и его путь соответствует `WORKDIR`.

- **psql: not found:**  
  Установите пакет `postgresql-client` в Dockerfile для тестового контейнера.

---

## 💡 Рекомендации для CI/CD

- Используйте `.env.example` для шаблона переменных.
- Храните секреты отдельно и не коммитьте рабочие пароли.
- Для деплоя используйте volume и миграции через init-скрипты или отдельные контейнеры.

---

## 🛠 Полезные команды для отладки

- Проверить переменные окружения:
    ```sh
    docker compose run dbtests env | grep IRC_CHAT
    ```

- Проверить наличие файла:
    ```sh
    docker compose run server ls -l /app/data/config.json
    ```

- Создать тестовую БД вручную (например, для отладки):
    ```sh
    docker compose exec postgres psql -U chat_user -c 'CREATE DATABASE irc_chat_test;'
    ```

> Если что-то не работает — проверяйте логи сервисов и соответствие всех переменных окружения!

---

## 📚 Ссылки

- [Документация Docker Compose](https://docs.docker.com/compose/)
- [Документация PostgreSQL Docker](https://hub.docker.com/_/postgres)
- [wait-for-it](https://github.com/vishnubob/wait-for-it)

