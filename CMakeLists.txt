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

# Defaulted
find_package(Qt6 COMPONENTS
        Core
        Gui
        OpenGL
        OpenGLWidgets
        Widgets
        REQUIRED)

add_executable(CSE165Project glwidget.cpp glwidget.h main.cpp window.cpp window.h textures.qrc)

# Set Qt features here
target_link_libraries(CSE165Project
        Qt::Core
        Qt::Gui
        Qt::OpenGL
        Qt::OpenGLWidgets
        Qt::Widgets
        )

# Set resources here
set(textures_resource_files
        "images/side1.png"
        "images/side2.png"
        "images/side3.png"
        "images/side4.png"
        "images/side5.png"
        "images/side6.png"
        )

qt6_add_resources(textures "textures"
        FILES
        ${textures_resource_files}
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
    foreach (QT_LIB Core Gui OpenGL OpenGLWidgets Widgets)
        add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                COMMAND ${CMAKE_COMMAND} -E copy
                "${QT_INSTALL_PATH}/bin/Qt6${QT_LIB}${DEBUG_SUFFIX}.dll"
                "$<TARGET_FILE_DIR:${PROJECT_NAME}>")
    endforeach (QT_LIB)
endif ()
