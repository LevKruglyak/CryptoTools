cmake_minimum_required(VERSION 3.21)
project(cryptotools)
# set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O0")
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

include(FetchContent)

FetchContent_Declare(hello_imgui GIT_REPOSITORY https://github.com/pthom/hello_imgui.git)
FetchContent_MakeAvailable(hello_imgui)
list(APPEND CMAKE_MODULE_PATH ${HELLOIMGUI_CMAKE_PATH})
include(hello_imgui_add_app)

FetchContent_Declare(crypto_cmake GIT_REPOSITORY https://github.com/abdes/cryptopp-cmake.git)
FetchContent_MakeAvailable(crypto_cmake)

FetchContent_Declare(fmt GIT_REPOSITORY https://github.com/fmtlib/fmt.git)
FetchContent_MakeAvailable(fmt)

FILE(GLOB SOURCES_CRYPTOTOOLS 
  ${CMAKE_CURRENT_LIST_DIR}/core/*.cpp
  ${CMAKE_CURRENT_LIST_DIR}/windows/*.cpp
  ${CMAKE_CURRENT_LIST_DIR}/lib/imnodes/*.cpp)

hello_imgui_add_app(cryptotools ${SOURCES_CRYPTOTOOLS})
target_link_libraries(cryptotools PRIVATE fmt)
target_link_libraries(cryptotools PRIVATE cryptopp)

target_include_directories(cryptotools PRIVATE 
  "${CMAKE_CURRENT_SOURCE_DIR}" 
  "${CMAKE_CURRENT_SOURCE_DIR}/core/include" 
  "${CMAKE_CURRENT_SOURCE_DIR}/lib/imnodes")

target_compile_options(cryptotools PRIVATE -fsanitize=address)
target_link_options(cryptotools PRIVATE -fsanitize=address)
