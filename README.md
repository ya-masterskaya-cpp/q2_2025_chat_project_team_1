# IRC chat

Этот проект представляет собой кроссплатформенное IRC-чат-приложение, разработанное в рамках командного проекта. Цель – предоставить удобный интерфейс для общения в реальном времени. Проект демонстрирует навыки работы с сетевыми протоколами, архитектурой клиент-серверных приложений.

---
## 🚀 Возможности

- Подключение к IRC-серверу
- Участие в комнатах
- Отправка и получение сообщений
- Загрузка истории сообщений из БД
- Минимальный, но удобный пользовательский интерфейс
- Логгирование
---
## 📸 Превью

[IRC-chat](./screenshots/IRC-chat.png).

---
## 🛠 Структура репозитория
```
.
├── server/             # Исходники серверной части
├── client/             # Исходники клиентской части
├── domain/             # Исходники общей клиент/серверной части
├── data/               # Файлы конфигурации
├── doc/                # Документация
├── setup/              # Файлы установки
├── CMakeLists.txt      # Файл конфигурации сборки проекта
├── Dockerfile          # docker-файл для развертывания сервера
├── docker-compose.yml  # упрощение запуска нескольких контейнеров
├── README.md           
└── LICENSE             # Лицензия проекта
```
---
## 📦 Установка

### Клиент и сервер из исходников на linux
Клонируем репозиторий vcpkg:
```bash
git clone https://github.com/Microsoft/vcpkg.git ~/.local/share/vcpkg
```

Запускаем bootstrap:
```bash
cd ~/.local/share/vcpkg
./bootstrap-vcpkg.sh
```

Убеждаемся что переменная окружения VCPKG_ROOT установлена корректо:
```bash
echo $VCPKG_ROOT
```

Если не установлена, устанавливаем:
```bash
export VCPKG_ROOT="$HOME/.local/share/vcpkg"
```
(действует только на данную сессию, для постоянной настройки__
добавляем строку в ~/.bashrc или ~/.zshrc).__

Устанавливаем необходимые библиотеки для клиента:
```bash
./vcpkg install jsoncpp cpr ixwebsocket wxWidgets gtest
```

Устанавливаем необходимые библиотеки для сервера:
```bash
./vcpkg install jsoncpp cpr ixwebsocket drogon libpqxx openssl gtest boost-uuid
```

При проблемах установки в файле `~/.local/share/vcpkg/installed/vcpkg/issue_body.md` 
написаны причины, в частности, каких зависимостей нехватает.

Сборка в директории build (для своей системы поменять путь триплета x64-linux на нужное):__
```bash
cmake .. -DCMAKE_PREFIX_PATH=${VCPKG_ROOT}/installed/x64-linux/share/ -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
cmake --build .

```

### Клиент из исходников на Windows
Устанавливаем необходимые библиотеки для клиента:
```bash
.\vcpkg install cpr jsoncpp ixwebsocket wxwidgets gtest --triplet x64-mingw-static
```

В системную переменную PATH нужно добавить путь к MinGW и vcpkg.

В директории /client выполнить:
```bash
mkdir build
cd build
cmake .. -G "MinGW Makefiles" -DCMAKE_TOOLCHAIN_FILE=%VCPKG_ROOT%/scripts/buildsystems/vcpkg.cmake -DVCPKG_TARGET_TRIPLET=x64-mingw-static -DVCPKG_APPLOCAL_DEPS=OFF
cmake --build .
```

### Flatpak
Для создания flatpak-приложения под linux, необходимо использовать flatpak-манифест. Для этого в директории
`setup` необходимо выполнить команду:
```bash
flatpak-builder --install --user --force-clean build-dir team_1.irc_chat.json
```
Команда для запуска приложения:
```bash
flatpak run io.team_1.irc_chat
```
Также после сборки можно упаковать приложение в flatpak-bundle, для переносимости. 
Команды создания bundle, установки, запуска:
```bash
flatpak build-bundle irc_chat.flatpak team_1.irc_chat.json
flatpak install --user irc_chat.flatpak
flatpak run irc_chat.flatpak
```

### Docker контейнер сервера
Инструкция по установке Docker контейнера для сервера,БД по [ссылке](./doc/README_DOCKER.md).

---
## 🎯 Как пользоваться
Инструкция по запуску Docker контейнера для сервера, БД по [ссылке](./doc/README_DOCKER.md).

Клиент запускает в зависимости от выбранного вами способа установки. 
Дополнительных флагов запуска не требует, настройки ip:port в самом приложении:
```bash
./Client
```

---
## 🌐 REST API

Запросы к API описаны по [ссылке](./doc/ReadMe_IRC.md).

---
## 🧪 Тестирование

Для тестирования используется фреймфорк gtest.
Запуск тестов для сервера,БД по [ссылке](./doc/README_DOCKER.md).

При сборке клиента из исходников для тестирования необходимо использовать флаг `Debug`:
```bash
-DCMAKE_BUILD_TYPE=Debug
```
Запуск полученного приложения для тестов:
```bash
./Client_tests ip:port
```
---
## 🔗 Зависимости

1.  [docker](https://www.docker.com/).
2.  [jsoncpp](https://github.com/open-source-parsers/jsoncpp).
3.  [cpr](https://github.com/libcpr/cpr). Версия 1.11.2.
4.  [ixwebsocket](https://github.com/machinezone/IXWebSocket). Версия 11.4.6.
5.  [wxWidgets](https://wxwidgets.org/). Версия 3.2.7.
6.  [drogon](https://drogon.org/). Версия 1.9.10.
7.  [libpqxx](https://pqxx.org/libpqxx/). Версия 7.10.1.
8.  [openssl](https://github.com/openssl/openssl). Версия 3.5.0.
9.  [gtest](https://github.com/google/googletest). Версия 1.17.0.
10. [boost-uuid](https://www.boost.org/doc/libs/1_69_0/libs/uuid/doc/uuid.html). Версия 1.88.0.
---
