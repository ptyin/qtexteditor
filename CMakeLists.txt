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
        src/exam.ui
        src/exam.cpp
        src/exam.h
        src/login.ui
        src/login.cpp
        src/login.h
        src/main.cpp
        src/passwdedit.ui
        src/passwdedit.cpp
        src/passwdedit.h
        src/register.ui
        src/register.cpp
        src/register.h
        src/editor.ui
        src/editor.cpp
        src/editor.h
        src/systemtrayicon.cpp
        src/systemtrayicon.h
        src/mtextedit.cpp
        src/mtextedit.h
        src/HTMLParser.cpp
        src/HTMLParser.h)
qt5_add_resources(SOURCELIST resources/lang.qrc resources/myqrc.qrc resources/qss.qrc)
# Tell CMake to create the QTLaTeX executable
add_executable(QtLaTeX ${SOURCELIST})
add_executable(test test/main.cpp test/HTMLParser.cpp test/HTMLParser.h)
# Use the Widgets module from Qt 5
target_link_libraries(QtLaTeX Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql)
target_link_libraries(test Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Sql)