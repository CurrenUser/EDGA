cmake_minimum_required(VERSION 3.29 FATAL_ERROR)
SET(PROJECT_NANE "EDGA")

SET(INCLUDE_IMGUI
    external/imgui/imgui.h
    external/imgui/backends/imgui_impl_glfw.h
    external/imgui/backends/imgui_impl_opengl3.h
)


SET(SOURCE_IMGUI
    external/imgui/imgui.cpp
    external/imgui/imgui_demo.cpp
    external/imgui/imgui_draw.cpp
    external/imgui/imgui_tables.cpp
    external/imgui/imgui_widgets.cpp
    external/imgui/backends/imgui_impl_glfw.cpp
    external/imgui/backends/imgui_impl_opengl3.cpp
)


project(${PROJECT_NANE})
add_executable(${PROJECT_NANE} src/main.cpp)


add_subdirectory(external/glfw)
add_subdirectory(external/glad)

add_library(ImGui STATIC
   ${INCLUDE_IMGUI}
   ${SOURCE_IMGUI}
)

target_include_directories(ImGui PUBLIC external/)
target_include_directories(ImGui PUBLIC external/imgui)
target_link_libraries(ImGui PRIVATE glad glfw)
target_link_libraries(${PROJECT_NANE} PRIVATE ImGui glfw glad)