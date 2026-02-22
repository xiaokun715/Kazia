cmake_minimum_required(VERSION 3.16)

project(Kazia VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set_property(GLOBAL PROPERTY AUTOGEN_SOURCE_GROUP "Qt Autogen")
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
    
    # Debug 配置选项
    add_compile_options($<$<CONFIG:Debug>:/Zi>)
    add_compile_options($<$<CONFIG:Debug>:/Od>)
    add_link_options($<$<CONFIG:Debug>:/DEBUG>)
    
    # 统一迭代器调试级别
    add_compile_options($<$<CONFIG:Debug>:/D_ITERATOR_DEBUG_LEVEL=0>)
    add_compile_definitions($<$<CONFIG:Debug>:_ITERATOR_DEBUG_LEVEL=0>)

    #set_target_properties(Kazia PROPERTIES
     #   VS_DEBUGGER_COMMAND
      #  "$<INSTALL_PREFIX>/bin/$<TARGET_FILE_NAME:Kazia>"
    #)
endif()

function(get_resgen_vars ARCHIVE_DIR ARCHIVE_NAME)
    set(OUTPUTS
        ${ARCHIVE_DIR}/${ARCHIVE_NAME}.bin
        ${ARCHIVE_DIR}/${ARCHIVE_NAME}.S
        ${ARCHIVE_DIR}/${ARCHIVE_NAME}.apple.S
        ${ARCHIVE_DIR}/${ARCHIVE_NAME}.h
    )
    if (IOS)
        set(ASM_ARCH_FLAG "-arch ${DIST_ARCH}")
    endif()
    if (APPLE)
        set(ASM_SUFFIX ".apple")
    endif()
    set(RESGEN_HEADER "${ARCHIVE_DIR}/${ARCHIVE_NAME}.h" PARENT_SCOPE)
    # Visual Studio makes it difficult to use assembly without using MASM. MASM doesn't support
    # the equivalent of .incbin, so on Windows we'll just tell resgen to output a C file.
    if (WEBGL OR WIN32 OR ANDROID_ON_WINDOWS)
        set(RESGEN_OUTPUTS "${OUTPUTS};${ARCHIVE_DIR}/${ARCHIVE_NAME}.c" PARENT_SCOPE)
        set(RESGEN_FLAGS -qcx ${ARCHIVE_DIR} -p ${ARCHIVE_NAME} PARENT_SCOPE)
        set(RESGEN_SOURCE "${ARCHIVE_DIR}/${ARCHIVE_NAME}.c" PARENT_SCOPE)
    else()
        set(RESGEN_OUTPUTS "${OUTPUTS}" PARENT_SCOPE)
        set(RESGEN_FLAGS -qx ${ARCHIVE_DIR} -p ${ARCHIVE_NAME} PARENT_SCOPE)
        set(RESGEN_SOURCE "${ARCHIVE_DIR}/${ARCHIVE_NAME}${ASM_SUFFIX}.S" PARENT_SCOPE)
        set(RESGEN_SOURCE_FLAGS "-I'${ARCHIVE_DIR}' ${ASM_ARCH_FLAG}" PARENT_SCOPE)
    endif()
endfunction()

# Qt 依赖
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets OpenGLWidgets Quick QuickWidgets)

