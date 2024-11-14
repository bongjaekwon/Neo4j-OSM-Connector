cmake_minimum_required(VERSION 3.10)

project(Neo4jCppConnector VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)

# Qt5 또는 Qt6 찾기
find_package(Qt5 REQUIRED Widgets)  # Qt5의 경우
# find_package(Qt6 REQUIRED Widgets)  # Qt6의 경우

# CURL 라이브러리 찾기
find_package(CURL REQUIRED)

# AUTOMOC 활성화 (MOC 파일 자동 생성)
set(CMAKE_AUTOMOC ON)

add_executable(Neo4jCppProject main.cpp)

# Qt 라이브러리와 CURL 라이브러리 링크
target_link_libraries(Neo4jCppProject PRIVATE Qt5::Widgets ${CURL_LIBRARIES})  # Qt5
# target_link_libraries(Neo4jCppProject PRIVATE Qt6::Widgets ${CURL_LIBRARIES})  # Qt6

target_include_directories(Neo4jCppProject PRIVATE ${CURL_INCLUDE_DIRS})

