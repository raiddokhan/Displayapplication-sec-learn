cmake_minimum_required(VERSION 3.16)
project(MyProject VERSION 0.0.1 DESCRIPTION "UI Feature with Qt and VTK")

include(ExternalProject)

# Clear the CMake cache
if (EXISTS "${CMAKE_BINARY_DIR}/CMakeCache.txt")
    file(REMOVE "${CMAKE_BINARY_DIR}/CMakeCache.txt")
endif()

# Setting CMake policies to support older CMake versions
if(POLICY CMP0020)
    cmake_policy(SET CMP0020 NEW)
endif()
if(POLICY CMP0071)
    cmake_policy(SET CMP0071 NEW)
endif()

# Options
option(BUILD_CROSS_COMPILE "Cross Compile" OFF)
option(BUILD_VTK_FROM_SOURCE "Build VTK from source" OFF)
option(BUILD_WITH_OPENGL_SUPPORT "Build With OpenGL support" ON)
##to see the inludable .h files (debuging only )
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Path variables
set(JSON_DIR "/home/mannigel/libs/json/single_include")
set(NAMED_PIPE_DIR "/home/mannigel/Projects/Sec/lib/named_pipe")
set(VTK_GIT_REPO "https://gitlab.kitware.com/vtk/vtk.git")

# Specify custom Qt installation path if needed
set(Qt6_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt6")

# Add Qt components
set(qt_components Core Gui Widgets)

# Cross-compilation settings
if(BUILD_CROSS_COMPILE)
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR aarch64)
    set(CMAKE_C_COMPILER /bin/arm-linux-gnueabi-gcc)
    set(CMAKE_CXX_COMPILER /bin/arm-linux-gnueabi-cpp)
    set(ENV{PKG_CONFIG_PATH} "/usr/aarch64-linux-gnu/lib/pkgconfig")
    set(ENV{PKG_CONFIG_LIBDIR} "/usr/aarch64-linux-gnu/lib/pkgconfig")
    set(ENV{PKG_CONFIG_SYSROOT_DIR} "/")
    find_program(PKG_CONFIG_EXECUTABLE pkg-config)
    if(NOT PKG_CONFIG_EXECUTABLE)
        message(FATAL_ERROR "pkg-config executable not found")
    else()
        message(STATUS "Using pkg-config executable: ${PKG_CONFIG_EXECUTABLE}")
    endif()
endif()

# GCC specific flags
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libstdc++ -static-libgcc")
endif()



# Include Named Pipe directory
message(STATUS "Named Pipe Directory: ${NAMED_PIPE_DIR}")
if(EXISTS "${NAMED_PIPE_DIR}")
    add_subdirectory(${NAMED_PIPE_DIR})
else()
    message(FATAL_ERROR "Missing dependency: ${NAMED_PIPE_DIR}")
endif()

#======================= INCLUSION OF Qt =======================#
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_PREFIX_PATH $ENV{QTDIR})
#adding OpenGl latter 
find_package(QT NAMES Qt6 COMPONENTS ${qt_components} REQUIRED PATHS ${Qt6_DIR} NO_DEFAULT_PATH)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS ${qt_components} REQUIRED PATHS ${Qt6_DIR} NO_DEFAULT_PATH)
set(CMAKE_AUTOUIC_SEARCH_PATHS ${CMAKE_SOURCE_DIR}/ui_elements/ui_files)

# Project sources
file(GLOB_RECURSE PROJECT_HEADERS "include/*/*.h")
file(GLOB_RECURSE PROJECT_SOURCES "src/*/*.cpp")
file(GLOB UI_FILES "${CMAKE_SOURCE_DIR}/ui_elements/ui_files/*.ui")

message(STATUS "UI files found:")
foreach(ui_file ${UI_FILES})
    message(STATUS "  ${ui_file}")
endforeach()

include_directories(${PROJECT_SOURCE_DIR}/include)

# Add executable
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(MyProject
        MANUAL_FINALIZATION
        main.cpp
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${UI_FILES}
    )
else()
    add_executable(MyProject
        main.cpp
        ${PROJECT_SOURCES}
        ${PROJECT_HEADERS}
        ${UI_FILES}
    )
endif()

# Include JSON directory => conflict with vtk :)
message(STATUS "JSON Directory: ${JSON_DIR}")
if(EXISTS "${JSON_DIR}")
    target_include_directories(MyProject PRIVATE ${JSON_DIR})
    # include_directories(${JSON_DIR})
else()
    message(FATAL_ERROR "Missing dependency: ${JSON_DIR}")
endif()

target_include_directories(MyProject PRIVATE ${CMAKE_CURRENT_BINARY_DIR})

