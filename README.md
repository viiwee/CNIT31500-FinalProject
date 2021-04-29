# CNIT31500-FinalProject
# To run these files:
#   Install cygwin: https://cygwin.com/setup-x86_64.exe
#       Install the following packages:
#           gcc-g++
#           make
#           gdb
#           curl
#   If "curl-7.76.1-win64-mingw" is not in the git, download from: https://curl.se/windows/dl-7.76.1/curl-7.76.1-win64-mingw.zip
#       Ensure that libcurl-x64.dll is in the same location as the built .exe
#   Configure Environment to use curl
#       CMakeLists:
#           set(libcurl_INCLUDE_DIR "C:/Users/Matthew/CLionProjects/CNIT31500-FinalProject/curl-7.76.1-win64-mingw/include")
#           include_directories(${libcurl_INCLUDE_DIR})
#
#           add_executable(CNIT31500-FinalProject main.c)
#
#           target_link_libraries(CNIT31500-FinalProject C:/Users/Matthew/CLionProjects/CNIT31500-FinalProject/curl-7.76.1-win64-mingw/lib/libcurl.dll.a)