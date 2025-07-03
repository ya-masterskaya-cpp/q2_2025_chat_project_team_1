/*
 @licstart  The following is the entire license notice for the JavaScript code in this file.

 The MIT License (MIT)

 Copyright (C) 1997-2020 by Dimitri van Heesch

 Permission is hereby granted, free of charge, to any person obtaining a copy of this software
 and associated documentation files (the "Software"), to deal in the Software without restriction,
 including without limitation the rights to use, copy, modify, merge, publish, distribute,
 sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all copies or
 substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
 BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 @licend  The above is the entire license notice for the JavaScript code in this file
*/
var NAVTREE =
[
  [ "IRC-Chat", "index.html", [
    [ "Diagrams", "_u_m_l-_diagram_of_classes.html", null ],
    [ "IRCDBWrapper — упрощённый интерфейс для работы с БД IRC-чата", "md__r_e_a_d_m_e__db__wrapper.html", [
      [ "📝 Описание", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md1", null ],
      [ "🚀 Быстрый старт", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md3", null ],
      [ "🛠️ Методы класса", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md5", [
        [ "Пользователи", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md6", null ],
        [ "Комнаты", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md8", null ],
        [ "Участие в комнатах", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md10", null ],
        [ "Сообщения", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md12", null ]
      ] ],
      [ "💡 Примеры использования", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md14", null ],
      [ "📝 Примечания", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md16", null ],
      [ "🧪 Тестирование", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md18", null ],
      [ "📂 Связанные файлы", "md__r_e_a_d_m_e__db__wrapper.html#autotoc_md20", null ]
    ] ],
    [ "IRC Chat Project — Docker Guide", "md__r_e_a_d_m_e___d_o_c_k_e_r.html", [
      [ "🐳 Быстрый старт с Docker Compose", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md23", [
        [ "Клонируйте репозиторий и перейдите в каталог проекта", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md24", null ],
        [ "Создайте файл .env с переменными для БД", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md25", null ],
        [ "Проверьте структуру проекта", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md26", null ],
        [ "Постройте и запустите все контейнеры", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md27", null ],
        [ "Посмотреть логи", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md28", null ],
        [ "Запустить только сервер и БД", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md29", null ],
        [ "Запустить только тесты", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md30", null ],
        [ "Остановить и удалить контейнеры", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md31", null ],
        [ "Войти в контейнер для ручной диагностики", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md32", null ],
        [ "Изменили код? Пересоберите!", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md33", null ]
      ] ],
      [ "🧩 Полезные советы", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md35", null ],
      [ "🚩 Частые ошибки и решения", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md37", null ],
      [ "💡 Рекомендации для CI/CD", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md39", null ],
      [ "🛠 Полезные команды для отладки", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md41", null ],
      [ "📚 Ссылки", "md__r_e_a_d_m_e___d_o_c_k_e_r.html#autotoc_md43", null ]
    ] ],
    [ "IRC-Chat", "md__read_me___i_r_c.html", [
      [ "Запросы к API", "md__read_me___i_r_c.html#autotoc_md46", [
        [ "REST API для аутентификации", "md__read_me___i_r_c.html#autotoc_md48", [
          [ "Ping сервера", "md__read_me___i_r_c.html#autotoc_md47", null ],
          [ "Регистрация", "md__read_me___i_r_c.html#autotoc_md49", null ],
          [ "Вход (выдает новый токен)", "md__read_me___i_r_c.html#autotoc_md50", null ],
          [ "Выход (токен аннулируется)", "md__read_me___i_r_c.html#autotoc_md51", null ]
        ] ],
        [ "REST API для работы с пользователями", "md__read_me___i_r_c.html#autotoc_md53", [
          [ "Получить список всех подключенных клиентов WebSocket", "md__read_me___i_r_c.html#autotoc_md54", null ]
        ] ],
        [ "REST API для работы с комнатами", "md__read_me___i_r_c.html#autotoc_md56", [
          [ "Создать новую комнату", "md__read_me___i_r_c.html#autotoc_md57", null ],
          [ "Присоединиться к комнате чата", "md__read_me___i_r_c.html#autotoc_md58", null ],
          [ "Выйти в общую комнату (перейти в комнату GENERAL_ROOM)", "md__read_me___i_r_c.html#autotoc_md59", null ],
          [ "Получить название текущей комнаты", "md__read_me___i_r_c.html#autotoc_md60", null ],
          [ "Получить список всех комнат", "md__read_me___i_r_c.html#autotoc_md61", null ],
          [ "Получить список участников комнаты", "md__read_me___i_r_c.html#autotoc_md62", null ]
        ] ],
        [ "REST API для работы с сообщениями", "md__read_me___i_r_c.html#autotoc_md64", [
          [ "Отправка сообщения (REST)", "md__read_me___i_r_c.html#autotoc_md65", null ],
          [ "Сохранение сообщения в БД (выполнять запрос после отправки сообщений на сервер!)", "md__read_me___i_r_c.html#autotoc_md66", null ],
          [ "Получение последних N сообщений в комнате", "md__read_me___i_r_c.html#autotoc_md67", null ]
        ] ],
        [ "Общие ошибки для всех запросов с авторизацией по токену", "md__read_me___i_r_c.html#autotoc_md69", null ]
      ] ],
      [ "Зависимости и сборка проекта", "md__read_me___i_r_c.html#autotoc_md71", null ],
      [ "Установка drogon и других библиотек", "md__read_me___i_r_c.html#autotoc_md72", null ],
      [ "Сборка", "md__read_me___i_r_c.html#autotoc_md73", null ],
      [ "Отладочная сборка, TSAN", "md__read_me___i_r_c.html#autotoc_md74", [
        [ "Проверка", "md__read_me___i_r_c.html#autotoc_md75", null ]
      ] ],
      [ "Скрипт для создания таблиц (локальная сборка, без Docker)", "md__read_me___i_r_c.html#autotoc_md76", null ],
      [ "Для удаления таблиц (локальная сборка, без Docker)", "md__read_me___i_r_c.html#autotoc_md77", null ],
      [ "Информация по таблицам (для отладки и мониторинга)", "md__read_me___i_r_c.html#autotoc_md78", [
        [ "Подключение к базе данных:", "md__read_me___i_r_c.html#autotoc_md79", null ],
        [ "Список пользователей с их хешами и длиной имени", "md__read_me___i_r_c.html#autotoc_md80", null ],
        [ "Список всех комнат", "md__read_me___i_r_c.html#autotoc_md81", null ],
        [ "Список пользователей и их комнат", "md__read_me___i_r_c.html#autotoc_md82", null ],
        [ "Выход из psql:", "md__read_me___i_r_c.html#autotoc_md83", null ]
      ] ]
    ] ],
    [ "IRC Chat Database Layer (C++ / PostgreSQL)", "md__r_e_a_d_m_e__irc__chat__db.html", [
      [ "📦 Описание", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md85", null ],
      [ "🗃️ Структуры и классы", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md87", [
        [ "util::Tagged, util::TaggedUUID", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md88", null ],
        [ "postgres::ConnectionPool", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md90", null ],
        [ "UserRecord, RoomRecord, MessageRecord, RoomMemberRecord", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md92", null ],
        [ "Репозитории", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md94", [
          [ "UsersRepository", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md95", null ],
          [ "RoomsRepository", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md96", null ],
          [ "MessagesRepository", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md97", null ],
          [ "RoomMembersRepository", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md98", null ]
        ] ],
        [ "postgres::Database", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md100", null ],
        [ "IRCDBWrapper (высокоуровневая обёртка для работы с БД)", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md102", [
          [ "Работа с пользователями", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md103", null ],
          [ "Работа с комнатами", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md104", null ],
          [ "Участие в комнатах", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md105", null ],
          [ "Работа с сообщениями", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md106", null ]
        ] ]
      ] ],
      [ "🔄 Пагинация (постраничная выборка)", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md108", null ],
      [ "🛠️ Примеры использования", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md110", null ],
      [ "🏗️ Структура таблиц", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md112", null ],
      [ "🧪 Тестирование", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md114", null ],
      [ "📚 Архитектурные принципы", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md116", null ],
      [ "⚡ Рекомендации", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md118", null ],
      [ "📎 Связанные файлы", "md__r_e_a_d_m_e__irc__chat__db.html#autotoc_md120", null ]
    ] ],
    [ "Namespaces", "namespaces.html", [
      [ "Namespace List", "namespaces.html", "namespaces_dup" ],
      [ "Namespace Members", "namespacemembers.html", [
        [ "All", "namespacemembers.html", null ],
        [ "Functions", "namespacemembers_func.html", null ],
        [ "Typedefs", "namespacemembers_type.html", null ]
      ] ]
    ] ],
    [ "Classes", "annotated.html", [
      [ "Class List", "annotated.html", "annotated_dup" ],
      [ "Class Index", "classes.html", null ],
      [ "Class Hierarchy", "hierarchy.html", "hierarchy" ],
      [ "Class Members", "functions.html", [
        [ "All", "functions.html", null ],
        [ "Functions", "functions_func.html", null ],
        [ "Variables", "functions_vars.html", null ],
        [ "Typedefs", "functions_type.html", null ]
      ] ]
    ] ],
    [ "Files", "files.html", [
      [ "File List", "files.html", "files_dup" ]
    ] ]
  ] ]
];

var NAVTREEINDEX =
[
"_u_m_l-_diagram_of_classes.html",
"functions_vars.html"
];

var SYNCONMSG = 'click to disable panel synchronization';
var SYNCOFFMSG = 'click to enable panel synchronization';