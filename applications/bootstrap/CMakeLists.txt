set(SETSUNA_APP_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/applications/bootstrap/include)

find_package(GLFW)

if(GLFW_FOUND)
    list(APPEND SETSUNA_APP_HEADER_FILES ${SETSUNA_APP_INCLUDE_DIR}/setsuna_app/app_glfw.h)
    list(APPEND SETSUNA_APP_SOURCE_FILES app_glfw.cpp)
    list(APPEND SETSUNA_APP_LIB_DIRS ${GLFW_INCLUDE_DIRS})
    list(APPEND SETSUNA_APP_LIBS ${GLFW_LIBRARIES})
endif(GLFW_FOUND)

add_library(setsuna_app
    ${SETSUNA_APP_SOURCE_FILES}
    ${SETSUNA_APP_HEADER_FILES}
)

set_target_properties(setsuna_app
   PROPERTIES
   FOLDER "applications"
)

target_include_directories(setsuna_app
    PUBLIC ${SETSUNA_APP_INCLUDE_DIR}
    PUBLIC ${SETSUNA_APP_LIB_DIRS}
    PUBLIC ${GLAD_ROOT_DIR}/include
)

target_link_libraries(setsuna_app
    ${SETSUNA_APP_LIBS}
)
