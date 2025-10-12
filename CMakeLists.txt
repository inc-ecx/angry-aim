cmake_minimum_required(VERSION 3.6)

project(angry-aim)

find_package(OpenGL REQUIRED)

include_directories(${OPENGL_INCLUDE_DIRS})

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

add_subdirectory(glfw)

include_directories("${GLFW_SOURCE_DIR}/deps")

if (MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif ()

set(GLAD_SRC "${CMAKE_SOURCE_DIR}/glad/src/glad.c")
include_directories("${CMAKE_SOURCE_DIR}/glad/include")

set(angry-aim-src
        src/main.cpp
        src/Application.cpp
        src/Application.h
        src/ui/Ui.cpp
        src/ui/Ui.h
        src/ui/UiEvent.cpp
        src/ui/UiEvent.h
        src/render/RenderUi.cpp
        src/render/RenderUi.h
        src/ui/Column.cpp
        src/ui/Column.h
        src/ui/Cell.cpp
        src/ui/Cell.h
        src/ui/Button.cpp
        src/ui/Button.h
        src/screens/ScreenMain.cpp
        src/screens/ScreenMain.h
        src/inc/inc_glfw.h
)

add_executable(angry-aim ${angry-aim-src} ${GLAD_SRC})

target_link_libraries(angry-aim ${OPENGL_LIBRARIES} glfw)
if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT angry-aim)
endif ()
