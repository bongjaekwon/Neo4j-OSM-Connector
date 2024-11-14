cmake_minimum_required(VERSION 3.10)


project(Neo4jCppConnector VERSION 1.0)


set(CMAKE_CXX_STANDARD 17)


find_package(CURL REQUIRED)


add_executable(Neo4jCppProject main.cpp)


target_link_libraries(Neo4jCppProject PRIVATE ${CURL_LIBRARIES})


target_include_directories(Neo4jCppProject PRIVATE ${CURL_INCLUDE_DIRS})
