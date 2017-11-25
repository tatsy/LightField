set(BUILD_TARGET "LightFieldViewer")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(${CMAKE_CURRENT_LIST_DIR}/viewer/directories.h.in
               ${CMAKE_CURRENT_LIST_DIR}/viewer/directories.h)

file(GLOB SOURCE_FILES "viewer/*.cpp" "viewer/*.h")
file(GLOB SHADER_FILES "shaders/*.vert" "shaders/*.frag")

add_executable(${BUILD_TARGET} ${SOURCE_FILES} ${SHADER_FILES})
qt5_use_modules(${BUILD_TARGET} Widgets OpenGL)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Shader Files" FILES ${SHADER_FILES})

target_link_libraries(${BUILD_TARGET} ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
  set_property(TARGET ${BUILD_TARGET} APPEND PROPERTY LINK_FLAGS "/DEBUG /PROFILE")
endif()
