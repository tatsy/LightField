set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(${CMAKE_CURRENT_LIST_DIR}/directories.h.in
               ${CMAKE_CURRENT_LIST_DIR}/directories.h)

set(SOURCES ${SOURCES}
    appmain.cpp
    light_field_widget.cpp
    main_interface.cpp)

set(HEADERS ${HEADERS}
    directories.h
    image_info.h
    light_field_widget.h
    main_interface.h)

add_definitions(-DLIGHT_FIELD_WIDGET_EXPORT)
add_definitions(-DMAIN_INTERFACE_EXPORT)
add_executable(LightFieldViewer ${SOURCES} ${HEADERS})
qt5_use_modules(LightFieldViewer Widgets OpenGL)

source_group("Sources" FILES ${SOURCES} ${HEADERS})

target_link_libraries(LightFieldViewer ${QT_LIBRARIES} ${OPENGL_LIBRARIES})

# ------------------------------------------------------------------------------
# Copy Qt DLLs
# ------------------------------------------------------------------------------
set(QT_LIB_LIST Qt5Widgets
                Qt5Widgetsd
                Qt5Core
                Qt5Cored
                Qt5Gui
                Qt5Guid
                Qt5OpenGL
                Qt5OpenGLd
                Qt5Test
                Qt5Testd
                icuin54
                icuuc54
                icudt54)

# ------------------------------------------------------------------------------
# Copy Qt5 DLLs
# ------------------------------------------------------------------------------
if (WIN32)
  foreach (qtlib ${QT_LIB_LIST})
    add_custom_command(TARGET LightFieldViewer POST_BUILD
                       COMMAND ${CMAKE_COMMAND} -E copy
                       ARGS ${CMAKE_PREFIX_PATH}/bin/${qtlib}.dll
                            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
  endforeach(qtlib)
endif(WIN32)

# ------------------------------------------------------------------------------
# Set MSVC Debugger
# ------------------------------------------------------------------------------
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi")
  set_property(TARGET LightFieldViewer APPEND PROPERTY LINK_FLAGS "/DEBUG /PROFILE")
endif()
