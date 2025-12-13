cmake_minimum_required(VERSION 3.10)
project(LANChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# set(SFML_STATIC_LIBRARIES TRUE)

# 尋找 SFML 套件
find_package(SFML 3.0 COMPONENTS Graphics Window System Network REQUIRED)


include_directories(include)

# 遞迴抓取所有來源檔案
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/ui/*.cpp" "src/networking/*.cpp" "main.cpp")

add_executable(LANChess ${SOURCES})

# 連結 SFML
target_link_libraries(LANChess PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Network)


if(WIN32)
    target_link_libraries(LANChess PRIVATE ws2_32)
endif()
if(MSVC)
    add_compile_options(/utf-8)
endif()

if(WIN32)
    add_custom_command(TARGET LANChess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:LANChess>
        $<TARGET_FILE_DIR:LANChess>
        COMMAND_EXPAND_LISTS
    )

endif()
