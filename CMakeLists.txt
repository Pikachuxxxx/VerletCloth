cmake_minimum_required(VERSION 3.20)

# set the project name and version
project(VerletCloth)

# specify the C++ standard and making it mandatory
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Place the cmake ALL_BUILD and ZERO_CHECK and INSTALL projects udner a filter/folder
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(PREDEFINED_TARGETS_FOLDER "_CMakePredefinedTargets")

include_directories(
    "${CMAKE_SOURCE_DIR}/vendor/GLEW/include"
    "${CMAKE_SOURCE_DIR}/vendor/GLFW/include"
    "${CMAKE_SOURCE_DIR}/vendor/glm"
    "${CMAKE_SOURCE_DIR}/vendor/stb_headers"
    "${CMAKE_SOURCE_DIR}/vendor/OpenGL"
    "${CMAKE_SOURCE_DIR}/vendor/imgui-docking"
)

link_directories(
    "${CMAKE_SOURCE_DIR}/vendor/GLFW/lib"
    "${CMAKE_SOURCE_DIR}/vendor/GLEW/lib"
)

# Add source files
file(GLOB_RECURSE OpenGL_SOURCE_FILES
	${CMAKE_SOURCE_DIR}/vendor/OpenGL/*.c
    ${CMAKE_SOURCE_DIR}/vendor/OpenGL/*.cpp
)

# Add source header files
file(GLOB_RECURSE OpenGL_HEADER_FILES
	${CMAKE_SOURCE_DIR}/vendor/OpenGL/*.h
    ${CMAKE_SOURCE_DIR}/vendor/OpenGL/*.hpp
)

file(GLOB VENDOR_SRC_FILES
    ${CMAKE_SOURCE_DIR}/vendor/imgui-docking/imgui_demo.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui-docking/imgui_draw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui-docking/imgui_impl_glfw.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui-docking/imgui_impl_opengl3.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui-docking/imgui_widgets.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui-docking/imgui_tables.cpp
    ${CMAKE_SOURCE_DIR}/vendor/imgui-docking/imgui.cpp
)

file(GLOB_RECURSE SOURCE_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Compile the source files for the executable
add_executable(VerletCloth ${SOURCE_FILES} ${OpenGL_HEADER_FILES} ${OpenGL_SOURCE_FILES} ${VENDOR_SRC_FILES})

if (APPLE)
    target_link_libraries(VerletCloth glfw3 GLEW "-framework CoreFoundation -framework CoreAudio -framework OpenAL -framework OpenGL -framework Cocoa -framework IOKit -framework CoreGraphics")
endif (APPLE)

# Run the binary built using 'make run' command
add_custom_target(run
    COMMAND VerletCloth
    DEPENDS VerletCloth
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)
