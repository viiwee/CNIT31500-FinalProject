cmake_minimum_required(VERSION 3.10)
project(CNIT31500-FinalProject)

set(CMAKE_C_STANDARD 99)
set(libcurl_INCLUDE_DIR "C:/Users/Matthew/CLionProjects/CNIT31500-FinalProject/curl-7.76.1-win64-mingw/include")
include_directories(${libcurl_INCLUDE_DIR})

add_executable(CNIT31500-FinalProject main.c)
#add_executable(CNIT31500-FinalProject curl_test.c)

target_link_libraries(CNIT31500-FinalProject C:/Users/Matthew/CLionProjects/CNIT31500-FinalProject/curl-7.76.1-win64-mingw/lib/libcurl.dll.a)
