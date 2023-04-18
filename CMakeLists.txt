cmake_minimum_required(VERSION 3.16)
project(CSE165Project)

# C++20 standard bc i like my lambdas
set(CMAKE_CXX_STANDARD 20)

# Use Qt6 compilers for resources and UI files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(NOT DEFINED ENV{USERNAME})
    set(ENV{USERNAME} $ENV{USER})
endif()

# My Windows Install
if ($ENV{USERNAME} STREQUAL "willi")
    set(CMAKE_PREFIX_PATH "C:\\Qt\\6.5.0\\msvc2019_64")
    find_package (Qt6 PATHS "C:\\Qt\\6.5.0\\msvc2019_64" NO_DEFAULT_PATH)
    set(FREETYPE_LIBRARY "C:/Users/willi/Documents/Programming Projects/freetype-bin/rdll/win64/freetype.lib")
    include_directories("../freetype-bin/include")
    set(CMAKE_DISABLE_FIND_PACKAGE_Freetype ON)
endif()

# My MacOS Install
if ($ENV{USERNAME} STREQUAL "doggysazhi")
    set(CMAKE_PREFIX_PATH "/Users/doggysazhi/Qt/6.5.0/macos")
    find_package (Qt6 PATHS "/Users/doggysazhi/Qt/6.5.0/macos" NO_DEFAULT_PATH)
    set(FREETYPE_LIBRARY "/opt/homebrew/Cellar/freetype/2.13.0_1/lib/libfreetype.dylib")
    include_directories("/opt/homebrew/Cellar/freetype/2.13.0_1/include/freetype2")
    set(CMAKE_DISABLE_FIND_PACKAGE_Freetype ON)
endif()

# Defaulted
find_package(Qt6 COMPONENTS
        Core
        Gui
        OpenGL
        OpenGLWidgets
        Widgets
        Multimedia
        REQUIRED)
find_package(Freetype) # unfortunately i can't make it required since my windows build of freetype doesn't have cmake files

file(GLOB_RECURSE HEADERS include/*.h)
file(GLOB_RECURSE SOURCES src/*.cpp)
qt6_add_resources(SOURCES assets/resources.qrc)
add_executable(${CMAKE_PROJECT_NAME} ${HEADERS} ${SOURCES})
# idk what this is here for
target_link_libraries(${CMAKE_PROJECT_NAME} ${FREETYPE_LIBRARIES})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE include/ ${FREETYPE_INCLUDE_DIRS})

# Set Qt features here
target_link_libraries(${CMAKE_PROJECT_NAME}
        Qt::Core
        Qt::Gui
        Qt::OpenGL
        Qt::OpenGLWidgets
        Qt::Widgets
        Qt::Multimedia
        ${FREETYPE_LIBRARY}
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
