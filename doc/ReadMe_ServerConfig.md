**1. Drogon-сервер, listeners**

Определяет IP-адреса и порты, на которых сервер принимает входящие HTTP-запросы

```json
"listeners": [
  {
    "address": "0.0.0.0",
    "port": 8080
  }
]
```

`address` - IP адрес (0.0.0.0 - все доступные интерфейсы, 127.0.0.1 - только на локальном интерфейсе)
`port` - TCP-порт для HTTP-трафика (8080 или 80)


**2. Drogon-сервер, логирование приложения (app.log)**

Управляет выводом логов

```json
"app": {
  "log": {
    "log_path": "../logs",
    "logfile_base_name": "server",
    "log_size_limit": 10485760,
    "log_level": "DEBUG"
  }
}
```

`log_path` - папка для файлов логов
`logfile_base_name` - базовое имя лог-файлов
`log_size_limit` - максимальный размер одного файла в байтах (при превышении создаётся новый файл с базовым именем)
`log_level` - уровень логирования: TRACE, DEBUG, INFO, WARN, ERROR

Если параметры файла не указаны - вывод в консоль


**3. Drogon-сервер, plugins**

модули для дополнительной функциональности (DatabasePlugin, ChatServicePlugin, TokenMonitorPlugin, LoggerPlugin)

```json
"plugins": [
  {
    "name": "LoggerPlugin",
    "type": "Global",
    "config": { "log_level": "debug" }
  },
  {
    "name": "DatabasePlugin",
    "config": {
      "conn_str": "",
      "pool_size": 4
    }
  },
  {
    "name": "ChatServicePlugin",
    "type": "Global"
  },
  {
    "name": "TokenMonitorPlugin",
    "config": {
      "cleanup_interval_seconds": 30,
      "token_timeout_minutes": 5
    }
  }
]
```

***Общие поля плагинов Drogon-сервера***

`name` - имя класса плагина, зарегистрированного в коде
`type` - область видимости, например Global
`config` - параметры, передаваемые в initAndStart()

***3.1. LoggerPlugin расширяет логгер Drogon, устанавливает собственный уровень***

`log_level` - уровень сообщений логов

***3.2. DatabasePlugin - пул соединений к БД PostgreSQL***

`conn_str` - строка подключения
`pool_size` - размер пула соединений

***3.3. ChatServicePlugin инициализирует логику чата***

параметры отсутствуют

***3.4. TokenMonitorPlugin для периодической очистки просроченных токенов***

`cleanup_interval_seconds` - интервал проверки (секунды)
`token_timeout_minutes` - время жизни токена (минуты)