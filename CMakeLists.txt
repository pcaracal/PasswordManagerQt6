cmake_minimum_required(VERSION 3.27)
project(PasswordManagerQt6)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

find_package(OpenSSL REQUIRED)

find_package(nlohmann_json 3.2.0 REQUIRED)

include_directories(/usr/include/cryptopp)

add_executable(PasswordManagerQt6 src/main.cpp
        src/AppMainWindow.cpp
        src/AppMainWindow.hpp
        src/LoginWidget.cpp
        src/LoginWidget.hpp
        src/StringUtil.cpp
        src/StringUtil.hpp
        src/FileUtil.cpp
        src/FileUtil.hpp
        src/PasswordWidget.cpp
        src/PasswordWidget.hpp)
target_link_libraries(PasswordManagerQt6
        Qt::Core
        Qt::Gui
        Qt::Widgets
        OpenSSL::SSL
        OpenSSL::Crypto
        nlohmann_json::nlohmann_json
        cryptopp
)

