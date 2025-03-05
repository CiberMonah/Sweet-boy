cmake_minimum_required(VERSION 3.10)

project(GeometryProject VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include_directories(include)

add_library(Tree_Lib
    source/RBTree.cpp
)

target_include_directories(Tree_Lib PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Исполняемый файл
add_executable(RBTree_example source/main.cpp)

target_link_libraries(RBTree_example
    Tree_Lib
)

# Подключение тестов
add_subdirectory(tests)

enable_testing()