cmake_minimum_required (VERSION 3.8)

project ("point-generator")

add_library(glfw STATIC IMPORTED) 
set_target_properties(glfw PROPERTIES IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/glfw3.lib") 

add_executable (point-generator "src/main.cpp" 
"src/glad.c" 
"src/imgui.cpp" 
"src/imgui_demo.cpp" 
"src/imgui_draw.cpp" 
"src/imgui_tables.cpp" 
"src/imgui_widgets.cpp" 
"src/imgui_impl_glfw.cpp" 
"src/imgui_impl_opengl3.cpp"
"src/shader.cpp")

target_link_libraries(point-generator glfw)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET point-generator PROPERTY CXX_STANDARD 20)
endif()

target_include_directories(point-generator PRIVATE ${CMAKE_SOURCE_DIR}/headers)