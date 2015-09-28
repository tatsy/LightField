set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(SOURCES ${SOURCES} Main.cpp LightFieldViewer.h LightFieldViewer.cpp)

add_definitions(-DLIGHT_FIELD_VIEWER_EXPORT)
add_executable(LightFieldViewer ${SOURCES})
qt5_use_modules(LightFieldViewer Widgets OpenGL)

source_group("Sources" FILES ${SOURCES})