# Add executable properties
set_target_properties(MyProject PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(QT_VERSION_MAJOR EQUAL 6)
    list(APPEND qt_components OpenGLWidgets)
    qt_finalize_executable(MyProject)
endif()
list(SORT qt_components)

# VTK configuration
if(BUILD_VTK_FROM_SOURCE)
    ExternalProject_Add(VTK
        GIT_REPOSITORY    ${VTK_GIT_REPO}
        GIT_TAG           master
        SOURCE_DIR        "${CMAKE_BINARY_DIR}/lib/VTK"
        BINARY_DIR        "${CMAKE_BINARY_DIR}/lib/VTK-build"
        CMAKE_ARGS        
            -DBUILD_SHARED_LIBS=OFF
            -DVTK_GROUP_ENABLE_Qt=YES
            -DVTK_MODULE_ENABLE_VTK_GUISupportQt=YES
            -DVTK_MODULE_ENABLE_VTK_RenderingQt=YES
            -DVTK_MODULE_ENABLE_VTK_RenderingOpenGL2=YES
            -DVTK_MODULE_ENABLE_VTK_RenderingVolumeOpenGL2=YES
            -DCMAKE_INSTALL_PREFIX=${CMAKE_BINARY_DIR}/lib/VTK-install
        BUILD_COMMAND     "${CMAKE_COMMAND}" --build "${CMAKE_BINARY_DIR}/lib/VTK-build" --config Release
        INSTALL_COMMAND   "${CMAKE_COMMAND}" --install "${CMAKE_BINARY_DIR}/lib/VTK-build" --prefix "${CMAKE_BINARY_DIR}/lib/VTK-install"
        TEST_COMMAND      ""
    )
    add_dependencies(MyProject VTK)

    set(VTK_INSTALL_DIR ${CMAKE_BINARY_DIR}/lib/VTK-install)
    set(VTK_INCLUDE_DIR ${VTK_INSTALL_DIR}/include/vtk-9.0)
    set(VTK_LIB_DIR ${VTK_INSTALL_DIR}/lib)
    set(VTK_CMAKE_DIR ${VTK_INSTALL_DIR}/lib/cmake/vtk-9.0)
else()
    set(VTK_INCLUDE_DIR /home/mannigel/vtk_build/VTK/build/lib/)
    set(VTK_ThirdParty_INCLUDE_DIR /home/mannigel/vtk_build/VTK/build/ThirdParty)
    set(VTK_LIB_DIR /home/mannigel/vtk_build/VTK/build/)
    set(VTK_CMAKE_DIR /home/mannigel/vtk_build/VTK/build/)
endif()


# Add VTK include directories
include_directories(${VTK_INCLUDE_DIR})
include_directories(${VTK_ThirdParty_INCLUDE_DIR})


# Ensure we use the correct VTK and not the older version
set(VTK_DIR ${VTK_CMAKE_DIR})
# find_package(VTK REQUIRED COMPONENTS CommonCore CommonDataModel IOXML ChartsCore RenderingContext2D)
find_package(VTK COMPONENTS 
  CommonColor
  CommonCore
  FiltersSources
  InteractionStyle
  InteractionWidgets
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
  GUISupportQt
  RenderingQt
  CommonColor
  CommonCore
  IOImage
  InteractionImage
  InteractionStyle
  RenderingContextOpenGL2
  RenderingCore
  RenderingFreeType
  RenderingGL2PSOpenGL2
  RenderingOpenGL2
)

if(NOT VTK_FOUND)
    message(FATAL_ERROR "VTK not found!")
endif()

if(NOT(TARGET VTK::GUISupportQt))
  message(FATAL_ERROR "VTK not built with Qt support.")
endif()


message("print use file ${VTK_USE_FILE}")
include(${VTK_USE_FILE})

#json being a pain in the ass in vtk
target_include_directories(MyProject PUBLIC /home/mannigel/vtk_build/VTK/build/ThirdParty/nlohmannjson)

message(STATUS "VTK_LIBRARIES: ${VTK_LIBRARIES}")

# Add VTK libraries to the target
target_link_libraries(MyProject PRIVATE
    named_pipe
    Qt${QT_VERSION_MAJOR}::Widgets
    ${qt_components}
    ${VTK_LIBRARIES}
)

# Find and link OpenGL if enabled
if(BUILD_WITH_OPENGL_SUPPORT)
    find_package(OpenGL REQUIRED)
    if(NOT OpenGL_FOUND)
        message(FATAL_ERROR "OpenGL not found!")
    endif()
    target_link_libraries(MyProject PRIVATE ${OPENGL_LIBRARIES})

    find_package(X11 REQUIRED)
    if(NOT X11_FOUND)
        message(FATAL_ERROR "X11 not found!")
    endif()
    target_link_libraries(MyProject PRIVATE ${X11_LIBRARIES})
endif()

# Enable clang-tidy
set(CLANG_TIDY_CHECKS "readability-*,modernize-*")
find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_CXX_CLANG_TIDY "${CLANG_TIDY_EXE};-checks=${CLANG_TIDY_CHECKS}")
endif()

# Debug include directories
get_property(dirs DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY INCLUDE_DIRECTORIES)
message("Include directories are: ${dirs}")

# Add MyProject project

# Prevent a "command line is too long" failure in Windows.
set(CMAKE_NINJA_FORCE_RESPONSE_FILE "ON" CACHE BOOL "Force Ninja to use response files.")
# add_executable(MyProject MACOSX_BUNDLE MyProject.cxx)
target_link_libraries(MyProject PRIVATE ${VTK_LIBRARIES})

# vtk_module_autoinit is needed
vtk_module_autoinit(
  TARGETS MyProject
  MODULES ${VTK_LIBRARIES}
)
