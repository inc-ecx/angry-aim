cmake_minimum_required(VERSION 3.6)

include(FetchContent)

project(angry-aim)

# add opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# add glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory(glfw)
include_directories("${GLFW_SOURCE_DIR}/deps")

# set startup project
if (MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif ()

# add glad
set(GLAD_SRC "${CMAKE_SOURCE_DIR}/glad/src/glad.c")
include_directories("${CMAKE_SOURCE_DIR}/glad/include")

# add application sources
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
        src/ui/Row.cpp
        src/ui/Row.h
        src/ui/Cell.cpp
        src/ui/Cell.h
        src/ui/Button.cpp
        src/ui/Button.h
        src/screens/ScreenMain.cpp
        src/screens/ScreenMain.h
        src/inc/inc_glfw.h
)

# fetch glm
FetchContent_Declare(
        glm
        GIT_REPOSITORY	https://github.com/g-truc/glm.git
        GIT_TAG 	0af55ccecd98d4e5a8d1fad7de25ba429d60e863
)
FetchContent_MakeAvailable(glm)

# add executable
add_executable(angry-aim ${angry-aim-src} ${GLAD_SRC})

target_link_libraries(angry-aim ${OPENGL_LIBRARIES} glfw glm::glm)
if (MSVC)
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT angry-aim)
endif ()
