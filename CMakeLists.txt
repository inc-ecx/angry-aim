cmake_minimum_required(VERSION 3.6)

include(FetchContent)

project(angry-aim)

set(CMAKE_CXX_STANDARD 20)

# add_compile_options(-H)

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
        src/screens/ScreenTest.cpp
        src/screens/ScreenTest.h
        src/scenes/UiDrill.cpp
        src/scenes/UiDrill.h
        src/scenes/Scene.cpp
        src/scenes/Scene.h
        src/world/model/World.cpp
        src/world/model/World.h
        src/inc/inc_glm.h
        src/render/RenderScene.cpp
        src/render/RenderScene.h
        src/render/Mesh.cpp
        src/render/Mesh.h
        src/render/Model.cpp
        src/render/Model.h
        src/lib/stb_image.h
        src/lib/stb_impl.cpp
        src/entities/MainPlayer.cpp
        src/entities/MainPlayer.h
        src/scenes/SceneBackpack.cpp
        src/scenes/SceneBackpack.h
        src/render/RenderSceneDrill.cpp
        src/render/RenderSceneDrill.h
        src/entities/StrafingTarget.cpp
        src/entities/StrafingTarget.h
        src/util/time_util.h
        src/util/math_util.h
        src/util/file_util.h
        src/DebugInfo.cpp
        src/DebugInfo.h
        src/screens/ScreenResult.cpp
        src/screens/ScreenResult.h
        src/world/controller/WorldController.cpp
        src/world/controller/WorldController.h
        src/world/model/Entity.cpp
        src/world/model/Entity.h
        src/world/controller/Controller.cpp
        src/world/controller/Controller.h
        src/world_controllers/CameraController.cpp
        src/world_controllers/CameraController.h
        src/entities/components/Looking.cpp
        src/entities/components/Looking.h
        src/world_controllers/MoveController.cpp
        src/world_controllers/MoveController.h
        src/entities/components/Positioned.cpp
        src/entities/components/Positioned.h
        src/world_controllers/components/Toggelable.cpp
        src/world_controllers/components/Toggelable.h
        src/drill/model/Drill.cpp
        src/drill/model/Drill.h
        src/drill/model/DrillManager.h
        src/scenes/SceneDrill.cpp
        src/scenes/SceneDrill.h
        src/drill/controller/DrillController.cpp
        src/drill/controller/DrillController.h
        src/drill_controllers/DrillSimple.cpp
        src/drill_controllers/DrillSimple.h
        src/entities/SimpleTarget.cpp
        src/entities/SimpleTarget.h
        src/entities/Miss.h
        src/world/WorldUtil.cpp
        src/world/WorldUtil.h
        src/drill_controllers/DrillDefault.cpp
        src/drill_controllers/DrillDefault.h
        src/ui/PanelY.cpp
        src/ui/PanelY.h
        src/drill_controllers/FactoryDrill.cpp
        src/drill_controllers/FactoryDrill.h
        src/drill_controllers/DrillMicro.cpp
        src/drill_controllers/DrillMicro.h
        src/world_controllers/cam_consts.h
        src/state/State.cpp
        src/state/State.h
        src/state/StateDrill.cpp
        src/state/StateDrill.h
        src/lib/json.h
        src/inc/inc_json.h
        src/state/ManagerState.cpp
        src/state/ManagerState.h
        src/drill/model/DrillManager.cpp
        src/Log.cpp
        src/Log.h
        src/state/StateSettings.cpp
        src/state/StateSettings.h
        src/ui/Field.h
        src/ui/Field.cpp
        src/ui/Slider.cpp
        src/ui/Slider.h
        src/ui/Range.cpp
        src/ui/Range.h
        src/renderers/RenderCrosshair.cpp
        src/renderers/RenderCrosshair.h
        src/screens/ScreenPause.cpp
        src/screens/ScreenPause.h
        src/util/Observable.cpp
        src/util/Observable.h
        src/screens/DialogHelp.cpp
        src/screens/DialogHelp.h
        src/lib/miniaudio.h
        src/lib/miniaudio_impl.cpp
        src/sound/Sound.h
        src/scenes/SceneDrillResources.h
        src/sound/Sound.cpp
        src/renderers/PipelineUi.cpp
        src/renderers/PipelineUi.h
        src/render/Framebuffer.cpp
        src/render/Framebuffer.h
        src/ui/UiRenderParams.cpp
        src/ui/UiRenderParams.h
        src/ui/UiUtil.cpp
        src/ui/UiUtil.h
        src/render/RenderPrimitive.cpp
        src/render/RenderPrimitive.h
        src/render/Shader.cpp
        src/render/Shader.h
        src/ui/Color.cpp
        src/ui/Color.h
        src/render/Texture.cpp
        src/render/Texture.h
        src/ui/Image.cpp
        src/ui/Image.h
        src/drill_controllers/DrillDynamic.cpp
        src/drill_controllers/DrillDynamic.h
        src/drill_controllers/UtilDrill.cpp
        src/drill_controllers/UtilDrill.h
        src/drill_controllers/DrillAce.cpp
        src/drill_controllers/DrillAce.h
        src/drill_controllers/shared/TimeController.cpp
        src/drill_controllers/shared/TimeController.h
)

# add executable
add_executable(angry-aim ${angry-aim-src} ${GLAD_SRC})

target_include_directories(angry-aim PRIVATE src)
target_link_libraries(angry-aim PRIVATE ${OPENGL_LIBRARIES} glfw freetype assimp::assimp)

# copy assets
add_custom_command(TARGET angry-aim POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:angry-aim>/assets"
)