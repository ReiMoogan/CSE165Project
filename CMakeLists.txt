cmake_minimum_required(VERSION 3.16)
project(CSE165Project)

# C++20 standard bc i like my lambdas
set(CMAKE_CXX_STANDARD 20)

# Use Qt6 compilers for resources and UI files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# My Windows Install
set(CMAKE_PREFIX_PATH "C:\\Qt\\6.5.0\\msvc2019_64")
find_package (Qt6 PATHS "C:\\Qt\\6.5.0\\msvc2019_64" NO_DEFAULT_PATH)

# My MacOS Install
set(CMAKE_PREFIX_PATH "/Users/doggysazhi/Qt/6.5.0/macos")
find_package (Qt6 PATHS "/Users/doggysazhi/Qt/6.5.0/macos" NO_DEFAULT_PATH)

# Defaulted
find_package(Qt6 COMPONENTS
        Core
        Gui
        OpenGL
        OpenGLWidgets
        Widgets
        Multimedia
        REQUIRED)

set(HEADERS include/Window.h include/GLWidget.h include/entities/Entity.h include/entities/Derek.h include/entities/ImageEntity.h)
set(SOURCES src/main.cpp src/Window.cpp src/GLWidget.cpp src/entities/Derek.cpp src/entities/ImageEntity.cpp)
qt6_add_resources(SOURCES assets/resources.qrc)
add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SOURCES})
# idk what this is here for
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/)

# Set Qt features here
target_link_libraries(${CMAKE_PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::OpenGL
        Qt::OpenGLWidgets
        Qt::Widgets
        Qt::Multimedia
        )

if (WIN32 AND NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    set(DEBUG_SUFFIX)
    if (MSVC AND CMAKE_BUILD_TYPE MATCHES "Debug")
        set(DEBUG_SUFFIX "d")
    endif ()
    set(QT_INSTALL_PATH "${CMAKE_PREFIX_PATH}")
    if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
        set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        if (NOT EXISTS "${QT_INSTALL_PATH}/bin")
            set(QT_INSTALL_PATH "${QT_INSTALL_PATH}/..")
        endif ()
    endif ()
    if (EXISTS "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E make_directory
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/plugins/platforms/qwindows${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>/plugins/platforms/")
    endif ()
    # Set Qt features here
    foreach (QT_LIB Core Gui OpenGL OpenGLWidgets Widgets Multimedia)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
