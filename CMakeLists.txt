cmake_minimum_required(VERSION 3.6)

include(FetchContent)

project(angry-aim)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif ()

#if (MSVC)
#    add_compile_options(/showIncludes)
#else ()
#    add_compile_options(-H)
#endif()

# add freetype
add_subdirectory("deps/freetype")
include_directories("deps/freetype/include")

# add opengl
find_package(OpenGL REQUIRED)
include_directories(${OPENGL_INCLUDE_DIRS})

# add glfw
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")
add_subdirectory("deps/glfw")
include_directories("${GLFW_SOURCE_DIR}/deps")

# add glad
set(GLAD_SRC "deps/glad/src/glad.c")
include_directories("deps/glad/include")

# add glm
set(GLM_BUILD_TESTS OFF CACHE BOOL "Disable GLM tests")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Build shared libs")
add_subdirectory("deps/glm")
include_directories("deps/glm")

# add assimp
set(ASSIMP_BUILD_GLTF_EXPORTER OFF CACHE BOOL "Disable GLTF Exporter" FORCE)
set(ASSIMP_BUILD_OBJ_IMPORTER ON CACHE BOOL "Enable OBJ importer" FORCE)
add_subdirectory("deps/assimp/")

#set(assimp_DIR "deps/assimp/installdir/lib/cmake/assimp-6.0")
#set(ASSIMP_USE_STATIC_LIBS ON CACHE BOOL "Use static Assimp" FORCE)
#find_package(assimp REQUIRED)

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
        src/render/RenderFont.cpp
        src/render/RenderFont.h
        src/ui/Label.cpp
        src/ui/Label.h
        src/ui/components.h
        src/screens/ScreenSettings.cpp
        src/screens/ScreenSettings.h
        src/screens/ScreenScene.cpp
        src/screens/ScreenScene.h
        src/scene/Scene.cpp
        src/scene/Scene.h
        src/world/World.cpp
        src/world/World.h
        src/inc/inc_glm.h
        src/render/RenderScene.cpp
        src/render/RenderScene.h
        src/render/Mesh.cpp
        src/render/Mesh.h
        src/render/Model.cpp
        src/render/Model.h
        src/lib/stb_image.h
        src/lib/stb_impl.cpp
        src/world/entities/MainPlayer.cpp
        src/world/entities/MainPlayer.h
        src/scene/SceneBackpack.cpp
        src/scene/SceneBackpack.h
        src/scene/default/SceneDefault.cpp
        src/scene/default/SceneDefault.h
        src/render/RenderSceneDefault.cpp
        src/render/RenderSceneDefault.h
        src/world/entities/StrafingTarget.cpp
        src/world/entities/StrafingTarget.h
        src/util/time_util.h
        src/util/math_util.h
        src/DebugInfo.cpp
        src/DebugInfo.h
)

# add executable
add_executable(angry-aim ${angry-aim-src} ${GLAD_SRC})


target_include_directories(angry-aim PRIVATE src)
target_link_libraries(angry-aim ${OPENGL_LIBRARIES} glfw freetype assimp::assimp)

# copy assets
add_custom_command(TARGET angry-aim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:angry-aim>/assets"
)