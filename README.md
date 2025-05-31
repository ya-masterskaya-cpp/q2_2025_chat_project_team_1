## Правила
* далее правила установки, прочтите их, запустите сборку для минимальных примеров,__
будет 2а исполняемых файла, проверьте их работу.
* работаем каждый в своей ветке (пример ветка server для сервера и тд.).
* ветка develop для слияния вашего готового кода, покрытого тестами.
* директория doc для ваших документов (uml в формате pdf).
* директория domain для кода доменной области, общего кода для сервера и клиента.
* ветку main впринципе не трогаем, там будет релиз.
* по директориям - это не жесткие требования, расширяемся как необходимо(это приветствуется).


## Устрановка

Клонируем репозиторий vcpkg:  
```bash
git clone https://github.com/Microsoft/vcpkg.git ~/.local/share/vcpkg
```

Запускаем bootstrap (для Windows - .\bootstrap-vcpkg.bat):  
```bash
cd ~/.local/share/vcpkg
./bootstrap-vcpkg.sh
```

Убеждаемся VCPKG_ROOT установлен корректо:__
```bash
echo $VCPKG_ROOT
```

Если не установлен, устанавливаем:__
```bash
export VCPKG_ROOT="$HOME/.local/share/vcpkg"
```
(действует только на данную сессию, для постоянной настройки__
добавлем строку в ~/.bashrc или ~/.zshrc)__

Устанавливаем libpqxx, Boost и wxWidgets:__
```bash
vcpkg install libpqxx boost wxwidgets
```

Для специфичных версий (если нужно):__
```bash
vcpkg install libpqxx@7.7.4 boost@1.83.0 wxwidgets@3.2.2.1
```

При проблемах установки смотрим файл `~/.local/share/vcpkg/installed/vcpkg/issue_body.md` 
там все расписано, в частности, каких зависимостей нехватает.__

Сборка в директории build (для своей системы поменять путь триплета x64-linux на нужное):__
```bash
cmake .. -DCMAKE_PREFIX_PATH=${VCPKG_ROOT}/installed/x64-linux/share/
cmake --build .

```