# Filament 依赖
set(FILAMENT_DIR ${CMAKE_SOURCE_DIR}/thirdparty/filament)
set(FILAMENT_LIB_DIR ${FILAMENT_DIR}/lib/x86_64)
find_library(FILAMENT_ABSEIL_LIBRARY abseil ${FILAMENT_LIB_DIR})
find_library(FILAMENT_BACKEND_LIBRARY backend ${FILAMENT_LIB_DIR})
find_library(FILAMENT_BASIS_TRANSCODER_LIBRARY basis_transcoder ${FILAMENT_LIB_DIR})
find_library(FILAMENT_BLUEGL_LIBRARY bluegl ${FILAMENT_LIB_DIR})
find_library(FILAMENT_BLUEVK_LIBRARY bluevk ${FILAMENT_LIB_DIR})
find_library(FILAMENT_CAMUTILS_LIBRARY camutils ${FILAMENT_LIB_DIR})
find_library(FILAMENT_CIVETWEB_LIBRARY civetweb ${FILAMENT_LIB_DIR})
find_library(FILAMENT_DRACODEC_LIBRARY dracodec ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FGVIEWER_LIBRARY fgviewer ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILABRIDGE_LIBRARY filabridge ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILAFALT_LIBRARY filaflat ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILAMAT_LIBRARY filamat ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILAMENT_LIBRARY filament ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILAMENT_IBL_PREFILTER_LIBRARY filament-iblprefilter ${FILAMENT_LIB_DIR})
find_library(FILAMENT_FILAMESHIO_LIBRARY filameshio ${FILAMENT_LIB_DIR})
find_library(FILAMENT_GEOMETRY_LIBRARY geometry ${FILAMENT_LIB_DIR})
find_library(FILAMENT_GLTFIO_LIBRARY gltfio ${FILAMENT_LIB_DIR})
find_library(FILAMENT_GLTFIO_CORE_LIBRARY gltfio_core ${FILAMENT_LIB_DIR})
find_library(FILAMENT_IBL_LITE_LIBRARY ibl-lite ${FILAMENT_LIB_DIR})
find_library(FILAMENT_IBL_LIBRARY ibl ${FILAMENT_LIB_DIR})
find_library(FILAMENT_IMAGE_LIBRARY image ${FILAMENT_LIB_DIR})
find_library(FILAMENT_IMAGEIOE_LIBRARY imageio ${FILAMENT_LIB_DIR})
find_library(FILAMENT_KTXREADER_LIBRARY ktxreader ${FILAMENT_LIB_DIR})
find_library(FILAMENT_MATDBG_LIBRARY matdbg ${FILAMENT_LIB_DIR})
find_library(FILAMENT_MATP_LIBRARY matp ${FILAMENT_LIB_DIR})
find_library(FILAMENT_MESHOPTIMIZER_LIBRARY meshoptimizer ${FILAMENT_LIB_DIR})
find_library(FILAMENT_MIKKTSPACE_LIBRARY mikktspace ${FILAMENT_LIB_DIR})
find_library(FILAMENT_PERFETTO_LIBRARY perfetto ${FILAMENT_LIB_DIR})
find_library(FILAMENT_SHADERS_LIBRARY shaders ${FILAMENT_LIB_DIR})
find_library(FILAMENT_SMOLV_LIBRARY smol-v ${FILAMENT_LIB_DIR})
find_library(FILAMENT_STB_LIBRARY stb ${FILAMENT_LIB_DIR})
find_library(FILAMENT_UBERARCHIVE_LIBRARY uberarchive ${FILAMENT_LIB_DIR})
find_library(FILAMENT_UBERZLIB_LIBRARY uberzlib ${FILAMENT_LIB_DIR})
find_library(FILAMENT_UTILS_LIBRARY utils ${FILAMENT_LIB_DIR})
find_library(FILAMENT_VIEWER_LIBRARY viewer ${FILAMENT_LIB_DIR})
find_library(FILAMENT_ZSTD_LIBRARY zstd ${FILAMENT_LIB_DIR})
find_library(FILAMENT_MATH_LIBRARY math ${FILAMENT_LIB_DIR})
find_library(FILAMENT_PNG_LIBRARY png ${FILAMENT_LIB_DIR})
find_library(FILAMENT_TINYEXR_LIBRARY tinyexr ${FILAMENT_LIB_DIR})
find_library(FILAMENT_BASIC_ENCODER_LIBRARY basis_encoder ${FILAMENT_LIB_DIR})
find_library(FILAMENT_Z_LIBRARY z ${FILAMENT_LIB_DIR})

set(RESGEN_EXECUTABLE "${FILAMENT_DIR}/bin/resgen.exe")
set(MATC_EXECUTABLE "${FILAMENT_DIR}/bin/matc.exe")

include_directories(
    ${CMAKE_SOURCE_DIR}/src
    ${FILAMENT_DIR}/include
    ${FILAMENT_DIR}/include/math
    ${FILAMENT_DIR}/include/utils
    ${FILAMENT_DIR}/include/imageio
    ${Qt6Core_INCLUDE_DIRS}
    ${Qt6Gui_INCLUDE_DIRS}
    ${Qt6Widgets_INCLUDE_DIRS}
    ${Qt6OpenGL_INCLUDE_DIRS}
)

