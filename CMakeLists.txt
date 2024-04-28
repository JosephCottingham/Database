cmake_minimum_required(VERSION 3.0.0)
project(BasicDatabase VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

add_executable(BasicDatabase src/BasicDatabase.cpp)

add_subdirectory(src/StorageEngine)
target_include_directories(${PROJECT_NAME} PUBLIC src/StorageEngine)
target_link_libraries(${PROJECT_NAME}  StorageEngine)
target_link_directories(${PROJECT_NAME} PUBLIC src/StorageEngine)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
