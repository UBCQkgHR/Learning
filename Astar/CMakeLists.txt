cmake_minimum_required(VERSION 3.10)
project(Astar)

# Устанавливаем стандарт C++
#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Подключаем заголовки из include/
include_directories(include)

# Автоматически ищем исходники и заголовки (рекурсивно)
file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS *.cpp)
file(GLOB_RECURSE HEADERS CONFIGURE_DEPENDS *.h)

# Создаём исполняемый файл
add_executable(Astar ${SOURCES} ${HEADERS})

# Ищем и подключаем SFML
find_package(SFML 2.5 REQUIRED COMPONENTS graphics window system)
target_link_libraries(Astar sfml-graphics sfml-window sfml-system)

# Дополнительные флаги компиляции (по желанию)
target_compile_options(Astar PRIVATE -Wall -Wextra -Wpedantic)

