cmake_minimum_required(VERSION 3.10)
project(OOPDataStructures VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Скачивание Google Test
include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

# Интерфейсная библиотека для шаблонных классов (реализация в заголовках)
add_library(data_structures INTERFACE)
target_include_directories(data_structures INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

# Тесты с Google Test
add_executable(tests_gtest tests_oop_gtest.cpp)
target_link_libraries(tests_gtest PRIVATE GTest::gtest_main data_structures)

# Оригинальные тесты (если нужны)
add_executable(tests_original tests_oop.cpp)
target_link_libraries(tests_original PRIVATE data_structures)

# Бенчмарки
add_executable(benchmark benchmark.cpp)
target_link_libraries(benchmark PRIVATE data_structures)

# Опция для включения покрытия кода
option(ENABLE_COVERAGE "Enable code coverage reporting" OFF)
if(ENABLE_COVERAGE)
  add_compile_options(--coverage)
  add_link_options(--coverage)
endif()

# Информация о целях
message(STATUS "Targets created:")
message(STATUS "  - tests_gtest (Google Test)")
message(STATUS "  - tests_original (original tests)")
message(STATUS "  - benchmark (performance tests)")
