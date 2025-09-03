# Spécifie la version minimale de CMake requise
cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Nom du projet
project(retropocket-back)

find_package(Crow)
set(CROW_FEATURES "ssl;compression")
find_package(OpenSSL REQUIRED)

include_directories("${PROJECT_SOURCE_DIR}/lib")
include_directories("${PROJECT_SOURCE_DIR}/include")

# Ajoute l'exécutable
add_executable(retropocket-back main.cpp)
target_link_libraries(retropocket-back PUBLIC Crow::Crow
  OpenSSL::SSL
  OpenSSL::Crypto
  pthread
  dl)