# 源代码文件
file(GLOB_RECURSE ALL_SRC CONFIGURE_DEPENDS
    src/*.cpp
    src/*.h
    src/*.qml
    src/*.fs
    src/*.vs
    src/*.glsl
)

set(QML_FILE ${CMAKE_CURRENT_SOURCE_DIR}/src/ui/qml/OverlayUI.qml)

set_source_files_properties(
    ${QML_FILE}
    PROPERTIES
        QT_RESOURCE_ALIAS OverlayUI.qml
)

qt_add_executable(Kazia ${ALL_SRC})

qt6_add_resources(Kazia qmls
    PREFIX "/qmls"
    FILES
        ${QML_FILE}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
             PREFIX ""
             FILES ${ALL_SRC})

# third party
add_subdirectory(thirdparty/Qt-Advanced-Docking-System)
set_target_properties(
    qtadvanceddocking-qt6 
    PROPERTIES
    FOLDER "thirdparty"
)
add_subdirectory(thirdparty/imgui)
set_target_properties(
    imgui
    PROPERTIES
    FOLDER "thirdparty"
)
add_subdirectory(thirdparty/filagui)
set_target_properties(
    filagui
    PROPERTIES
    FOLDER "thirdparty"
)

# 链接库
target_link_libraries(Kazia PRIVATE
    #filagui
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::OpenGLWidgets
    Qt6::Quick
    Qt6::QuickWidgets
    ${FILAMENT_ABSEIL_LIBRARY}
    ${FILAMENT_BACKEND_LIBRARY}
    ${FILAMENT_BASIS_TRANSCODER_LIBRARY}
    ${FILAMENT_BLUEGL_LIBRARY}
    ${FILAMENT_BLUEVK_LIBRARY}
    ${FILAMENT_CAMUTILS_LIBRARY}
    ${FILAMENT_CIVETWEB_LIBRARY}
    ${FILAMENT_DRACODEC_LIBRARY}
    ${FILAMENT_FGVIEWER_LIBRARY}
    ${FILAMENT_FILABRIDGE_LIBRARY}
    ${FILAMENT_FILAFALT_LIBRARY}
    ${FILAMENT_FILAMAT_LIBRARY}
    ${FILAMENT_FILAMENT_LIBRARY}
    ${FILAMENT_FILAMENT_IBL_PREFILTER_LIBRARY}
    ${FILAMENT_FILAMESHIO_LIBRARY}
    ${FILAMENT_GEOMETRY_LIBRARY}
    ${FILAMENT_GLTFIO_LIBRARY}
    ${FILAMENT_GLTFIO_CORE_LIBRARY}
    ${FILAMENT_IBL_LITE_LIBRARY}
    ${FILAMENT_IBL_LIBRARY}
    ${FILAMENT_IMAGE_LIBRARY}
    ${FILAMENT_IMAGEIOE_LIBRARY}
    ${FILAMENT_KTXREADER_LIBRARY}
    ${FILAMENT_MATDBG_LIBRARY}
    ${FILAMENT_MATP_LIBRARY}
    ${FILAMENT_MESHOPTIMIZER_LIBRARY}
    ${FILAMENT_MIKKTSPACE_LIBRARY}
    ${FILAMENT_PERFETTO_LIBRARY}
    ${FILAMENT_SHADERS_LIBRARY}
    ${FILAMENT_SMOLV_LIBRARY}
    ${FILAMENT_STB_LIBRARY}
    ${FILAMENT_UBERARCHIVE_LIBRARY}
    ${FILAMENT_UBERZLIB_LIBRARY}
    ${FILAMENT_UTILS_LIBRARY}
    ${FILAMENT_VIEWER_LIBRARY}
    ${FILAMENT_ZSTD_LIBRARY}
    ${FILAMENT_MATH_LIBRARY}
    ${FILAMENT_PNG_LIBRARY}
    ${FILAMENT_TINYEXR_LIBRARY}
    ${FILAMENT_BASIC_ENCODER_LIBRARY}
    ${FILAMENT_Z_LIBRARY}
    ads::qtadvanceddocking-qt6
    #imgui
    filagui
    opengl32
    Shlwapi
    wsock32
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
