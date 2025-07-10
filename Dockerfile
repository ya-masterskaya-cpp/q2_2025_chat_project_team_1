FROM debian:12

# 1. Базовые зависимости
RUN apt-get update && \
    apt-get install -y build-essential cmake git pkg-config curl libssl-dev zip bison flex autoconf automake libtool m4 pkg-config postgresql-client
# 2. vcpkg
WORKDIR /opt
RUN git clone https://github.com/microsoft/vcpkg.git && cd vcpkg && ./bootstrap-vcpkg.sh
ENV VCPKG_ROOT=/opt/vcpkg
ENV PATH="$VCPKG_ROOT:$PATH"

# 3. Установка всех зависимостей через vcpkg
RUN $VCPKG_ROOT/vcpkg install drogon libpqxx openssl jsoncpp cpr gtest ixwebsocket --triplet x64-linux
RUN $VCPKG_ROOT/vcpkg install boost-uuid --triplet x64-linux

# 4. Копируем проект
WORKDIR /app
COPY . .

# 5. Сборка обеих целей с vcpkg toolchain
RUN cmake -Bbuild -S. \
    -DBUILD_CLIENT=OFF \
    -DCMAKE_TOOLCHAIN_FILE=$VCPKG_ROOT/scripts/buildsystems/vcpkg.cmake \
    && cmake --build build --target Server --target DbTests

# 6. Значение по умолчанию: запуск тестов (можно переопределить через docker-compose)
CMD ["./build/server/DbTests"]
