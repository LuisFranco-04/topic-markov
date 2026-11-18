cmake_minimum_required(VERSION 3.16)
project(TopicMarkov LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

option(TM_ASAN "Compilar con AddressSanitizer y UBSan" OFF)

find_package(Threads REQUIRED)

set(TM_WARNINGS -Wall -Wextra -Wpedantic)

add_library(topicmarkov_core
  src/Vocabulario.cpp
  src/Tokenizador.cpp
  src/LectorDatos.cpp
  src/Distribucion.cpp
  src/ModeloMarkov.cpp
  src/ModeloMezcla.cpp
  src/Generador.cpp
  src/Metrica.cpp
)
target_include_directories(topicmarkov_core PUBLIC include)
target_compile_options(topicmarkov_core PRIVATE ${TM_WARNINGS})
target_link_libraries(topicmarkov_core PUBLIC Threads::Threads)

add_executable(topicmarkov src/main.cpp)
target_compile_options(topicmarkov PRIVATE ${TM_WARNINGS})
target_link_libraries(topicmarkov PRIVATE topicmarkov_core)

enable_testing()
add_executable(tests tests/test_main.cpp)
target_compile_options(tests PRIVATE ${TM_WARNINGS})
target_link_libraries(tests PRIVATE topicmarkov_core)
add_test(NAME unit COMMAND tests)

if(TM_ASAN)
  foreach(t topicmarkov_core topicmarkov tests)
    target_compile_options(${t} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer -g)
    target_link_options(${t} PRIVATE -fsanitize=address,undefined)
  endforeach()
endif()
