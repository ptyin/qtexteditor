cmake_minimum_required(VERSION 3.13)
project(QtLaTeX)

set(CMAKE_CXX_STANDARD 14)

set(CMAKE_PREFIX_PATH /home/peter/Qt5.12.1/5.12.1/gcc_64/)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)

# Find the QtWidgets library
find_package(Qt5 COMPONENTS Core Widgets Gui Sql REQUIRED)

# Populate a CMake variable with the sources
set(SOURCELIST
        exam.ui
        exam.cpp
        exam.h
        login.ui
        login.cpp
        login.h
        main.cpp
        passwdedit.ui
        passwdedit.cpp
        passwdedit.h
        register.ui
        register.cpp
        register.h
        editor.ui
        editor.cpp
        editor.h
        systemtrayicon.cpp
        systemtrayicon.h
        )
qt5_add_resources(SOURCELIST lang.qrc myqrc.qrc qss.qrc)
# Tell CMake to create the QTLaTeX executable
add_executable(QtLaTeX ${SOURCELIST})
# Use the Widgets module from Qt 5
target_link_libraries(QtLaTeX Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql)