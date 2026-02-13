cmake_minimum_required(VERSION 3.16)

project(Kazia VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Qt 依赖
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets)

# Filament 依赖
set(FILAMENT_DIR ${CMAKE_SOURCE_DIR}/thirdparty/filament)
set(FILAMENT_LIB_DIR ${FILAMENT_DIR}/lib/x86_64/mdd)
find_library(FILAMENT_LIBRARY filament PATHS ${FILAMENT_LIB_DIR})
find_library(FILAMENT_UTILS_LIBRARY utils ${FILAMENT_LIB_DIR})
find_library(FILAMENT_GLTFIO_LIBRARY gltfio ${FILAMENT_LIB_DIR})
find_library(FILAMENT_BACKEND_LIBRARY backend ${FILAMENT_LIB_DIR})
find_library(FILAMENT_BLUEGL_LIBRARY bluegl ${FILAMENT_LIB_DIR})
find_library(FILAMENT_BLUEVK_LIBRARY bluevk ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILAFALT_LIBRARY filaflat ${FILAMENT_LIB_DIR})
find_library(FILAMENT_ZSTD_LIBRARY zstd ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILABRIDGE_LIBRARY filabridge ${FILAMENT_LIB_DIR})
find_library(FILAMENT_MATDBG_LIBRARY matdbg ${FILAMENT_LIB_DIR})
find_library(FILAMENT_SMOLV_LIBRARY smol-v ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILAMAT_LIBRARY filamat ${FILAMENT_LIB_DIR})
find_library(FILAMENT_ABSEIL_LIBRARY abseil ${FILAMENT_LIB_DIR})
find_library(FILAMENT_SHADERS_LIBRARY shaders ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILAMESHIO_LIBRARY filameshio ${FILAMENT_LIB_DIR})
find_library(FILAMENT_GEOMETRY_LIBRARY geometry ${FILAMENT_LIB_DIR})
find_library(FILAMENT_IMAGE_LIBRARY image ${FILAMENT_LIB_DIR})

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${FILAMENT_DIR}/include
    ${FILAMENT_DIR}/include/math
    ${FILAMENT_DIR}/include/utils
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Gui_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Qt6OpenGL_INCLUDE_DIRS}
)

# 源代码文件
set(SOURCE_FILES
    src/core/FilamentEngine.cpp
    src/core/Renderer.cpp
    src/ui/MainWindow.cpp
    src/ui/RenderWidget.cpp
    src/ui/SceneTree.cpp
    src/ui/PropertiesPanel.cpp
    src/ui/TitleBar.cpp
    src/ui/DockTitleBar.cpp
    src/scene/Scene.cpp
    src/scene/GameObject.cpp
    src/scene/Camera.cpp
    src/utils/Logger.cpp
)

# 头文件
set(HEADER_FILES
    src/core/FilamentEngine.h
    src/core/Renderer.h
    src/ui/MainWindow.h
    src/ui/RenderWidget.h
    src/ui/SceneTree.h
    src/ui/PropertiesPanel.h
    src/ui/TitleBar.h
    src/ui/DockTitleBar.h
    src/scene/Scene.h
    src/scene/GameObject.h
    src/scene/Camera.h
    src/utils/Logger.h
)

# 资源文件
set(RESOURCE_FILES
    assets.qrc
)

# 构建目标
add_executable(Kazia
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${RESOURCE_FILES}
    main.cpp
)

# 链接库
target_link_libraries(Kazia PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    ${FILAMENT_LIBRARY}
    ${FILAMENT_UTILS_LIBRARY}
    ${FILAMENT_BACKEND_LIBRARY}
    ${FILAMENT_BLUEGL_LIBRARY}
    ${FILAMENT_BLUEVK_LIBRARY}
    ${FILAMENT_FILAFALT_LIBRARY}
    ${FILAMENT_ZSTD_LIBRARY}
    ${FILAMENT_FILABRIDGE_LIBRARY}
    ${FILAMENT_SMOLV_LIBRARY}
    ${FILAMENT_MATDBG_LIBRARY}
    ${FILAMENT_FILAMAT_LIBRARY}
    ${FILAMENT_GLTFIO_LIBRARY}
    ${FILAMENT_ABSEIL_LIBRARY}
    ${FILAMENT_SHADERS_LIBRARY}
    ${FILAMENT_FILAMESHIO_LIBRARY}
    ${FILAMENT_GEOMETRY_LIBRARY}
    ${FILAMENT_IMAGE_LIBRARY}
    opengl32
)

# 复制 Filament 库文件到输出目录
if(WIN32)
    add_custom_command(TARGET Kazia POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${FILAMENT_DIR}/bin
        $<TARGET_FILE_DIR:Kazia>
    )
    add_custom_command(TARGET Kazia POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${FILAMENT_DIR}/lib/x86_64
        $<TARGET_FILE_DIR:Kazia>
    )
endif()

# 安装配置
install(TARGETS Kazia
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Kazia)
endif()

if (MSVC)
    if (CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
        add_compile_options(/Zi)
        add_compile_options(/Od)
        add_link_options(/DEBUG)
        add_compile_definitions(_ITERATOR_DEBUG_LEVEL=2)
    endif()
endif()