cmake_minimum_required(VERSION 3.10)
project(LANChess)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 如果你使用的是動態連結 (Dynamic Linking)，SFML 3 預設就是動態
# 如果你想用靜態連結，需要解開下面這行的註解，並確保你下載了靜態庫
# set(SFML_STATIC_LIBRARIES TRUE)

# 尋找 SFML 套件
find_package(SFML 3.0 COMPONENTS Graphics Window System Network REQUIRED)

# 設定 Include 路徑 (如果有自己的 header)
include_directories(include)

# 遞迴抓取所有來源檔案
file(GLOB_RECURSE SOURCES "src/*.cpp" "src/ui/*.cpp" "src/networking/*.cpp" "main.cpp")

add_executable(LANChess ${SOURCES})

# 連結 SFML
target_link_libraries(LANChess PRIVATE SFML::Graphics SFML::Window SFML::System SFML::Network)

# Windows Socket 連結
if(WIN32)
    target_link_libraries(LANChess PRIVATE ws2_32)
endif()
if(MSVC)
    add_compile_options(/utf-8)
endif()
# *** 新增：自動複製 DLL 到 build 資料夾 (Windows 必備) ***
# 這可以避免編譯成功後，執行時跳出 "找不到 xxx.dll" 的錯誤
if(WIN32)
    add_custom_command(TARGET LANChess POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:LANChess>
        $<TARGET_FILE_DIR:LANChess>
        COMMAND_EXPAND_LISTS
    )
endif()