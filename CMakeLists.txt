cmake_minimum_required(VERSION 3.15.3)
project(2700)

include_directories(/usr/local/opt/curl/include/)

set(CMAKE_CXX_STANDARD 17)

set(CURL_LIBRARY "-lcurl")
find_package(CURL REQUIRED)

add_executable(2700 main.cpp )

include_directories(${CURL_INCLUDE_DIR})
target_link_libraries(2700 ${CURL_LIBRARIES})