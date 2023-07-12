cmake_minimum_required(VERSION 3.12)
project(cryptotools)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON CACHE INTERNAL "")

##########################################################
# Prepare hello_imgui during configure time
##########################################################
include(FetchContent)
FetchContent_Declare(
    hello_imgui
    GIT_REPOSITORY https://github.com/pthom/hello_imgui.git
    # Enter the desired git tag below
    # GIT_TAG
)
FetchContent_MakeAvailable(hello_imgui)
# Make cmake function `hello_imgui_add_app` available
list(APPEND CMAKE_MODULE_PATH ${HELLOIMGUI_CMAKE_PATH})
include(hello_imgui_add_app)

# TODO: figure out why this isn't building
# set(CRYPTOPP_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/cryptopp")
add_subdirectory(cryptopp-cmake)

##########################################################
# Build your app
##########################################################
FILE(GLOB SOURCES_CRYPTOTOOLS 
    ${CMAKE_CURRENT_LIST_DIR}/*.h
    ${CMAKE_CURRENT_LIST_DIR}/*.cpp
    )

hello_imgui_add_app(cryptotools ${SOURCES_CRYPTOTOOLS})
target_link_libraries(cryptotools PRIVATE cryptopp)
target_include_directories(cryptotools PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
